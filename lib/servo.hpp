//
// Created by ethepherin on 2021/4/1.
//
//#define ClionArduino

#ifndef my_servo
#define my_servo

#ifdef ClionArduino
#include <Servo/src/Servo.h>
#else
#include <Servo.h>
#endif

#include "Arduino.h"
#include "timeQueue.hpp"

#define arm_n 2
#define eachArmServo_n 5
#define leftArm 0
#define rightArm 1
#define servoMoveMills 4
//                                                        L1,    L2,    L3,    L4,    L5,      R1,    R2,    R3,    R4,    R5
const int constPin[arm_n][eachArmServo_n] =             {{9,     8,     7,     10,    11},    {6,     5,     4,     2,     3}};
const int initialPos[arm_n][eachArmServo_n] =           {{90,    90,    95,    90,    90},    {90,    90,    85,    90,    90}};
const double armReachIndex[arm_n][eachArmServo_n] =     {{0.0,   0.0,   1.0,   2.0,   -1.0},  {0.0,   0.0,   -1.0,  2.0,   -1.0}};
const double armReachIntercept[arm_n][eachArmServo_n] = {{0.0,   0.0,   5.0,   -99.0, 180.0}, {0.0,   0.0,   175.0, -99.0, 180.0}};

const double armSpeed  = 0.0625;
const double servoTwoSpeed = 1.0;

struct _arm{
    int id;
    unsigned long nextMoveMills;
    double currentArmPosition, currentServoTwoPosition;
    _timeQueue timeQueue;
    _arm() = default;
    Servo servo[eachArmServo_n];
    void reset(){
        for (int i = 0; i < eachArmServo_n; ++i)
            servo[i].write(initialPos[this->id][i]);
        currentArmPosition = 90.0;
        currentServoTwoPosition = 90.0;
        nextMoveMills = 0;
    }

    void init(int id){
        this->id = id;
        for (int i = 0; i < eachArmServo_n; ++i) {
//            pinMode(constPin[this->id][i], OUTPUT);
            servo[i].attach(constPin[this->id][i]);
        }
        reset();
    }

    bool reachArm(double toArmPosition) {//toArmPosition:90~30(90:the initial position,30:the furthest postion
        if (abs(toArmPosition - this->currentArmPosition) < 0.1)
            return true;
        if (millis() < this->nextMoveMills)
            return false;

        this->nextMoveMills = millis() + servoMoveMills;
        if (this->currentArmPosition > toArmPosition)
            this->currentArmPosition -= armSpeed;
        else
            this->currentArmPosition += armSpeed;
//        Serial.println(this->currentArmPosition);
        for (int i = 2; i < 5; ++i)
            this->servo[i].write(armReachIndex[this->id][i] * this->currentArmPosition + armReachIntercept[this->id][i]);
        return false;
    }

    bool rotateServoTwo(double toServoTwoPosition){
        if (abs(toServoTwoPosition - this->currentServoTwoPosition) < servoTwoSpeed + 0.05)
            return true;
        if (millis() < this->nextMoveMills)
            return false;

        this->nextMoveMills = millis() + servoMoveMills;
        if (this->currentServoTwoPosition > toServoTwoPosition)
            this->currentServoTwoPosition -= servoTwoSpeed;
        else
            this->currentServoTwoPosition += servoTwoSpeed;
        servo[1].write(this->currentServoTwoPosition);
    }

    void loopRun(){
        if (this->timeQueue.empty())
            return;

        if (signal >= this->timeQueue.frontPoint()->startSignal)
            switch(this->timeQueue.front().id){
                case timeUnitBeingDelay:
                    if (millis() >= this->timeQueue.front().index) {
                        signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                        this->timeQueue.pop();
                    }
                    break;
                case timeUnitDelay:
                    this->timeQueue.frontPoint()->id = timeUnitBeingDelay;
                    this->timeQueue.frontPoint()->index = millis() + (*this->timeQueue.frontPoint()).index;
                    break;

                case timeUnitReachArm:
                    if (this->reachArm(this->timeQueue.frontPoint()->index)) {
                        signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                        this->timeQueue.pop();
                    }
                    break;

                case timeUnitRotateServoTwo:
                    if (this->rotateServoTwo(this->timeQueue.frontPoint()->index)){
                        signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                        this->timeQueue.pop();
                    }
                    break;
        }
    }
}arm[arm_n];

void moveArm(int armNum, double position, int startSignal = -1, int finishSignal = -1) {//armNum: leftArm/rightArm, Position:90~30(90:the initial position,30:the furthest postion
    arm[armNum].timeQueue.push(timeUnitReachArm, position, startSignal, finishSignal);
}

void rotateServoTwo(int armNum, double position, int startSignal = -1, int finishSignal = -1){//armNum: leftArm/rightArm,
    arm[armNum].timeQueue.push(timeUnitRotateServoTwo, position, startSignal, finishSignal);
}

void delayArm(int armNum, double delay, int startSignal = -1, int finishSignal = -1){//delay单位:秒
    arm[armNum].timeQueue.push(timeUnitDelay, delay * 1e3, startSignal, finishSignal);
}

#endif

