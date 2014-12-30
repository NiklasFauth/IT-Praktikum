#include "ADC.h"

ADC::ADC() {}

ADC::~ADC()
{
    cleanUpChannel(ID);
}
/************************************************************************/
/* init resettet den ADC zun�chst und setzt anschlie�end die ADC clock
und die Startup time. Danach wird die 10 bit Konvertierung eingestellt
und der sleep Modus, sowie der Hardware Trigger werden deaktiviert.
Am Ende wird noch eine einzelne Konvertierung gestartet.                                                                     */
/************************************************************************/
bool ADC::init()
{
    //ADC reset
    SET_BIT(*(VINTP)(ADC_MODULE + ADC_CR_OFFSET), 0);
    //ADC Clock einstellen
    prescalerRate = Configuration::ADCCLK / (Configuration::ADC_Internal_Clock * 2);
    //prescalerRate darf nicht zu gro� werden
    if (prescalerRate > 255)
        return false;
    //Prescale Inhalt l�schen
    CLEAR_BITS(*(VINTP)(ADC_MODULE + ADC_MR_OFFSET), 0xFF << 8);
    //prescalerRate in das Mode Register reinschreiben
    SET_BITS(*(VINTP)(ADC_MODULE + ADC_MR_OFFSET), (prescalerRate & 0xFF) << 8);
    //Startup time einstellen
    startupTime = Configuration::ADCCLK / ((prescalerRate + 1) * 800000);
    if (startupTime > 127)
        return false;
    //startup time Inhalt l�schen
    CLEAR_BITS(*(VINTP)(ADC_MODULE + ADC_MR_OFFSET), 0x7F << 16);
    //neuen Inhalt reinschreiben
    SET_BITS(*(VINTP)(ADC_MODULE + ADC_MR_OFFSET), (startupTime & 0x7F) << 16);
    SET_BITS(*(VINTP)(ADC_MODULE + ADC_MR_OFFSET), 0xF << 24);
    //Hardware Trigger deaktivieren,
    CLEAR_BIT(*(VINTP)(ADC_MODULE + ADC_MR_OFFSET), 0);
    //10-bit Konvertierung einstellen
    CLEAR_BIT(*(VINTP)(ADC_MODULE + ADC_MR_OFFSET), 4);
    //normalen Modus w�hlen, kein Sleep Mode
    CLEAR_BIT(*(VINTP)(ADC_MODULE + ADC_MR_OFFSET), 5);
    //eine Konvertierung starten
    //SET_BIT ( *( VINTP ) ( ADC_MODULE + CR_OFFSET ), 1 );
    return 1;
}
/************************************************************************/
/* Falls enabled = true, wird hier festgelegt, dass eine Peripheral
    Function den Pin kontrolliert. Au�erdem werden der Pull-Up Widerstand
    und der Output Driver deaktiviert. Dar�ber hinaus werden die Peripheral
    Mux Register �berpr�ft und gegebenenfalls gesetzt, oder gel�scht.
    Falls enabled = false, kontrolliert der Gpio den Pin  und der output
    Driver wird deaktiviert               */
/************************************************************************/
bool ADC::enableInPinSelector(unsigned long channelID, bool enabled)
{
    if (channelID >= ADC_NUM_CONFIGURED_CHANNELS  || !Configuration::ADC_gpioMultiplexData[channelID].configured)
        return 0;
    if (enabled) {
        //Peripheral Function einstellen
        CLEAR_BIT(*(VINTP)(GPIO_MODULE + GPIO_GPER_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_OFFSET), Configuration::ADC_gpioMultiplexData[channelID].pin);
        //disable pull-up resistor
        //CLEAR_BIT ( *( VINTP ) ( GPIO_MODULE + GPIO_PULL_UP_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * PORT_OFFSET ), Configuration::ADC_gpioMultiplexData[channelID].pin );
        //Output driver deaktivieren
        //CLEAR_BIT ( *( VINTP ) ( GPIO_MODULE + OUTPUT_DRIVER_ENABLE_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * PORT_OFFSET ), Configuration::ADC_gpioMultiplexData[channelID].pin );
        //PMR1 pr�fen, wenn 1, schreibe eine 1 rein, wenn nicht, schreibe eine 0 rein
        VINTP PMR0 = (VINTP)(GPIO_MODULE + GPIO_PMR0_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_OFFSET);
        VINTP PMR1 = (VINTP)(GPIO_MODULE + GPIO_PMR1_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_OFFSET);
        if (BIT_IS_SET(Configuration::ADC_gpioMultiplexData[channelID].multiplexRegisterValue, 0))
            SET_BIT(*PMR0 , Configuration::ADC_gpioMultiplexData[channelID].pin);
        else
            CLEAR_BIT(*PMR0, Configuration::ADC_gpioMultiplexData[channelID].pin);
        //PMR0 pr�fen, analog zu PMR0
        if (BIT_IS_SET(Configuration::ADC_gpioMultiplexData[channelID].multiplexRegisterValue, 1))
            SET_BIT(*PMR1, Configuration::ADC_gpioMultiplexData[channelID].pin);
        else
            CLEAR_BIT(*PMR1, Configuration::ADC_gpioMultiplexData[channelID].pin);
        return 1;
    } else {
        //GPIO controll einstellen
        SET_BIT(*(VINTP)(GPIO_MODULE + GPIO_GPER_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * GPIO_PORT_OFFSET), Configuration::ADC_gpioMultiplexData[channelID].pin);
        //Deaktivieren des Output Drivers
        //CLEAR_BIT ( *( VINTP ) ( GPIO_MODULE + OUTPUT_DRIVER_ENABLE_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * PORT_OFFSET ), Configuration::ADC_gpioMultiplexData[channelID].pin );
        return 1;
    }
}
/************************************************************************/
/*  Hier wird der aktuelle Wert des ADC im Last Converted Data Register
    ausgelesen.
    Falls getAverage true ist, wird eine Schleife aufgerufen, die in jedem
    Durchlauf zun�chst eine Konvertierung startet, anschlie�end wartet,
    bis die Konvertierung abgechlossen ist und dann die Konvertierten Werte
    aufsummiert. Am ende wird noch der Mittelwert gebildet.                                                                      */
/************************************************************************/
unsigned long ADC::getChannelValue(unsigned long channelID, bool getAverage, unsigned long numberOfConversionsForAverage)
{
    if (channelID >= ADC_NUM_CONFIGURED_CHANNELS)
        return 0;
    channelValue = 0;
    //Aktivieren des mit channelID �bergebenen ADC Kanals
    SET_BIT(*(VINTP)(ADC_MODULE + ADC_CHER_OFFSET), channelID);
    if (getAverage == true) {
        //In jedem Schleifendurchlauf wird eine Konvertierung gestartet. Daraufhin wird gewartet, bis die Konvertierung abgeschlossen ist
        //und im DRD bit des Status Registers eine 1 steht. Dies bedeutet, dass eine Konvertierung abgeschlossen wurde und im Last converted Data Register
        //Daten ausgelesen werden kann. Die ausgelesenen Werte werden in der Variablen channelValue aufsummiert.
        for (unsigned long i = 0; i < numberOfConversionsForAverage; i++) {
            //Hier wird die Konvertierung gestartet
            SET_BIT(*(VINTP)(ADC_MODULE + ADC_CR_OFFSET), 1);
            //Hier wird gewartet, bis im Data ready bit eine 1 steht
            while (!BIT_IS_SET(*(VINTP)(ADC_MODULE + ADC_SR_OFFSET), 16));
            channelValue += *(VINTP)(ADC_MODULE + ADC_LCDR_OFFSET);
        }
        channelValue = channelValue / numberOfConversionsForAverage;
    } else {
        SET_BIT(*(VINTP)(ADC_MODULE + ADC_CR_OFFSET), 1);
        while (!BIT_IS_SET(*(VINTP)(ADC_MODULE + ADC_SR_OFFSET), 16));
        channelValue = *(VINTP)(ADC_MODULE + ADC_LCDR_OFFSET);
    }
    //Deaktivieren des verwendeten Kanals und Reset des ADC
    SET_BIT(*(VINTP)(ADC_MODULE + ADC_CHDR_OFFSET), channelID);
    //SET_BIT ( *( VINTP ) ( ADC_MODULE + CR_OFFSET ), 0 );
    return channelValue;
}

void ADC::cleanUpChannel(unsigned char channelID)
{
    ///Channel disable
    SET_BIT(*(VINTP)(ADC_MODULE + ADC_CHDR_OFFSET), channelID);
    ///Output driver deaktivieren
    //CLEAR_BIT ( *( VINTP ) ( GPIO_MODULE + OUTPUT_DRIVER_ENABLE_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * PORT_OFFSET ), Configuration::ADC_gpioMultiplexData[channelID].pin );
    ///disable pull-up resistor
    //CLEAR_BIT ( *( VINTP ) ( GPIO_MODULE + GPIO_PULL_UP_OFFSET + Configuration::ADC_gpioMultiplexData[channelID].port * PORT_OFFSET ), Configuration::ADC_gpioMultiplexData[channelID].pin );
    ///ADC reset
    SET_BIT(*(VINTP)(ADC_MODULE + ADC_CR_OFFSET), 0);
}