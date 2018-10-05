#include "KM1_I2C.h"
#include "TypeUtility.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"


// #define _ESP32_HAL_I2C_H_

KeiganMotor::KeiganMotor(uint8_t address)
{
    init(address);
}

void KeiganMotor::init(uint8_t address)
{
    _address = address;
    _commandID = 0;

}

void KeiganMotor::begin()
{
    Wire.begin();
}


/*
 **@brief Data frame to send to KeiganMotor
 * 
 *      [0]          [1][2]           [3]...[n-3]		 [n-2][n-1]	
 * uint8_t command	uint16_t id		uint8_t *value		 uint16_t CRC		
 *      0~0xFF	     0～0xFFFF      depends on command    0～0xFFFF   
 */

/*
 **@brief Read response from KeiganMotor 
 * 
 *      [0]          [1][2]         	[3]		          [4]...[n-3]		 [n-2][n-1]	
 * uint8_t tx_type	uint16_t id		uint8_t command 	uint8_t *value		 uint16_t CRC		
 *      0x40	     0～0xFFFF         0~0xFF   		depends on command   0～0xFFFF   
 */


void KeiganMotor::appendID(uint8_t *data)
{
    uint16_big_encode(_commandID, data);
    if(_commandID == 0xFFFF){
        _commandID = 0;
    } else {
        _commandID++;
    }
}


void KeiganMotor::write(uint8_t command, uint8_t *value, uint8_t value_len)
{
    uint8_t len = value_len + 5;
    uint8_t data[255] = {0};
    data[0] = command;
    appendID(&data[1]);
    if(value_len > 0){
        memcpy(&data[3], value, value_len);
    }
    // TODO CheckSum
    Wire.beginTransmission(_address);
    Wire.write(data, len);
    Wire.endTransmission();
}

void KeiganMotor::readRegister(uint8_t reg, uint8_t *value, uint8_t value_len)
{
    // TODO
    uint8_t data[] = {CMD_REG_READ_REGISTER, 0, 0, 0, 0};

    Wire.beginTransmission(_address);
    Wire.write(data, CMD_REG_READ_REGISTER_LEN);
    Wire.endTransmission();

    uint8_t len = value_len + 6;
    Wire.beginTransmission(_address);
    Wire.requestFrom(_address, len);

    uint8_t cnt = 0;
    while (Wire.available()) { // slave may send less than requested
    
        value[cnt] = Wire.read();
        cnt ++;
    }
    
    Wire.endTransmission();
}

float KeiganMotor::readFloat(uint8_t reg)
{
    uint8_t value[sizeof(float)]; 
    readRegister(reg, value, sizeof(float));
}


// Set motion control curve type 0:None, 1:Trapezoid
void KeiganMotor::curveType(uint8_t curveType)
{
    uint8_t data[] = {curveType};
    write(CMD_REG_CURVE_TYPE, data, sizeof(data));
}


// Set acceleration [rad/s^2]
void KeiganMotor::acc(float value)
{
    uint8_t data[sizeof(float)] = {0};
    float_big_encode(value, data);
    write(CMD_REG_ACC, data, sizeof(float));
}

// Set deceleration [rad/s^2]
void KeiganMotor::dec(float value)
{
    uint8_t data[sizeof(float)] = {0};
    float_big_encode(value, data);
    write(CMD_REG_DEC, data, sizeof(float));
}


// Set I2C Slave address
// NOTE) Need to saveAllRegisters() and reboot() to reflect change.
void KeiganMotor::i2cSlaveAddress(uint8_t address)
{
    uint8_t data[] = {address};
    write(CMD_REG_I2C_SLAVE_ADDR, data, sizeof(data));
}

void KeiganMotor::saveAllRegisters()
{
    write(CMD_REG_SAVE_ALL_REGISTERS, NULL, 0);
}

void KeiganMotor::resetAllRegisters()
{
    write(CMD_REG_RESET_ALL_REGISTERS, NULL, 0);   
}

void KeiganMotor::enable()
{
    write(CMD_DRIVER_ENABLE, NULL, 0);
}

void KeiganMotor::disable()
{
    write(CMD_DRIVER_DISABLE, NULL, 0);
}


void KeiganMotor::speed(float value)
{
    uint8_t data[sizeof(float)] = {0};
    float_big_encode(value, data);
    write(CMD_ACT_SPEED, data, sizeof(float));
}


void KeiganMotor::speedRpm(float rpm)
{
    speed(RPM_TO_RADPERSEC(rpm));
}   


void KeiganMotor::runAtVelocity(float value)
{
    uint8_t data[sizeof(float)] = {0};
    float_big_encode(value, data);
    write(CMD_ACT_RUN_AT_VELOCITY, data, sizeof(float));
}

void KeiganMotor::runAtVelocityRpm(float rpm)
{
    runAtVelocity(RPM_TO_RADPERSEC(rpm));
}

void KeiganMotor::presetPosition(float position)
{
    // TODO
}

void KeiganMotor::runForward()
{
    write(CMD_ACT_RUN_FORWARD, NULL, 0);
}

void KeiganMotor::runReverse()
{
    write(CMD_ACT_RUN_REVERSE, NULL, 0);
}

void KeiganMotor::moveToPosition(float position)
{
    uint8_t data[sizeof(float)] = {0};
    float_big_encode(position, data);
    write(CMD_ACT_MOVE_TO_POS, data, sizeof(float));
}

void KeiganMotor::moveToPositionDegree(float degree)
{
    moveToPosition(DEGREES_TO_RADIANS(degree));
}

void KeiganMotor::moveByDistance(float distance)
{
    uint8_t data[sizeof(float)] = {0};
    float_big_encode(distance, data);
    write(CMD_ACT_MOVE_BY_DIST, data, sizeof(float));
}

void KeiganMotor::moveByDistanceDegree(float degree)
{
    moveByDistance(DEGREES_TO_RADIANS(degree));
}

void KeiganMotor::stop()
{
    write(CMD_ACT_STOP, NULL, 0);

}
void KeiganMotor::free()
{
    write(CMD_ACT_FREE, NULL, 0);
}

void KeiganMotor::wait(uint32_t time)
{
    uint8_t data[sizeof(float)] = {0};
    uint32_big_encode(time, data);
    write(CMD_QUE_WAIT, data, sizeof(float));
}

void KeiganMotor::led(uint8_t state, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t data[4] = {(uint8_t)state, r, g, b};
    write(CMD_LED_SET_LED, data, sizeof(data));
}

void KeiganMotor::reboot()
{
    write(CMD_OTHERS_REBOOT, NULL, 0);
}
