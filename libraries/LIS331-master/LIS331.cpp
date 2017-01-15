/**************************************************************************/
/*!
    @file     LIS331.cpp
*/
/**************************************************************************/

#include "Arduino.h"
#include <Wire.h>
#include <LIS331.h>

/**************************************************************************/
/*!
    @brief  Instantiates a new LIS331 class in I2C
*/
/**************************************************************************/
// I2C
LIS331::LIS331()
{
}
/**************************************************************************/
/*!
    @brief  Setup the HW
*/
/**************************************************************************/
bool LIS331::begin(uint8_t i2caddr) {

  _i2caddr = i2caddr;
  
  Wire.begin();
  writeRegister8(LIS331_REG_CTRL1, LIS331_CTRL1_bits);  
  writeRegister8(LIS331_REG_CTRL4, LIS331_CTRL4_bits);

  return true;
}

void LIS331::read(void) {
  // read x y z at once

    Wire.beginTransmission(_i2caddr);
    Wire.write(LIS331_REG_OUT_X_L | 0x80); // 0x80 for autoincrement
    Wire.endTransmission();

    Wire.requestFrom(_i2caddr, 6);
    x = Wire.read(); x |= ((uint16_t)Wire.read()) << 8;
    y = Wire.read(); y |= ((uint16_t)Wire.read()) << 8;
    z = Wire.read(); z |= ((uint16_t)Wire.read()) << 8;
 
  uint16_t divider = 1365;  

  x_g = (float)x / divider;
  y_g = (float)y / divider;
  z_g = (float)z / divider;

}

/**************************************************************************/
/*!
    @brief  Gets the most recent sensor event
*/
/**************************************************************************/
bool LIS331::getEvent(sensors_event_t *event) {
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  event->version   = sizeof(sensors_event_t);
  event->type      = SENSOR_TYPE_ACCELEROMETER;
  event->timestamp = 0;

  read();

  event->acceleration.x = x_g * SENSORS_GRAVITY_STANDARD;
  event->acceleration.y = y_g * SENSORS_GRAVITY_STANDARD;
  event->acceleration.z = z_g * SENSORS_GRAVITY_STANDARD;
}

/**************************************************************************/
/*!
    @brief  Gets the sensor_t data
*/
/**************************************************************************/
void LIS331::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "LIS331", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->type        = SENSOR_TYPE_ACCELEROMETER;
  sensor->min_delay   = 0;
  sensor->max_value   = 0;
  sensor->min_value   = 0;
  sensor->resolution  = 0;
}

/**************************************************************************/
/*!
    @brief  Writes 8-bits to the specified destination register
*/
/**************************************************************************/
void LIS331::writeRegister8(uint8_t reg, uint8_t value) {

    Wire.beginTransmission((uint8_t)_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
    Wire.endTransmission();   
}

/**************************************************************************/
/*!
    @brief  Reads 8-bits from the specified register
*/
/**************************************************************************/
uint8_t LIS331::readRegister8(uint8_t reg) {
  uint8_t value;

    Wire.beginTransmission(_i2caddr);
    Wire.write((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(_i2caddr, 1);
    value = Wire.read();  
  return value;
}
