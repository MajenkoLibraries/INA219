#ifndef _INA219_H
#define _INA219_H

#include <Arduino.h>
#ifdef __PIC32__
#include <DTWI.h>
#else
#include <Wire.h>
#endif

class INA219 {
private:
#ifdef __PIC32__
	DTWI *_dtwi;
#else
    TwoWire *_wire;
#endif
	uint8_t _addr;
    float _shunt;

	uint16_t readRegister(uint8_t reg);
	void writeRegister(uint8_t reg, uint16_t val);

    uint16_t _config;

public:
    static const uint8_t SENSE_40MV     = 0;
    static const uint8_t SENSE_80MV     = 1;
    static const uint8_t SENSE_160MV    = 2;
    static const uint8_t SENSE_320MV    = 3;

    static const uint8_t SENSE_84US     = 0;
    static const uint8_t SENSE_148US    = 1;
    static const uint8_t SENSE_276US    = 2;
    static const uint8_t SENSE_532US    = 3;

    static const uint8_t SENSE_1060US   = 9;
    static const uint8_t SENSE_2130US   = 10;
    static const uint8_t SENSE_4260US   = 11;
    static const uint8_t SENSE_8510US   = 12;
    static const uint8_t SENSE_17020US  = 13;
    static const uint8_t SENSE_34050US  = 14;
    static const uint8_t SENSE_68100US  = 15;

    static const uint8_t SENSE_16V      = 0;
    static const uint8_t SENSE_32V      = 1;

    static const uint8_t SENSE_TRIGGER  = 0x0;
    static const uint8_t SENSE_SHUNT    = 0x1;
    static const uint8_t SENSE_BUS      = 0x2;
    static const uint8_t SENSE_CONTINUOUS = 0x4;

#ifdef __PIC32__
	INA219(DTWI *dtwi, uint8_t addr = 0x40) : _dtwi(dtwi), _addr(addr) {}
	INA219(DTWI &dtwi, uint8_t addr = 0x40) : _dtwi(&dtwi), _addr(addr) {}
#else
    INA219(TwoWire *wire, uint8_t addr = 0x40) : _wire(wire), _addr(addr) {}
    INA219(TwoWire &wire, uint8_t addr = 0x40) : _wire(&wire), _addr(addr) {}
#endif

	void setCurrentSampleTime(uint8_t st);
	void setVoltageSampleTime(uint8_t st);
	void setShuntResistance(float res);
	void setCurrentRange(uint8_t i);

    void setVoltageRange(uint8_t range);

	float readVoltage();
	float readCurrent();

    void setMode(uint8_t mode);
	void begin();
};

#endif
