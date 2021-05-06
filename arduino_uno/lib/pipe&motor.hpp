#ifndef my_pipeAndMotor
#define my_pipeAndMotor

#include "timeQueue.hpp"
//                                    0      1      2      3
const float pipeOpenPosition[4]    = {120,   130,   130,   150};
const float pipeClosePosition[4]   = {0,     0,     0,     0};
const int constPin[4]              = {3,     5,     6,     9};
const int motorPin = 2;
#define timeDivideVolume (float)1.0
#define timeDivideGram (float)1.0
#define solidAnalogSpeed (int)175

#define solidOnTime 3000
#define solidOffTime 1000

struct _pipe{
    int id;
    bool ifWait;
    unsigned long waitTime;
    _timeQueue timeQueue;
    Servo servo;
    _pipe() = default;

    void reset(){
        ifWait = false;
        waitTime = 0.0;
        servo.write(pipeClosePosition[this->id]);
    }

    void init(int id){
        this->id = id;
        servo.attach(constPin[this->id]);
        reset();
    }

    bool getLiquid(float time){
        if (!ifWait){
            ifWait = true;
            waitTime = millis() + (unsigned long)(time * 1000);
            servo.write(pipeOpenPosition[this->id]);
            return false;
        }
        else{
            if (millis() < waitTime)
                return false;
            ifWait = false;
            servo.write(pipeClosePosition[this->id]);
            return true;
        }
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

                case timeUnitGetLiquid:
                    if (this->getLiquid(this->timeQueue.frontPoint()->index)) {
                        signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                        this->timeQueue.pop();
                    }
                    break;
            }
    }

}pipe[4];

struct _motor{
    bool ifWait, changeType;
    unsigned long waitTime, nextChangeTime;
    _timeQueue timeQueue;
    _motor() = default;

    void reset(){
        changeType = false;
        nextChangeTime = 0;
        ifWait = false;
        waitTime = 0.0;
        analogWrite(motorPin, 0);
    }

    void init(){
        pinMode(motorPin, OUTPUT);
        reset();
    }

    bool getSolid(float time){
        if (!ifWait){
            ifWait = true;
            waitTime = millis() + (unsigned long)(time * 1000);
            return false;
        }
        else{
            if (millis() < waitTime) {
                if (micros() > nextChangeTime) {
                    changeType = !changeType;
                    if (changeType) {
                        nextChangeTime = micros() + solidOnTime;
                        analogWrite(motorPin, 255);
                    }
                    else {
                        nextChangeTime = micros() + solidOffTime;
                        analogWrite(motorPin, 0);
                    }
                }
                return false;
            }
            ifWait = false;
            analogWrite(motorPin, 0);
            return true;
        }
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

                case timeUnitGetSolid:
                    if (this->getSolid(this->timeQueue.frontPoint()->index)) {
                        signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                        this->timeQueue.pop();
                    }
                    break;
            }
    }

}motor;

void getLiquid(int liquidNum, float volume, int startSignal = -1, int finishSignal = -1) {//armNum: leftArm/rightArm, Position:90~30(90:the initial position,30:the furthest postion
    pipe[liquidNum].timeQueue.push(timeUnitGetLiquid, volume * timeDivideVolume, startSignal, finishSignal);
}

void delayLiquid(int liquidNum, float delay, int startSignal = -1, int finishSignal = -1){//delay单位:秒
    pipe[liquidNum].timeQueue.push(timeUnitDelay, delay * 1e3, startSignal, finishSignal);
}
void getSolid(float gram, int startSignal = -1, int finishSignal = -1) {//armNum: leftArm/rightArm, Position:90~30(90:the initial position,30:the furthest postion
    motor.timeQueue.push(timeUnitGetSolid, gram * timeDivideGram, startSignal, finishSignal);
}

void delaySolid(float delay, int startSignal = -1, int finishSignal = -1){//delay单位:秒
    motor.timeQueue.push(timeUnitDelay, delay * 1e3, startSignal, finishSignal);
}
#endif