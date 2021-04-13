//#define ClionArduino//在Arduino IDE环境下，注释掉一行
#include <Arduino.h>
#include "lib/stepperMotor.hpp"
#include "lib/servo.hpp"
#include "lib/action.hpp"
#include "lib/spiRead.hpp"

void setup(){
    Serial.begin(9600);
    Serial.println("Setupping");
//    Serial.println(stringHash(String("cgMs")));
    pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);
    SPCR |= _BV(SPIE);
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].init(i);
    for (int i = 0; i < arm_n; ++i)
        arm[i].init(i);
//    delayArm(armRight, 5, 0, 1);

//        rotateServoOne(armLeft, 90);
//        shake(armLeft, 8);
//
//        rightArmGrab();
//
//      rotateRotationBaseTo(rotationBaseLeft, 180);
//        rotateServoOne(armLeft, 110);
//        rotateServoOne(armLeft, 90);
//        rotateServoOne(armLeft, 110);
//        rotateServoOne(armLeft, 90);
//        rotateServoOne(armLeft, 110);
//        rotateServoOne(armLeft, 90);
//
//    liftBaseTo(liftBaseRight, -0.3, 0, 0); 

//    rotateServoOne(armRight, 105, 1, 2);
//    moveArm(armRight, 83.5, 2, 3);
//    rotateServoOne(armRight, 87, 3, 4);
//    liftBaseTo(liftBaseRight, -3.0, 4, 5);
//    changeMass(armRight, 0.018, 5, 6);
//    moveArm(armRight, 90, 6, 7);
//    delayArm(armLeft, 3.0, 7, 8);
//    moveArm(armRight, 83.5, 8, 9);
//    liftBaseTo(liftBaseRight, 0.0, 9, 10);
//    rotateServoOne(armRight, 110, 10, 11);
//    changeMass(armRight, 0.0, 11, 12);
//    moveArm(armRight, 90, 11, 12);
//
//    rotateServoOne(armLeft, 110, 0, 1);
//    moveArm(armLeft, 83.2, 1, 2);
//    rotateServoOne(armLeft, 90, 2, 3);
//    liftBaseTo(liftBaseLeft, -5.5, 3, 4);
//    delayArm(armLeft, 3.0, 4, 5);
//    moveArm(armLeft, 90.0, 5, 6);
//    liftBaseTo(liftBaseLeft, 0.0, 6, 7);
//
//    liftBaseTo(liftBaseLeft, -5.5, 7, 11);
//    moveArm(armLeft, 81.2, 11, 12);
//    liftBaseTo(liftBaseLeft, 0.0, 12, 13);
//    delayArm(armLeft, 5.0, 12, 13);
//    rotateServoOne(armLeft, 110, 13, 14);
//    delayArm(armLeft, 5.0, 14, 15);
//    moveArm(armLeft, 90.0, 15, 16);


    //    setBack();


    //    moveBaseTo(-5.0, -1, 2);
    //    delayMoveBase(3.0);
    //    moveBaseTo(0.0, 2, -1);

//        liftBaseTo(liftBaseRight, -15.0);
//        delayLiftBase(liftBaseRight, 3.0);
//        liftBaseTo(liftBaseRight, 0);

    //    rotateRotationBaseTo(rotationBaseLeft, -180.0, -1, 2);
    //    delayRotationBase(rotationBaseLeft, 3.0);
    //    rotateRotationBaseTo(rotationBaseLeft, 0);

//        moveArm(rightArm, 75.0, 1, 2);
//        changeMass(rightArm, 0.03, 2, 3);
//        delayArm(rightArm, 3.0, 3, 4);
//        moveArm(rightArm, 90.0, 4, 5);
//    processCmd(String("rtRtBs#1#5.2#"));
    
//    Serial.println(stringToDouble(String("#11.235#"), 0));
      Serial.println("finish setup");
}

void loop(){
    processSPI();
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].loopRun();
    for (int i = 0; i < arm_n; ++i)
        arm[i].loopRun();
}
