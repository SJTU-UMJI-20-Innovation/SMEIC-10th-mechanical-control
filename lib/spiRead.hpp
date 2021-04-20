#include "servo.hpp"
#include "stepperMotor.hpp"
#include "action.hpp"

#include <SPI.h>
#define BUFFER_SIZE 5
#define SPIDebug(sth) if(ifDebug){Serial.print(sth);printIntDoubleIntInt(buf);}

void initSPI() {
    pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);
    SPCR |= _BV(SPIE);
}

void _processCmd(byte* buf);
float _thirteenBitsToFloat(uint16_t rawData);



volatile void processOneByte() {
    static byte buf[BUFFER_SIZE];
    static uint8_t bufPos = 0;
    static uint8_t remainingBytes = 0;

    byte b = SPDR;
    buf[bufPos] = b;
    if (bufPos) {
        if (remainingBytes > 1) {
            remainingBytes --;
            bufPos ++;
        } else {
            _processCmd(buf);
            memset(buf, 0, BUFFER_SIZE);
            bufPos = 0;
        }
    } else {
        //5bit command 3bit length
        remainingBytes = b & 0x07;
        bufPos ++;
    }
}

void printIntDoubleIntInt(byte* buf){
    Serial.print((buf[1] & 0x60) >> 5);
    Serial.print(" rawData: ");
    Serial.print(((buf[1] & 0x1F) << 8) + buf[2]);
    Serial.print(" Data: ");
    Serial.print(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]));
    Serial.print(' ');
    Serial.print(buf[3]);
    Serial.print(' ');
    Serial.println(buf[4]);
}

void _processCmd(byte* buf) {
    if (debugMode) {
//        Serial.println("process Cmd");
//        Serial.println(buf[0]);
//        Serial.println(buf[1]);
//        Serial.println(buf[2]);
//        Serial.println(buf[3]);
//        Serial.println(buf[4]);
    }
    switch (buf[0] >> 3) {
        case 1: //stop
            stop();
            break;
        case 3: //changeSignal 8bit changeLast 8bit signal
            changeSignal(buf[1]);
            break;
        case 4: //rotateBase 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
            if (buf[1] >> 7) {
                SPIDebug("delayRotationBase ")
                delayRotationBase((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            } else {
                SPIDebug("rotateRotationBaseTo ")
                rotateRotationBaseTo((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            }
            break;
        case 5: //liftBase 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
            if (buf[1] >> 7) {
                SPIDebug("delayLifeBase ")
                delayLiftBase((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            } else {
                SPIDebug("liftBaseTo ")
                liftBaseTo((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            }
            break;
        case 6: //moveBase 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
            if (buf[1] >> 7) {
                SPIDebug("delayMoveBase ")
                delayMoveBase(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            } else {
                SPIDebug("moveBaseTo ")
//                Serial.println(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]));
                moveBaseTo(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            }
            break;
        case 7: //moveArm 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
            if (buf[1] >> 7) {
                SPIDebug("delayArm ")
                delayArm((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            } else {
                SPIDebug("moveArm ")
                moveArm((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            }
            break;
        case 8: //rotateServo 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
            if (buf[1] >> 7) {
                SPIDebug("rotateServoTwo ")
                rotateServoTwo((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            }
            else{
                SPIDebug("rotateServoOne ")
                rotateServoOne((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            }
            break;
        case 9: //changeMass 1bit reserved 2bit armNum 13bit data 8bit startSignal 8bit endSignal
            SPIDebug("changeMass ")
            changeMass((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            break;
    }
}

float _thirteenBitsToFloat(uint16_t rawData) {
    if (rawData > 0) {
        return (float) ((int)rawData - 4096) / (float) 16;
    }
}
