#include <Servo.h>
#include <Arduino.h>
#include "lib\spiRead.hpp"
#include "lib\timeQueue.hpp"
#include "lib\pipe&motor.hpp"

void setup() {
    Serial.begin(9600);
    Serial.println("setupping");
    pinMode(piCommunicationPin, OUTPUT);
    digitalWrite(piCommunicationPin, LOW);
    initSPI();
    for (int i = 0; i < 4; ++i)
        pipe[i].init(i);
    motor.init();
    if (latePrint) {
        if (print_n == 0)
            print_time = millis() + 3000;
        print[print_n] = "Late print ready\n";
        print_n++;
    }
//    getSolid(10);

    Serial.println("finish setup");
}

ISR(SPI_STC_vect) {
    processOneByte();
}

void loop() {
    for (int i = 0; i < 4; ++i)
        pipe[i].loopRun();
    motor.loopRun();
    if (latePrint && print_n != 0 && print_time < millis()){
        int temp_n = print_n;
        print_n = 0;
        for (int i = 0; i < temp_n; ++i)
            Serial.print(print[i]);
    }
    if (spiSendingFlag && spiStartTime + 1000 < micros()){
        if (latePrint){
            if (print_n == 0)
                print_time = millis() + 3000;
//            Serial.println("Lose byte");
            print[print_n] = "Lose byte asking for resending\n";
            print_n++;
        }
        spiResendFlag = true;
        spiSendingFlag = false;
        digitalWrite(piCommunicationPin, HIGH);
    }
}
  
