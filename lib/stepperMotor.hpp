//
// Created by ethepherein on 2021/4/1.
//
#ifndef stepMotor
#define stepMotor

#include <Arduino.h>
#include "timeQueue.hpp"
#define stepperMotor_n 6

#define rotationBasePlusNum 2
#define rotationBaseRatio ((float)20.8)

#define liftBasePlusNum 0
#define liftBaseRatio ((float)350.0)

#define moveBaseFront 4
#define moveBaseBack 5
#define moveBaseRatio ((float)350.0)

//                          l_L, l_R, r_L, r_R, h_1, h_2
//rotate 小齿轮12， 大齿轮64
const int constPul[] =     {28,  44,  33,  47,  34,  36};
const int constDir[] =     {14,  17,  20,  23,  41,  45};
//const int intervalTime[] = {250, 250, 240, 240, 150, 150};
const int intervalTime[] = {250, 250, 240, 240, 180, 180};

struct _stepperMotor{
    int id, pul, dir, current_dir;//current_dir: 0->无方向, counter: 计数器
    unsigned long int time;
    unsigned long long counter;
    float current_position;//current_position继承了angle与position,命名为position
    _timeQueue timeQueue;

    _stepperMotor() = default;
    void init(int id){
        this->id = id;
        pinMode(constPul[this->id], OUTPUT);
        pinMode(constDir[this->id], OUTPUT);
        this->counter = 1;
        this->pul = constPul[this->id];
        this->dir = constDir[this->id];
        this->current_dir = 0;
        this->time = micros();
        this->current_position = 0.0;
        digitalWrite(this->pul, 0);
        digitalWrite(this->dir, 0);
    }

    bool rotate(float to_angle){//正：逆时针，负：顺时针
        if (abs(to_angle - this->current_position) < 1.0){
            digitalWrite(this->pul, 0);
            digitalWrite(this->dir, 0);
            this->counter = 1;
            this->current_dir = 0;
            return true;
        }
        int to_dir = to_angle - this->current_position > 0 ? 1 : -1;
        if (this->current_dir != to_dir && this->current_dir != 0){
//            Serial.println("!!!");
            digitalWrite(this->pul, 0);
            if((micros() - this->time) > 1000000){
                this->current_dir = to_dir;
                this->time = micros();
            }
        }
        else{
            digitalWrite(this->dir, (to_dir + 1) / 2);//to_dir = 1, val = 1; to_dir = -1, val = 0
            this->current_dir = to_dir;
            if((micros() - this->time) >= intervalTime[this->id]){
                digitalWrite(pul, this->counter%2);
                this->current_position += to_dir * (this->counter%2 == 1)*0.225;
                this->counter += 1;
                this->time = micros();
            }
        }
        return false;
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

            case timeUnitStepperMotorMove:
                if (this->rotate(this->timeQueue.frontPoint()->index)) {
                    signal = max(signal, this->timeQueue.frontPoint()->finishSignal);
                    this->timeQueue.pop();
//                    Serial.println("Update to ");
//                    Serial.println(signal);
                }
                break;

        }
    }

}stepperMotor[stepperMotor_n];

//void rotateRotationBase(int rotate_num, float angle){//rotate_num: 0(rotationBaseLeft)->左底座 (rotationBaseRight)->右底座
//    if (rotate_num == 0)
//        stepperMotor[2].rotate(angle * rotationBaseRatio);
//    else
//        stepperMotor[3].rotate(angle * rotationBaseRatio);
//}

void delayRotationBase(int armNum, float delay, int startSignal = -1, int finishSignal = -1){//delay单位:秒 armNum: armLeft->左底座 armRight->右底座
    stepperMotor[armNum + rotationBasePlusNum].timeQueue.push(timeUnitDelay, delay * 1e3, startSignal, finishSignal);
}

void rotateRotationBaseTo(int armNum, float angle, int startSignal = -1, int finishSignal = -1){//angle单位:度 armNum: armLeft->左底座 armRight->右底座
    stepperMotor[armNum + rotationBasePlusNum].timeQueue.push(timeUnitStepperMotorMove, angle * rotationBaseRatio, startSignal, finishSignal);
}

void delayLiftBase(int armNum, float delay, int startSignal = -1, int finishSignal = -1){//delay单位:秒 armNum: armLeft->左底座 armRight->右底座
    stepperMotor[armNum + liftBasePlusNum].timeQueue.push(timeUnitDelay, delay * 1e3, startSignal, finishSignal);
}

void liftBaseTo(int armNum, float height, int startSignal = -1, int finishSignal = -1){//height单位:厘米, armNum: armLeft->左底座 armRight->右底座
    stepperMotor[armNum + liftBasePlusNum].timeQueue.push(timeUnitStepperMotorMove, height * liftBaseRatio, startSignal, finishSignal);
}

void delayMoveBase(float delay, int startSignal = -1, int finishSignal = -1){//delay单位:秒
    stepperMotor[moveBaseFront].timeQueue.push(timeUnitDelay, delay * 1e3, startSignal, finishSignal);
    stepperMotor[moveBaseBack].timeQueue.push(timeUnitDelay, delay * 1e3, startSignal, finishSignal);
}

void moveBaseTo(float position, int startSignal = -1, int finishSignal = -1){//position单位:厘米
    Serial.println("In moveBaseTo");
    stepperMotor[moveBaseFront].timeQueue.push(timeUnitStepperMotorMove, position * moveBaseRatio, startSignal, finishSignal);
    stepperMotor[moveBaseBack].timeQueue.push(timeUnitStepperMotorMove, position * moveBaseRatio, startSignal, finishSignal);
}


#endif