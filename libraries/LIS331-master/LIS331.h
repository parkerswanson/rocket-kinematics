/**************************************************************************/
/*!
    @file     LIS331.h
*/
/**************************************************************************/

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define LIS331_DEFAULT_ADDRESS  (0x18)    // if SDO/SA0 is 3V, it's 0x19
/*=========================================================================*/
#define LIS331_REG_CTRL1         0x20
#define LIS331_REG_CTRL4         0x23
#define LIS331_REG_OUT_X_L       0x28
#define LIS331_REG_OUT_X_H       0x29
#define LIS331_REG_OUT_Y_L       0x2A
#define LIS331_REG_OUT_Y_H       0x2B
#define LIS331_REG_OUT_Z_L       0x2C
#define LIS331_REG_OUT_Z_H       0x2D

//Constants to write to registers
#define LIS331_CTRL1_bits		 0x37  // Normal, 400 Hz, xyz 
#define LIS331_CTRL4_bits		 0x30  // +/- 24g range

class LIS331 : public Adafruit_Sensor {
 public:

  LIS331(void);
  
  bool begin(uint8_t addr = LIS331_DEFAULT_ADDRESS);

  void read(void); 

  bool getEvent(sensors_event_t *event);
  void getSensor(sensor_t *sensor);

  int16_t x, y, z;
  float x_g, y_g, z_g;

 private:
  
  uint8_t readRegister8(uint8_t reg);
  void writeRegister8(uint8_t reg, uint8_t value);

  int8_t  _i2caddr;
  
};
