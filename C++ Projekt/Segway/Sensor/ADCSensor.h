/*! \brief
        In der ADCSensor Klasse sind primaer set und get Methoden implementiert, um fest zu legen, welcher ADC Kanal verwendet
        wird, ob ein slope factor, oder ein offset verwendet werden und um die entsprechenden Einstellungen aus zu lesen.
        Darueber hinaus wird in der getIntegerValue die ADC Methode getChannelValue verwendet, um aus einem Speziellen Kanal einen
        Wert aus zu lesen und gegebenenfalls einen Offset zu subtrahieren.
        Mit den Methoden dieser Klasse werden den Variablen in der Segway Klasse ihre Werte zugewiesen.


*/

#ifndef ADCSENSOR_H_
#define ADCSENSOR_H_

#include "ADC.h"

class ADCSensor {
  private:
    Configuration::s_ADCSensorConfig* adcSensor;
    ADC* controller;
    Configuration::s_gpioMultiplexData* multiplexData;
    signed long integerValue;
    float floatValue;
    unsigned long channelID;
    VINTP disable;

  public:
    ADCSensor();
    ~ADCSensor();
    bool init(Configuration::s_ADCSensorConfig* thisADCSensorConfig_, ADC* ADCController_);
    long getIntegerValue(bool average = false, unsigned long numberOfValuesForAverage = 0);
    void setZeroOffset(bool active, signed long offset);
    bool getZeroOffsetIsActive();
    signed long getZeroOffset();
    float getFloatValue(bool average, unsigned long numberOfValuesForAverage);
    void setSlopeFactor(bool active, float factor);
    bool getSlopeFactorIsActive(void);
    float getSlopeFactor(void);
    void setChannelID(unsigned long newChannelID);
    unsigned long getChannelID(void);
};

#endif /* ADCSENSOR_H_ */