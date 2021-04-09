#ifndef my_action
#define my_action

#include "servo.hpp"
#include "stepperMotor.hpp"


void setBack(){
    moveArm(leftArm, 90.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    moveArm(rightArm, 90.0, lastAppliedSignal, lastAppliedSignal + 1);
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
//    liftBaseTo(liftBaseRight, -15.0, -1, 1);
//    rotateRotationBaseTo(rotationBaseRight, -180.0, 1, 2);
//    moveArm(rightArm, 75.0, lastAppliedSignal, -1);
//    delayArm(rightArm, 3.0);
//    moveArm(rightArm, 90.0, lastAppliedSignal, ++lastAppliedSignal);
//    rotateRotationBaseTo(rotationBaseRight, 0.0, lastAppliedSignal, ++lastAppliedSignal);

    lastAppliedSignal += 1;
    liftBaseTo(liftBaseRight, -5.0, -1, lastAppliedSignal);
    rotateRotationBaseTo(rotationBaseRight, -180.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    moveArm(rightArm, 80.0, lastAppliedSignal, -1);
    delayArm(rightArm, 3.0);
    moveArm(rightArm, 90.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    rotateRotationBaseTo(rotationBaseRight, 0.0, lastAppliedSignal, lastAppliedSignal + 1);
    lastAppliedSignal += 1;
    liftBaseTo(liftBaseRight, -5.0, lastAppliedSignal, lastAppliedSignal);

}


void shake(int armNum, int times){
    for (int i = 0; i < times; ++i) {
        rotateServoTwo(armNum, 110);
        rotateServoTwo(armNum, 70);
    }
    rotateServoTwo(armNum, 90);
}

#endif