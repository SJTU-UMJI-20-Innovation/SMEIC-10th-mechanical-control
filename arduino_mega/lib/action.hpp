#ifndef myAction
#define myAction

#include "timeQueue.hpp"
#include "servo.hpp"
#include "stepperMotor.hpp"

void stop(){
    if (debugMode)
        Serial.println("Stopping all orders");
    for (int i = 0; i < stepperMotor_n; ++i)
        while (!stepperMotor[i].timeQueue.empty())
            stepperMotor[i].timeQueue.pop();
    for (int i = 0; i < arm_n; ++i)
        while (!arm[i].timeQueue.empty())
            arm[i].timeQueue.pop();
}

void changeSignal(int changeToSignal){
    signal = changeToSignal;
}

#endif