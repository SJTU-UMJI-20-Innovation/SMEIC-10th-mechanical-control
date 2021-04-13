#ifndef mySpiRead
#define mySpiRead

#include "action.hpp"
#include "timeQueue.hpp"
#include "servo.hpp"
#include "stepperMotor.hpp"

#define bufferStorageSize 20
#define BUFFER_SIZE 32

#define debugMode true

volatile byte buf[BUFFER_SIZE];
char msg[BUFFER_SIZE];
volatile byte bufPos = 0;
volatile bool readFinished = false;

bool ifBufferOn = false;
int  bufferStorageNum = 0;
String bufferStorage[bufferStorageSize];

int readIndexInt[5];
double readIndexDouble[3];

#define readAbnormal (-1)
#define readNoSignal 0
#define readWithSignal 1

#define stringToIntAbnormal (-32768)
#define stringToDoubleAbnormal (double)(1.23456)

ISR(SPI_STC_vect) {
    byte b = SPDR;
    if (b == '\0') {
        readFinished = true;
        bufPos = 0;
    } else {
        buf[bufPos] = b;
        bufPos = (bufPos + 1) % BUFFER_SIZE;
    }
}

int stringToInt(String temp, int* p){// stringToIntAbnormal: abnormal
    int index = 1;
    if (*p > temp.length() - 1){
        Serial.println("Error: stringToInt reads over range");
        return stringToIntAbnormal;
    }
    if (temp[*p] == '#'){
        Serial.println("Error: stringToInt reads # the first place");
        return stringToIntAbnormal;
    }
    if (temp[*p] == '-') {
        index = -1;
        (*p)++;
    }
    int sum = 0;
    for (int i = *p; i < temp.length(); ++i) {
        if (temp[i] == '#') {
            *p = i;
            return index * sum;
        }
        if (temp[i] < '0' || temp [i] > '9'){
            Serial.println("Error: stringToInt reads " + temp[i]);
        }
        sum = sum * 10 + temp[i] - '0';
    }
    Serial.println("Error: stringToInt string doesn't ends");
    return stringToIntAbnormal;
}

double stringToDouble(String temp, int* p){//stringToDoubleAbnormal: abnormal
    double index = 1.0;
    if (*p > temp.length() - 1){
        Serial.println("Error: stringToInt reads over range");
        return stringToDoubleAbnormal;
    }
    if (temp[*p] == '#'){
        Serial.println("Error: stringToDouble reads # the first place");
        return stringToDoubleAbnormal;
    }
    if (temp[*p] == '-') {
        index = -1.0;
        (*p)++;
    }
    double sum = 0;
    bool ifAfterPoint = false;
    double afterPointNum = 0.1;
    for (int i = *p; i < temp.length(); ++i) {
        if (temp[i] == '#') {
            *p = i;
            return index * sum;
        }
        if (temp[i] == '.')
            if (ifAfterPoint){
                Serial.println("Error: stringToDouble reads two .");
                return stringToDoubleAbnormal;
            }
            else {
                ifAfterPoint = true;
                continue;
            }
        if (temp[i] < '0' || temp [i] > '9'){
            Serial.println("Error: stringToDouble reads " + temp[i]);
            return stringToDoubleAbnormal;
        }
        else
            if (ifAfterPoint) {
                sum = sum + (double)(temp[i] - '0') * afterPointNum;
                afterPointNum *= (double)0.1;
            }
            else
                sum = sum * 10 + temp[i] - '0';
    }
}

int readIntDoubleIntInt(String temp, int p){//: -1: abnormal, 0:
    int tempInt;
    double tempDouble;
    p++;
    tempInt = stringToInt(temp, &p);
    if (tempInt == stringToIntAbnormal)
        return readAbnormal;
    readIndexInt[0] = tempInt;

    p++;
    tempDouble = stringToDouble(temp, &p);
    if (tempDouble == stringToDoubleAbnormal)
        return readAbnormal;
    readIndexDouble[0] = tempDouble;

    if (p == temp.length() - 1) {
        readIndexInt[1] = -1;
        readIndexInt[2] = -1;
        return readNoSignal;
    }

    p++;
    tempInt = stringToInt(temp, &p);
    if (tempInt == stringToIntAbnormal)
        return readAbnormal;
    readIndexInt[1] = tempInt;

    p++;
    tempInt = stringToInt(temp, &p);
    if (tempInt == stringToIntAbnormal)
        return readAbnormal;
    readIndexInt[2] = tempInt;
    return readWithSignal;
}

void printIntDoubleIntInt(){
    Serial.print(readIndexInt[0]);
    Serial.print(' ');
    Serial.print(readIndexDouble[0]);
    Serial.print(' ');
    Serial.print(readIndexInt[1]);
    Serial.print(' ');
    Serial.println(readIndexInt[2]);
}

int readDoubleIntInt(String temp, int p){//: -1: abnormal, 0:
    int tempInt;
    double tempDouble;

    p++;
    tempDouble = stringToDouble(temp, &p);
    if (tempDouble == stringToDoubleAbnormal)
        return readAbnormal;
    readIndexDouble[0] = tempDouble;

    if (p == temp.length() - 1) {
        readIndexInt[0] = -1;
        readIndexInt[1] = -1;
        return readNoSignal;
    }

    p++;
    tempInt = stringToInt(temp, &p);
    if (tempInt == stringToIntAbnormal)
        return readAbnormal;
    readIndexInt[0] = tempInt;

    p++;
    tempInt = stringToInt(temp, &p);
    if (tempInt == stringToIntAbnormal)
        return readAbnormal;
    readIndexInt[1] = tempInt;
    return readWithSignal;
}

void printDoubleIntInt(){
    Serial.print(readIndexDouble[0]);
    Serial.print(' ');
    Serial.print(readIndexInt[0]);
    Serial.print(' ');
    Serial.println(readIndexInt[1]);
}

unsigned int stringHash(String temp) {
    unsigned long long num = 0;
    for (int i = 0; i < temp.length(); ++i)
        num = num * 2333 + temp[i];
    return num;
}

void processCmd(String cmd){
    if (debugMode) {
        Serial.println("Processing command: " + cmd);
    }
    int p = 0;
    int readSignal;

    String cmdHead = "";
    for ( ; p < cmd.length(); ++p)
        if (cmd[p] != '#')
            cmdHead += cmd[p];
        else
            break;

    if (ifBufferOn && stringHash(cmdHead) != 36603 && stringHash(cmdHead) != 6625) {// != bfOff / bfOn
        if (debugMode)
            Serial.println("storing in buffer");
        bufferStorage[bufferStorageNum++] = cmd;
        return;
    }

    switch(stringHash(cmdHead)){

        case 11798://stop
            if (debugMode)
                Serial.println("Stopping all orders");
            for (int i = 0; i < stepperMotor_n; ++i)
                while (!stepperMotor[i].timeQueue.empty())
                    stepperMotor[i].timeQueue.pop();
            for (int i = 0; i < arm_n; ++i)
                while (!arm[i].timeQueue.empty())
                    arm[i].timeQueue.pop();
            break;

        case 16123://reset
            if (debugMode)
                Serial.println("Resetting");
////not implemented
            break;

        case 6625://bfOn
            if (debugMode)
                Serial.println("Doing bfOn");
            ifBufferOn = true;
            break;

        case 36603://bfOff
            if (debugMode)
                Serial.println("Doing bfOff");
            ifBufferOn = false;
            for (int i = 0; i < bufferStorageNum; ++i)
                processCmd(bufferStorage[i]);
            break;

        case 24028://cgSg
            if (debugMode)
                Serial.println("Doing change signal");
            p++;
            readIndexInt[0] = stringToInt(cmd, &p);
            if (readIndexInt[0] != stringToIntAbnormal)
                signal = readIndexInt[0];
            if (debugMode)
                Serial.print("Change signal to ");
                Serial.println(readIndexInt[0]);
            break;

        case 5339://dlRtBs
            if (debugMode)
                Serial.println("Processing delay rotation base command");
            readSignal = readIntDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readIntDoubleIntInt responds abnormal");
                break;
            }
            else {
                if (readIndexInt[0] != armLeft && readIndexInt[0] != armRight) {
                    Serial.println("Error: delay rotation base armNum != armLeft or armRight");
                    break;
                }
                if (debugMode) {
                    Serial.print("Doing delay rotation base ");
                    printIntDoubleIntInt();
                }
                delayRotationBase(readIndexInt[0] + rotationBaseLeft, readIndexDouble[0], readIndexInt[1], readIndexInt[2]);
            }
            break;

        case 2265://rtRtBs
            if (debugMode)
                Serial.println("Processing rotate rotation base to command");
            readSignal = readIntDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readIntDoubleIntInt responds abnormal");
                printIntDoubleIntInt();
                break;
            }
            else {
                if (readIndexInt[0] != armLeft && readIndexInt[0] != armRight) {
                    Serial.println("Error: rotate rotation base armNum != armLeft or armRight");
                    break;
                }
                if (debugMode) {
                    Serial.print("Doing rotate rotation base to ");
                    printIntDoubleIntInt();
                }
                rotateRotationBaseTo(readIndexInt[0] + rotationBaseLeft, readIndexDouble[0], readIndexInt[1], readIndexInt[2]);
            }
            break;

        case 60735://dlLfBs
            if (debugMode)
                Serial.println("Processing delay lift base command");
            readSignal = readIntDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readIntDoubleIntInt responds abnormal");
                printIntDoubleIntInt();
                break;
            }
            else {
                if (readIndexInt[0] != armLeft && readIndexInt[0] != armRight) {
                    Serial.println("Error: delay lift base armNum != armLeft or armRight");
                    break;
                }
                if (debugMode) {
                    Serial.print("Doing delay lift base ");
                    printIntDoubleIntInt();
                }
                delayLiftBase(readIndexInt[0] + liftBaseLeft, readIndexDouble[0], readIndexInt[1], readIndexInt[2]);
            }
            break;

        case 43457://lfLfBs
            if (debugMode)
                Serial.println("Processing lift lift base to command");
            readSignal = readIntDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readIntDoubleIntInt responds abnormal");
                printIntDoubleIntInt();
                break;
            }
            else {
                if (readIndexInt[0] != armLeft && readIndexInt[0] != armRight) {
                    Serial.println("Error: lift lift base armNum != armLeft or armRight");
                    break;
                }
                if (debugMode) {
                    Serial.print("Doing lift lift base to ");
                    printIntDoubleIntInt();
                }
                liftBaseTo(readIndexInt[0] + liftBaseLeft, readIndexDouble[0], readIndexInt[1], readIndexInt[2]);
            }
            break;

        case 54292://dlMvBs
            if (debugMode)
                Serial.println("Processing delay move base to command");
            readSignal = readDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readDoubleIntInt responds abnormal");
                printDoubleIntInt();
                break;
            }
            else {
                if (debugMode) {
                    Serial.print("Doing delay move base ");
                    printDoubleIntInt();
                }
                delayMoveBase(readIndexDouble[0], readIndexInt[0], readIndexInt[1]);
            }
            break;

        case 13395://mvMvBs
            if (debugMode)
                Serial.println("Processing move base to command");
            readSignal = readDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readDoubleIntInt responds abnormal");
                printDoubleIntInt();
                break;
            }
            else {
                if (debugMode) {
                    Serial.print("Doing move base to ");
                    printDoubleIntInt();
                }
                moveBaseTo(readIndexDouble[0], readIndexInt[0], readIndexInt[1]);
            }
            break;

        case 3722://dlAm
            if (debugMode)
                Serial.println("Processing delay arm command");
            readSignal = readIntDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readIntDoubleIntInt responds abnormal");
                printIntDoubleIntInt();
                break;
            }
            else {
                if (readIndexInt[0] != armLeft && readIndexInt[0] != armRight) {
                    Serial.println("Error: delay arm armNum != armLeft or armRight");
                    break;
                }
                if (debugMode) {
                    Serial.print("Doing delay arm ");
                    printIntDoubleIntInt();
                }
                delayArm(readIndexInt[0], readIndexDouble[0], readIndexInt[1], readIndexInt[2]);
            }
            break;

        case 14289://mvAm
            if (debugMode)
                Serial.println("Processing move arm command");
            readSignal = readIntDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readIntDoubleIntInt responds abnormal");
                printIntDoubleIntInt();
                break;
            }
            else {
                if (readIndexInt[0] != armLeft && readIndexInt[0] != armRight) {
                    Serial.println("Error: move arm armNum != armLeft or armRight");
                    break;
                }
                if (debugMode) {
                    Serial.print("Doing move arm ");
                    printIntDoubleIntInt();
                }
                moveArm(readIndexInt[0], readIndexDouble[0], readIndexInt[1], readIndexInt[2]);
            }
            break;

        case 22773://rtSvTwo
            if (debugMode)
                Serial.println("Processing rotate servo two command");
            readSignal = readIntDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readIntDoubleIntInt responds abnormal");
                printIntDoubleIntInt();
                break;
            }
            else {
                if (readIndexInt[0] != armLeft && readIndexInt[0] != armRight) {
                    Serial.println("Error: rotate servo two armNum != armLeft or armRight");
                    break;
                }
                if (debugMode) {
                    Serial.print("Doing rotate servo two ");
                    printIntDoubleIntInt();
                }
                rotateServoTwo(readIndexInt[0], readIndexDouble[0], readIndexInt[1], readIndexInt[2]);
            }
            break;

        case 50297://rtSvOne
            if (debugMode)
                Serial.println("Processing rotate servo one command");
            readSignal = readIntDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readIntDoubleIntInt responds abnormal");
                printIntDoubleIntInt();
                break;
            }
            else {
                if (readIndexInt[0] != armLeft && readIndexInt[0] != armRight) {
                    Serial.println("Error: rotate servo one armNum != armLeft or armRight");
                    break;
                }
                if (debugMode) {
                    Serial.print("Doing rotate servo one ");
                    printIntDoubleIntInt();
                }
                rotateServoOne(readIndexInt[0], readIndexDouble[0], readIndexInt[1], readIndexInt[2]);
            }
            break;

        case 10042://cgMs
            if (debugMode)
                Serial.println("Processing change mass command");
            readSignal = readIntDoubleIntInt(cmd, p);
            if (readSignal == readAbnormal){
                Serial.println("Error: readIntDoubleIntInt responds abnormal");
                printIntDoubleIntInt();
                break;
            }
            else {
                if (readIndexInt[0] != armLeft && readIndexInt[0] != armRight) {
                    Serial.println("Error: change mass armNum != armLeft or armRight");
                    break;
                }
                if (debugMode) {
                    Serial.print("Doing change mass ");
                    printIntDoubleIntInt();
                }
                changeMass(readIndexInt[0], readIndexDouble[0], readIndexInt[1], readIndexInt[2]);
            }
            break;

        default:
            Serial.println("Error: Unknown command, please check again");
            break;
    }
}

void processSPI() {
    if (readFinished) {
        memcpy(msg, buf, BUFFER_SIZE);
        memset(buf, 0x00, BUFFER_SIZE);
        String tempCmd = msg;
        if (debugMode) {
            Serial.print("Receive message:");
            Serial.println(tempCmd);
        }
        processCmd(tempCmd);
        readFinished = false;
    }
}

#endif