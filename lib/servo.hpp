//
// Created by ethepherin on 2021/4/1.
//
#define ClionArduino

#ifndef my_servo
#define my_servo

#ifdef ClionArduino
#include <Servo/src/Servo.h>
#else
#include <Servo.h>
#endif

#include "Arduino.h"
#define arm_n 2
#define eachArmServo_n 5
//                           L1,  L2,  L3,  L4,  L5,    R1,  R2,  R3,  R4,  R5
const int constPin[arm_n][eachArmServo_n] =   {{9,   8,   7,   10,  11},  {6,   5,   4,   2,   3}};
const int initialPos[arm_n][eachArmServo_n] = {{90,  90,  95,  90,  90},  {90,  90,  85,  90,  90}};

//struct _arm{
//    Servo servo[eachArmServo_n];
//}arm[arm_n];

#endif