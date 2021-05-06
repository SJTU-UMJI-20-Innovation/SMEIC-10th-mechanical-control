//
// Created by ethepherin on 2021/4/1.
//
//#define ClionArduino

#ifndef myServo
#define myServo

#ifdef ClionArduino
#include <Servo/src/Servo.h>
#else
#include <Servo.h>
#endif

#include "Arduino.h"
#include "timeQueue.hpp"

#define arm_n 2
#define eachArmServo_n 5

#define servoMoveMills 4

#define cameraUpDown 0
// 50~180
#define cameraLeftRight 1
//
//                                                        L1,    L2,    L3,    L4,    L5,      R1,    R2,    R3,    R4,    R5
const int constPin[arm_n][eachArmServo_n] =             {{9,     8,     7,     10,    11},    {6,     5,     4,     2,     3}};
const float initialPos[arm_n][eachArmServo_n] =           {{90,    94,    94,    90,    92},    {90,    88,    88,    90,    90.5}};

const float armReachIndex[arm_n][eachArmServo_n] =     {{0.0,   0.0,   0.85,   2.0,   -0.8},  {0.0,   0.0,   -1.15,  2.0,   -0.8}};

const float weightIndex[arm_n][eachArmServo_n] =       {{0.0,   0.0,   0.0,    0.0,  -1.5},   {0.0,   0.0,     0.0,   0.0,  -1.5}};
////调整参数                                                                                                                 -1.5
//                                                               越重    -a     -2a    -a                     +a     -2a    -a
const float armReachIntercept[arm_n][eachArmServo_n] = {{0.0,   0.0,   17.5,   -90.0,  164.0}, {0.0,   0.0,   193.5, -90.0, 164.0}};

const int cameraPin[2] = {13, 12};
const float initialCameraPos[2] = {90, 90};

const float armSpeed  = 0.0625;
const float servoTwoSpeed = 1.0;
const float grabSpeed = 0.1;


const float releasePosition = 80.0;
const float grabPosition = 90.0;

struct _arm{
    int id;
    unsigned long nextMoveMills;
    float currentArmPosition, currentServoTwoPosition, currentGrabPosition, mass;
    _timeQueue timeQueue;
    _arm() = default;
    Servo servo[eachArmServo_n];
    void reset(){
        for (int i = 0; i < eachArmServo_n; ++i)
            servo[i].write(initialPos[this->id][i]);
        currentArmPosition = 90.0;
        currentServoTwoPosition = 90.0;
        currentGrabPosition = 90.0;
        nextMoveMills = 0;
        mass = 0.0;
    }

    void init(int id){
        this->id = id;
        for (int i = 0; i < eachArmServo_n; ++i) {
//            pinMode(constPin[this->id][i], OUTPUT);
            servo[i].attach(constPin[this->id][i]);
        }
        reset();
    }

    bool reachArm(float toArmPosition) {//toArmPosition:90~30(90:the initial position,30:the furthest postion
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
            this->servo[i].write((armReachIndex[this->id][i] + weightIndex[this->id][i] * mass) * this->currentArmPosition + armReachIntercept[this->id][i]);
        return false;
    }

    bool rotateServoTwo(float toServoTwoPosition){
        if (abs(toServoTwoPosition - this->currentServoTwoPosition) < servoTwoSpeed + 0.02)
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

    bool rotateServoOne(float toServoOnePosition){
        toServoOnePosition = min((float)120.0, toServoOnePosition);
        toServoOnePosition = max((float)85.0, toServoOnePosition);

        if (abs(toServoOnePosition - this->currentGrabPosition) < grabSpeed + 0.02){
//            Serial.println("rotateServoOne->true");
//            Serial.println(toServoOnePosition);
//            Serial.println(this->currentGrabPosition);
//            Serial.println("finish");
            return true;
        }

        if (millis() < this->nextMoveMills)
            return false;

        this->nextMoveMills = millis() + servoMoveMills;
        if (this->currentGrabPosition > toServoOnePosition)
            this->currentGrabPosition -= grabSpeed;
        else
            this->currentGrabPosition += grabSpeed;
        servo[0].write(this->currentGrabPosition);
        return false;
    }

    void loopRun(){
        if (this->timeQueue.empty())
            return;

        if (signal >= this->timeQueue.frontPoint()->startSignal)
            switch(this->timeQueue.frontPoint()->id){
                case timeUnitBeingDelay:
                    if (millis() >= this->timeQueue.frontPoint()->index) {
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

                case timeUnitRotateServoOne:
                    if (this->rotateServoOne(this->timeQueue.frontPoint()->index)){
                        signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                        this->timeQueue.pop();
                    }
                    break;

                case timeUnitChangeMass:
                    this->mass  = this->timeQueue.frontPoint()->index;
                    signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                    this->timeQueue.pop();
                    break;
        }
    }
}arm[arm_n];

struct _camera{
    _timeQueue timeQueue;
    Servo servo[2];
    void reset(){
        servo[cameraUpDown].write(initialCameraPos[cameraUpDown]);
        servo[cameraLeftRight].write(initialCameraPos[cameraLeftRight]);
    }

    void init(){
        servo[cameraUpDown].attach(cameraPin[cameraUpDown]);
        servo[cameraLeftRight].attach(cameraPin[cameraLeftRight]);
        reset();
    }

    bool moveCamera(int direction, double angle){
        servo[direction].write(angle);
        return true;
    }

    void loopRun(){
        if (this->timeQueue.empty())
            return;

        if (signal >= this->timeQueue.frontPoint()->startSignal)
            switch(this->timeQueue.frontPoint()->id){
                case timeUnitBeingDelay:
                    if (millis() >= this->timeQueue.frontPoint()->index) {
                        signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                        this->timeQueue.pop();
                    }
                    break;
                case timeUnitDelay:
                    this->timeQueue.frontPoint()->id = timeUnitBeingDelay;
                    this->timeQueue.frontPoint()->index = millis() + (*this->timeQueue.frontPoint()).index;
                    break;

                case timeUnitMoveCameraUpDown:
                    if (this->moveCamera(cameraUpDown, this->timeQueue.frontPoint()->index)) {
                        signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                        this->timeQueue.pop();
                    }
                    break;

                case timeUnitMoveCameraLeftRight:
                    if (this->moveCamera(cameraLeftRight, this->timeQueue.frontPoint()->index)) {
                        signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                        this->timeQueue.pop();
                    }
                    break;
            }
    }
}camera;

float deltaArmReachIndex(float mass){
    return mass;
}

void moveArm(int armNum, float position, int startSignal = -1, int finishSignal = -1) {//armNum: leftArm/rightArm, Position:90~30(90:the initial position,30:the furthest postion
    arm[armNum].timeQueue.push(timeUnitReachArm, position, startSignal, finishSignal);
}

void delayArm(int armNum, float delay, int startSignal = -1, int finishSignal = -1){//delay单位:秒
    arm[armNum].timeQueue.push(timeUnitDelay, delay * 1e3, startSignal, finishSignal);
}

void rotateServoTwo(int armNum, float position, int startSignal = -1, int finishSignal = -1){//armNum: leftArm/rightArm,
    arm[armNum].timeQueue.push(timeUnitRotateServoTwo, position, startSignal, finishSignal);
}

void rotateServoOne(int armNum, float position, int startSignal = -1, int finishSignal = -1){//armNum: leftArm/rightArm,
    arm[armNum].timeQueue.push(timeUnitRotateServoOne, position, startSignal, finishSignal);
}

void changeMass(int armNum, float mass, int startSignal = -1, int finishSignal = -1){
    arm[armNum].timeQueue.push(timeUnitChangeMass, mass / (float)100, startSignal, finishSignal);
}

void delayCamera(double delay, int startSignal = -1, int finishSignal = -1){
    camera.timeQueue.push(timeUnitDelay, delay * 1e3, startSignal, finishSignal);
}

void moveCamera(int direction, float angle, int startSignal = -1, int finishSignal = -1){
    camera.timeQueue.push(direction + 1, angle, startSignal, finishSignal);
}

#endif

