#ifndef myAction
#define myAction

#include "servo.hpp"
#include "stepperMotor.hpp"


void setBack(){
    moveArm(armLeft, 90.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    moveArm(armRight, 90.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    moveBaseTo(0.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    rotateRotationBaseTo(rotationBaseLeft, 0.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    rotateRotationBaseTo(rotationBaseRight, 0.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    liftBaseTo(liftBaseLeft, 0.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    liftBaseTo(liftBaseRight, 0.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;

}

void rightArmGrab(){
//    rotateServoOne(rightArm, 105, 1, 2);
//    moveArm(rightArm, 83.5, 2, 3);
//    rotateServoOne(rightArm, 87, 3, 4);
//    liftBaseTo(liftBaseRight, -3.0, 4, 5);
//    changeMass(rightArm, 0.013, 5, 6);
//    moveArm(rightArm, 90, 6, 7);
//    delayArm(armLeft, 3.0, 7, 8);
//    moveArm(rightArm, 83.5, 8, 9);
//    liftBaseTo(liftBaseRight, 0.0, 9, 10);
//    rotateServoOne(rightArm, 110, 10, 11);
//    changeMass(rightArm, 0.0, 11, 12);
//    moveArm(rightArm, 90, 11, 12);
}


void shake(int armNum, int times){
    for (int i = 0; i < times; ++i) {
        rotateServoTwo(armNum, 110);
        rotateServoTwo(armNum, 70);
    }
    rotateServoTwo(armNum, 90);
}

#endif