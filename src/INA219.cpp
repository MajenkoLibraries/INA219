#include <INA219.h>

#ifdef __PIC32__
uint16_t INA219::readRegister(uint8_t reg) {
    uint8_t state = 0;
    uint32_t ts = millis();

    uint8_t h = 0;
    uint8_t l = 0;

    while (1) {
        if (millis() - ts > 100) {
            _dtwi->stopMaster();
            return 0;
        }

        switch (state) {
            case 0: // begin write
                if (_dtwi->startMasterWrite(_addr)) {
                    state = 1;
                }

                break;

            case 1: // Send register address
                if (_dtwi->write(&reg, 1) == 1) {
                    state = 2;
                }

                break;

            case 2: // Stop Master
                if (_dtwi->stopMaster()) {
                    state = 3;
                }

                break;

            case 3:
                if (_dtwi->startMasterRead(_addr, 2)) {
                    state = 4;
                }

                break;

            case 4:
                if (_dtwi->available()) {
                    _dtwi->read(&h, 1);
                    state = 5;
                }

                break;

            case 5:
                if (_dtwi->available()) {
                    _dtwi->read(&l, 1);
                    state = 6;
                }

                break;

            case 6:
                if (_dtwi->stopMaster()) {
                    return (h << 8) | l;
                }

                break;
        }
    }
}

void INA219::writeRegister(uint8_t reg, uint16_t val) {
    uint8_t state = 0;
    uint32_t ts = millis();

    uint8_t h = val >> 8;
    uint8_t l = val;

    while (1) {
        if (millis() - ts > 100) {
            _dtwi->stopMaster();
        }

        switch (state) {
            case 0: // begin write
                if (_dtwi->startMasterWrite(_addr)) {
                    state = 1;
                }

                break;

            case 1: // Send register address
                if (_dtwi->write(&reg, 1) == 1) {
                    state = 2;
                }

                break;

            case 2: // Send register address
                if (_dtwi->write(&h, 1) == 1) {
                    state = 3;
                }

                break;
            case 3: // Send register address
                if (_dtwi->write(&l, 1) == 1) {
                    state = 4;
                }

                break;

            case 4: // Stop Master
                if (_dtwi->stopMaster()) {
                    return;
                }

                break;
        }
    }
}
#else
uint16_t INA219::readRegister(uint8_t reg) {
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->endTransmission();
    _wire->requestFrom(_addr, (uint8_t)2);
    uint8_t h = Wire.read();
    uint8_t l = Wire.read();
    return (h << 8) | l;
}

void INA219::writeRegister(uint8_t reg, uint16_t val) {
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->write(val >> 8);
    _wire->write(val);
    _wire->endTransmission();
}
#endif

void INA219::begin() {

    _config = 0x399f;

#ifdef __PIC32__
    _dtwi->beginMaster();
#else
//    Wire.begin();
#endif
    writeRegister(0x00, 0x8000); // Reset
    writeRegister(0x00, _config);
    setMode(SENSE_CONTINUOUS | SENSE_BUS | SENSE_SHUNT);
    setCurrentSampleTime(SENSE_532US);
    setVoltageSampleTime(SENSE_532US);
    setShuntResistance(0.1);
    setCurrentRange(SENSE_40MV);
    setVoltageRange(SENSE_32V);
}

void INA219::setCurrentSampleTime(uint8_t st) {
    _config &= 0b0111111110000111;
    _config |= ((st & 0xf) << 3);
    writeRegister(0x00, _config);
}

void INA219::setVoltageSampleTime(uint8_t st) {
    _config &= 0b0111100001111111;
    _config |= ((st & 0xf) << 7);
    writeRegister(0x00, _config);
}

void INA219::setShuntResistance(float res) {
    _shunt = res;
}

void INA219::setCurrentRange(uint8_t i) {
    _config &= 0b0110011111111111;
    _config |= ((i & 0x3) << 11);
    writeRegister(0x00, _config);
}

void INA219::setVoltageRange(uint8_t range) {
    _config &= 0b0101111111111111;
    _config |= ((range & 0x1) << 13);
    writeRegister(0x00, _config);
}

float INA219::readVoltage() {
    uint16_t v = readRegister(0x02);
    v >>= 3;
    return (float)v * 0.004; // 4mV LSB
}

float INA219::readCurrent() {
    int16_t v = readRegister(0x01);
    float volts = (float)v * 0.00001; // 10µV LSB
    return volts / _shunt;  // I=V/R
}

void INA219::setMode(uint8_t mode) {
    _config &= 0b0111111111111000;
    _config |= 7; //(mode & 0x7);
    writeRegister(0x00, _config);
}


