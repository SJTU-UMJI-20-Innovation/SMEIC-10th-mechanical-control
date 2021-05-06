#include "pipe&motor.hpp"
#include "timeQueue.hpp"

#include <SPI.h>
#define BUFFER_SIZE 6
#define SPIDebug(sth) if(latePrint){print[print_n] = (sth) + printIntDoubleIntInt(buf); print_n++;}

volatile bool spiResendFlag = false;
volatile bool spiSendingFlag = false;
volatile unsigned long spiStartTime = 0;

const uint8_t polynome = 0x7;
const int piCommunicationPin = 4;

int print_n = 0;
unsigned long print_time = 0;
String print[10];

void initSPI() {
    pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);
    SPCR |= _BV(SPIE);
}

void _processCmd(byte* buf);
float _thirteenBitsToFloat(uint16_t rawData);



volatile void processOneByte() {
//    if (latePrint){
//        if (print_n == 0)
//            print_time = millis() + 3000;
//        print[print_n] = "p\n";
//        print_n++;
//    }
    static byte buf[BUFFER_SIZE];
    static uint8_t bufPos = 0;
    static uint8_t remainingBytes = 1;

    if (spiResendFlag){
        bufPos = 0;
        remainingBytes = 1;
        spiResendFlag = false;
    }

    digitalWrite(piCommunicationPin, LOW);

    byte b = SPDR;
    buf[bufPos] = b;
    remainingBytes --;
    if (bufPos) {
        if (remainingBytes) {
            bufPos ++;
        } else {
            _processCmd(buf);
//            memset(buf, 0, BUFFER_SIZE);
            bufPos = 0;
            remainingBytes = 1;
            spiSendingFlag = false;
        }
    } else {

        //5bit command 3bit length
        spiSendingFlag = true;
        spiStartTime = micros();
        remainingBytes = 5;
        bufPos ++;
    }
}

String printIntDoubleIntInt(byte* buf){
    String temp = String((buf[1] & 0x60) >> 5) + " rawData: " + String(((buf[1] & 0x1F) << 8) + buf[2])
            + " Data: "+ String(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2])) + ' ' +
            String(buf[3]) + ' ' + String(buf[4]) + "\n";
//    Serial.print((buf[1] & 0x60) >> 5);
//    Serial.print(" rawData: ");
//    Serial.print(((buf[1] & 0x1F) << 8) + buf[2]);
//    Serial.print(" Data: ");
//    Serial.print(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]));
//    Serial.print(' ');
//    Serial.print(buf[3]);
//    Serial.print(' ');
//    Serial.println(buf[4]);
    return temp;
}

uint8_t CRC8(byte* buf){
    uint8_t _crc = 0;
    for (int i = 0; i < BUFFER_SIZE; ++i){
        _crc ^= buf[i];
        for (int j = 8; j; --j){
            if (_crc & (1 << 7)){
                _crc <<= 1;
                _crc ^= polynome;
            }
            else
                _crc <<= 1;
        }
    }
    return _crc;
}


void _processCmd(byte* buf) {
//    Serial.println("in process cmd");
//        Serial.println(buf[0]);
//    Serial.println(buf[1]);

//    Serial.println(buf[2]);

//    Serial.println(buf[3]);

//    Serial.println(buf[4]);

//    Serial.println(buf[5]);


    uint8_t CRC8Check = CRC8(buf);
    if (CRC8Check != 0){
        digitalWrite(piCommunicationPin, HIGH);
        spiResendFlag = true;
        if (latePrint){
            print[print_n] = "Fail CRC check, callback: " + String(CRC8Check) + "\n\n";
            print_n++;
        }
        return;
    }

    if (latePrint){
        print[print_n] = "Pass CRC check\n";
        print_n++;
    }

    switch (buf[0] >> 3) {
        case 10: //changeMass 1bit reserved 2bit armNum 13bit data 8bit startSignal 8bit endSignal
            SPIDebug("getLiquid ")
            getLiquid((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            break;

        case 12:
            SPIDebug("getSolid ")
            getSolid(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
            break;

    }
//        case 1: //stop
//            stop();
//            break;
//        case 3: //changeSignal 8bit changeLast 8bit signal
//            changeSignal(buf[1]);
//            break;
//        case 4: //rotateBase 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
//            if (buf[1] >> 7) {
//                SPIDebug("delayRotationBase ")
//                delayRotationBase((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            } else {
//                SPIDebug("rotateRotationBaseTo ")
//                rotateRotationBaseTo((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            }
//            break;
//        case 5: //liftBase 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
//            if (buf[1] >> 7) {
//                SPIDebug("delayLifeBase ")
//                delayLiftBase((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            } else {
//                SPIDebug("liftBaseTo ")
//                liftBaseTo((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            }
//            break;
//        case 6: //moveBase 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
//            if (buf[1] >> 7) {
//                SPIDebug("delayMoveBase ")
//                delayMoveBase(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            } else {
//                SPIDebug("moveBaseTo ")
////                Serial.println(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]));
//                moveBaseTo(_thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            }
//            break;
//        case 7: //moveArm 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
//            if (buf[1] >> 7) {
//                SPIDebug("delayArm ")
//                delayArm((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            } else {
//                SPIDebug("moveArm ")
//                moveArm((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            }
//            break;
//        case 8: //rotateServo 1bit delay 2bit armNum 13bit data 8bit startSignal 8bit endSignal
//            if (buf[1] >> 7) {
//                SPIDebug("rotateServoTwo ")
//                rotateServoTwo((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            }
//            else{
//                SPIDebug("rotateServoOne ")
//                rotateServoOne((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            }
//            break;
//        case 9: //changeMass 1bit reserved 2bit armNum 13bit data 8bit startSignal 8bit endSignal
//            SPIDebug("changeMass ")
//            changeMass((buf[1] & 0x60) >> 5, _thirteenBitsToFloat(((buf[1] & 0x1F) << 8) + buf[2]), buf[3], buf[4]);
//            break;
//    }
}

float _thirteenBitsToFloat(uint16_t rawData) {
    if (rawData > 0) {
        return (float) ((int)rawData - 4096) / (float) 16;
    }
}