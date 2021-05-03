//#define ClionArduino//在Arduino IDE环境下，注释掉一行
#include <Arduino.h>
#include "lib/stepperMotor.hpp"
#include "lib/servo.hpp"
#include "lib/action.hpp"
#include "lib/spiRead.hpp"


unsigned long t = 0;
void setup(){
    Serial.begin(9600);
    Serial.println("Setupping");
//    Serial.println(stringHash(String("cgMs")));
    initSPI();
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].init(i);
    for (int i = 0; i < arm_n; ++i)
        arm[i].init(i);
//    delayArm(0, 5.0);
//    moveArm(0, 85);
//    delayArm(0, 5.0);
//    moveArm(0, 80);
//    delayArm(0, 5.0);
//    moveArm(0, 75);
//    delayArm(0, 5.0);
//    moveArm(0, 70);
//    moveBaseTo(5.0);
//    moveBaseTo(0.0);

//    delayArm(1, 5.0);
//    rotateServoOne(1, 1 10);
//    delayArm(1, 3.0);
//    rotateServoOne(1, 90);
    digitalWrite(piCommunicationPin, LOW);
    Serial.println("finish setup");
    if (latePrint) {
        if (print_n == 0)
            print_time = millis() + 3000;
        print[print_n] = "Late print ready\n";
        print_n++;
    }
}

ISR(SPI_STC_vect) {
    processOneByte();
}

void loop(){
//    if (millis() % 5000 == 0 && t - millis() > 4000){
//        t = millis();
//        Serial.println("Still looping");
//    }
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].loopRun();
    for (int i = 0; i < arm_n; ++i)
        arm[i].loopRun();
    if (spiSendingFlag && spiStartTime + 1000 < micros()){
        if (latePrint){
            if (print_n == 0)
                print_time = millis() + 3000;
            print[print_n] = "Lose byte asking for resending\n";
            print_n++;
        }
        spiResendFlag = true;
        spiSendingFlag = false;
        digitalWrite(piCommunicationPin, HIGH);
    }
    if (latePrint && print_n != 0 && print_time < millis()){
        int temp_n = print_n;
        print_n = 0;
        for (int i = 0; i < temp_n; ++i)
            Serial.print(print[i]);
    }
}
