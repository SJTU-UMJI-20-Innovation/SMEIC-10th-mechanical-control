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


    //    rotateServoOne(leftArm, 90);
    //    shake(leftArm, 8);

    //    rightArmGrab();

    //  rotateRotationBaseTo(rotationBaseLeft, 180);
//        rotateServoOne(leftArm, 110);
//        rotateServoOne(leftArm, 90);
//        rotateServoOne(leftArm, 110);
//        rotateServoOne(leftArm, 90);
//        rotateServoOne(leftArm, 110);
//        rotateServoOne(leftArm, 90);

/*
    rotateServoOne(leftArm, 110, 0, 1);
    moveArm(leftArm, 81.2, 1, 2);
    rotateServoOne(leftArm, 90, 2, 3);
    liftBaseTo(liftBaseLeft, -5.5, 3, 4);
    delayArm(leftArm, 3.0, 4, 5);
    moveArm(leftArm, 90.0, 5, 6);
    liftBaseTo(liftBaseLeft, 0.0, 6, 7);
    
    liftBaseTo(liftBaseLeft, -5.5, 7, 11);
    moveArm(leftArm, 81.2, 11, 12);
    liftBaseTo(liftBaseLeft, 0.0, 12, 13);
    delayArm(leftArm, 5.0, 12, 13);
    rotateServoOne(leftArm, 110, 13, 14);
    delayArm(leftArm, 5.0, 14, 15);
    moveArm(leftArm, 90.0, 15, 16);
*/

    //    setBack();


    //    moveBaseTo(-5.0, -1, 2);
    //    delayMoveBase(3.0);
    //    moveBaseTo(0.0, 2, -1);

    //    liftBaseTo(liftBaseLeft, -15.0);
    //    delayLiftBase(liftBaseLeft, 3.0);
    //    liftBaseTo(liftBaseLeft, 0);

    //    rotateRotationBaseTo(rotationBaseLeft, -180.0, -1, 2);
    //    delayRotationBase(rotationBaseLeft, 3.0);
    //    rotateRotationBaseTo(rotationBaseLeft, 0);

    //    moveArm(rightArm, 75.0, -1, 3);
    //    delayArm(rightArm, 3.0, 3, -1);
    //    moveArm(rightArm, 90.0);

    Serial.println("finish setup");
}

void loop(){
//    if (millis() % 200 == 0) {
//        Serial.println(arm[leftArm].currentGrabPosition);
//        Serial.println(signal);
//    }
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
