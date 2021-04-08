//#define ClionArduino//在Arduino IDE环境下，注释掉一行
#include <Arduino.h>
#include "lib/stepperMotor.hpp"
#include "lib/servo.hpp"
#include "lib/action.hpp"

void setup(){
    Serial.begin(9600);
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].init(i);
    for (int i = 0; i < arm_n; ++i)
        arm[i].init(i);
    
    shake(rightArm, 8);
    
//    rightArmGrab();
//    moveArm(rightArm, 75.0);
//    moveArm(rightArm, 90.0);
//    setBack();


//    moveBaseTo(-5.0, -1, 2);
//    delayMoveBase(3.0);
//    moveBaseTo(0.0, 2, -1);

//    liftBaseTo(liftBaseLeft, -15.0);
//    delayLiftBase(liftBaseLeft, 3.0);
//    liftBaseTo(liftBaseLeft, 0);

//    rotateRotationBaseTo(rotationBaseLeft, 180, -1, 2);
//    delayRotationBase(rotationBaseLeft, 3.0);
//    rotateRotationBaseTo(rotationBaseLeft, 0);

//    moveArm(rightArm, 75.0, -1, 3);
//    delayArm(rightArm, 3.0, 3, -1);
//    moveArm(rightArm, 90.0);

    Serial.println("finish setup");
}

void loop(){
    if (millis() % 1000 == 0)
        Serial.println(signal);
//    arm[leftArm].armReach(90.0);
//    test.write(1080);
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].loopRun();
    for (int i = 0; i < arm_n; ++i)
        arm[i].loopRun();
//    stepperMotor[0].rotate(3600);
//    stepperMotor[0].rotate(36);
//    stepperMotor[4].rotate(6000);
//    stepperMotor[5].rotate(6000);
}
