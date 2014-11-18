#ifndef BITMACROS_H
#define BITMACROS_H


/* set bit */
#define SET_BIT(var, bit) ((var) |= (1u << (bit)))

/* set bits */
#define SET_BITS(var, bits) ((var) |= (bits))

/* clear bit */
#define CLEAR_BIT(var, bit) ((var) &= (unsigned)~(1u << (bit)))

/* clear bits */
#define CLEAR_BITS(var, bits) ((var) &= (unsigned)~(bits))

/* bit is set/clear */
#define BIT_IS_SET(var, bit) ((var) & (1u << (bit)))
#define BIT_IS_CLEAR(var, bit) !BIT_IS_SET(var, bit)


#endif /* BITMACROS_H */

// adresses
const int GPIO_ADRESS = 0xFFFF1000;
const int PORT_OFFSET = 0x0100;
const int PMR0_ADRESS = 0x10; // peri mux 0
const int PMR1_ADRESS = 0x20; // peri mux 1
const int ODER_ADRESS = 0x40; // output driver
const int OVR_ADRESS = 0x50; // output value
const int PVR_ADRESS = 0x60; // value
const int PUER_ADRESS = 0x70; // pull-up
const int GFER_ADRESS = 0xC0; // glitch filter