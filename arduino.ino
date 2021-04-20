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
   initSPI();
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].init(i);
    for (int i = 0; i < arm_n; ++i)
        arm[i].init(i);

    moveBaseTo(5.0);
    moveBaseTo(0.0);

//    delayArm(1, 5.0);
//    rotateServoOne(1, 1 10);
//    delayArm(1, 3.0);
//    rotateServoOne(1, 90);
    
      Serial.println("finish setup");
}

ISR(SPI_STC_vect) {
    processOneByte();
}

void loop(){
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].loopRun();
    for (int i = 0; i < arm_n; ++i)
        arm[i].loopRun();
}
