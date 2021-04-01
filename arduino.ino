//#define ClionArduino//在Arduino IDE环境下，注释掉一行
#include <Arduino.h>
#include "lib/stepperMotor.hpp"
#define global_t_step ((double)(8e-6) * (double)4.3)
double global_t = 0;

void setup(){
    Serial.begin(9600);
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].init(i);
    moveBaseTo(15.0);
    delayMoveBase(3.0);
    moveBaseTo(0.0);
//    liftBaseTo(liftBaseRight, -15.0);
//    delayLiftBase(liftBaseRight, 3.0);
//    liftBaseTo(liftBaseRight , 0);
//    rotateRotationBaseTo(rotationBaseRight, -90);
//    delayRotationBase(rotationBaseRight, 3.0);
//    rotateRotationBaseTo(rotationBaseRight, 0);
}


void loop(){
    global_t += global_t_step;
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].loop_run();
//    stepperMotor[0].rotate(3600);
//    stepperMotor[0].rotate(36);
//    stepperMotor[4].rotate(6000);
//    stepperMotor[5].rotate(6000);
}