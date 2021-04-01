//
// Created by ethepherein on 2021/4/1.
//
#ifndef stepMotor
#define stepMotor

#include <Arduino.h>
#define stepperMotor_n 6

#define rotationBaseLeft 2
#define rotationBaseRight 3
#define rotationBaseRatio ((double)20.8)

#define liftBaseLeft 0
#define liftBaseRight 1
#define liftBaseRatio ((double)350.0)

#define moveBaseFront 4
#define moveBaseBack 5
#define moveBaseRatio ((double)350.0)


#define timeQueueNum 20
#define timeUnitBeingDelay -1
#define timeUnitDelay 0
#define timeUnitMove 1

//                          l_L, l_R, r_L, r_R, h_1, h_2
//rotate 小齿轮12， 大齿轮64
const int constPul[] =     {28,  13,  33,  12,  34,  36};
const int constDir[] =     {14,  17,  20,  23,  41,  45};
const int intervalTime[] = {250, 250, 250, 250, 150, 150};

struct _timeUnit{
    int id;
    double index;
    _timeUnit() = default;
    _timeUnit(int id, double index){
        this->id = id;
        this->index = index;
    }
};

struct _timeQueue{
    _timeUnit timeUnit[timeQueueNum];
    int p_head, p_end;
    _timeQueue(){
        this->p_head = this->p_end = 0;
    }

    void move_next(int* p){
        if ((*p) == timeQueueNum)
            (*p) = 0;
        else
            (*p)++;
    }

    bool empty(){
        return this->p_head == this->p_end;
    }

    void push(_timeUnit tmpTimeUnit){
        this->timeUnit[this->p_end] = tmpTimeUnit;
        move_next(&this->p_end);
    }

    void pop(){
        move_next(&this->p_head);
    }

    _timeUnit front(){
        return this->timeUnit[this->p_head];
    }

    _timeUnit* frontPoint(){
        return this->timeUnit + this->p_head;
    }
};

struct _stepperMotor{
    int id, pul, dir, current_dir;//current_dir: 0->无方向, counter: 计数器
    unsigned long int time;
    unsigned long long counter;
    double current_position;//current_position继承了angle与position,命名为position
    _timeQueue timeQueue;

    _stepperMotor() = default;
    void init(int id){
        this->id = id;
        pinMode(constPul[this->id], OUTPUT);
        pinMode(constDir[this->id], OUTPUT);
        this->counter = 1;
        this->pul = constPul[this->id];
        this->dir = constDir[this->id];
        this->current_dir = 0;
        this->time = micros();
        this->current_position = 0.0;
    }

    bool rotate(double to_angle){//正：逆时针，负：顺时针
        if (abs(to_angle - this->current_position) < 1.0){
            digitalWrite(this->pul, 0);
            digitalWrite(this->dir, 0);
            this->counter = 1;
            this->current_dir = 0;
            return true;
        }
        int to_dir = to_angle - this->current_position > 0 ? 1 : -1;
        if (this->current_dir != to_dir && this->current_dir != 0){
//            Serial.println("!!!");
            digitalWrite(this->pul, 0);
            if((micros() - this->time) > 1000000){
                this->current_dir = to_dir;
                this->time = micros();
            }
        }
        else{
            digitalWrite(this->dir, (to_dir + 1) / 2);//to_dir = 1, val = 1; to_dir = -1, val = 0
            this->current_dir = to_dir;
            if((micros() - this->time) >= intervalTime[this->id]){
                digitalWrite(pul, this->counter%2);
                this->current_position += to_dir * (this->counter%2 == 1)*0.225;
                this->counter += 1;
                this->time = micros();
            }
        }
        return false;
    }

    void loop_run(){
        if (this->timeQueue.empty())
            return;
//        Serial.println(this->timeQueue.front().id);
        switch(this->timeQueue.front().id){
            case timeUnitBeingDelay:
                if (global_t >= this->timeQueue.front().index)
                    this->timeQueue.pop();
                break;

            case timeUnitDelay:
                this->timeQueue.frontPoint()->id = -1;
                this->timeQueue.frontPoint()->index = global_t + (*this->timeQueue.frontPoint()).index;
                break;

            case timeUnitMove:
                if (this->rotate(this->timeQueue.frontPoint()->index))
                    this->timeQueue.pop();
                break;

        }
    }

}stepperMotor[stepperMotor_n];

//void rotateRotationBase(int rotate_num, double angle){//rotate_num: 0(rotationBaseLeft)->左底座 (rotationBaseRight)->右底座
//    if (rotate_num == 0)
//        stepperMotor[2].rotate(angle * rotationBaseRatio);
//    else
//        stepperMotor[3].rotate(angle * rotationBaseRatio);
//}

void delayRotationBase(int rotateNum, double delay){//delay单位:秒 rotate_num: (rotationBaseLeft)->左底座 (rotationBaseRight)->右底座
    stepperMotor[rotateNum].timeQueue.push(_timeUnit(timeUnitDelay, delay));
}

void rotateRotationBaseTo(int rotateNum, double angle){//angle单位:度 rotateNum: (rotationBaseLeft)->左底座 (rotationBaseRight)->右底座
    stepperMotor[rotateNum].timeQueue.push(_timeUnit(timeUnitMove, angle * rotationBaseRatio));
}

void delayLiftBase(int liftNum, double delay){//delay单位:秒 liftNum: (liftBaseLeft)->左底座 (liftBaseRight)->右底座
    stepperMotor[liftNum].timeQueue.push(_timeUnit(timeUnitDelay, delay));
}

void liftBaseTo(int liftNum, double height){//height单位:厘米, liftNum: (liftBaseLeft)->左底座 (liftBaseRight)->右底座
    Serial.println(height * liftBaseRatio);
    stepperMotor[liftNum].timeQueue.push(_timeUnit(timeUnitMove, height * liftBaseRatio));
}

void delayMoveBase(double delay){//delay单位:秒
    stepperMotor[moveBaseFront].timeQueue.push(_timeUnit(timeUnitDelay, delay));
    stepperMotor[moveBaseBack].timeQueue.push(_timeUnit(timeUnitDelay, delay));
}

void moveBaseTo(double position){//position单位:厘米
    stepperMotor[moveBaseFront].timeQueue.push(_timeUnit(timeUnitMove, position * moveBaseRatio));
    stepperMotor[moveBaseBack].timeQueue.push(_timeUnit(timeUnitMove, position * moveBaseRatio));
}

#endif