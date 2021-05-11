//#define ClionArduino//在Arduino IDE环境下，注释掉一行
#include <Arduino.h>
#include "lib/stepperMotor.hpp"
#include "lib/servo.hpp"
#include "lib/action.hpp"
#include "lib/spiRead.hpp"
//#include <Servo_Hardware_PWM.h>

//const int servoPin = 45;
//unsigned long servoHighMicros = 1000;
//unsigned long servoLowMicros = 19000;
//double currentDirection = 0.0;
//bool servoFlag = false;
//unsigned long servoChangeMicros = 0;

unsigned long t = 0;

//void changeServo(double direction){
//    servoHighMicros = (unsigned long)(1000 + direction / 180.0 * 1000.0);
//    servoLowMicros = (unsigned long)(20000 - servoHighMicros);
//}


void setup(){
    t = millis();
    Serial.begin(9600);
    Serial.println("Setupping");
//    servo.attach(servoPin);
//    pinMode(servoPin, OUTPUT);
//    changeServo(0.0);
//    Serial.println(stringHash(String("cgMs")));
    initSPI();
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].init(i);
    for (int i = 0; i < arm_n; ++i)
        arm[i].init(i);
    camera.init();
    liquid.init();
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
//    delayCamera(2);
//    moveCamera(1, 110);
}

ISR(SPI_STC_vect) {
        processOneByte();
}

void loop(){
//    servo.write(180);
//    if (millis() - t >= 2000) {
//        t = millis();
//        currentDirection = ((unsigned long)(currentDirection + 15)) % 180;
//        changeServo(currentDirection);
//    }
//        if (servoChangeMicros < micros()) {
//        Serial.println("change");
//        Serial.println(servoFlag);
//            if (servoFlag) {
//                digitalWrite(servoPin, HIGH);
//                servoChangeMicros = micros() + servoHighMicros;
//            } else {
//                digitalWrite(servoPin, LOW);
//                servoChangeMicros = micros() + servoLowMicros;
//            }
//            servoFlag = !servoFlag;
//        }
    for (int i = 0; i < stepperMotor_n; ++i)
        stepperMotor[i].loopRun();
    for (int i = 0; i < arm_n; ++i)
        arm[i].loopRun();
    camera.loopRun();
    liquid.loopRun();
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
//        Serial.println("Lose byte want to resend");
//        Serial.println(remainingBytes);
//        Serial.println(bufferPos);
    }
    if (latePrint && print_n != 0 && print_time < millis()){
        int temp_n = print_n;
        print_n = 0;
        for (int i = 0; i < temp_n; ++i)
            Serial.print(print[i]);
    }
}
