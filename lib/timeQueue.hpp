#ifndef my_timeQueue
#define my_timeQueue

#define timeUnitBeingDelay -1
#define timeUnitDelay 0
#define timeUnitStepperMotorMove 1

#define timeUnitReachArm 1
#define timeUnitRotateServoTwo 2

int signal = 0;
int lastAppliedSignal = 0;

struct _timeUnit{
    _timeUnit *last, *next;
    int id,startSignal, finishSignal;
    double index;
//    int finish_flag = -1;
    _timeUnit() = default;
    _timeUnit(int id, double index, int startSignal = -1, int finishSignal = -1){
        this->id = id;
        this->index = index;
        this->startSignal = startSignal;
        this->finishSignal = finishSignal;
//        Serial.println("33");
//        Serial.println(finishSignal);
    }
};

struct _timeQueue{
//    _timeUnit timeUnit[timeQueueNum];
    _timeUnit *p_head, *p_end;
    _timeQueue(){
        this->p_head = new _timeUnit(-1, 0);
        if (this->p_head == nullptr)
            Serial.println("Critical Error: memory not enough!!!");
        this->p_end = this->p_head;
    }

    bool empty(){
        return this->p_head == this->p_end;
    }

    void push(int id, double index, int startSignal = -1, int finishSignal = -1){
        _timeUnit *new_p_end = new _timeUnit(id, index, startSignal, finishSignal);
        if (new_p_end == nullptr)
            Serial.println("Critical Error: memory not enough!!!");
        p_end->next = new_p_end;
        new_p_end->last = p_end;
        p_end = new_p_end;
    }

    void pop(){
        _timeUnit *release_p = p_head;
        this->p_head = this->p_head->next;
        delete(release_p);
    }

    _timeUnit front(){
        return *this->p_head->next;
    }

    _timeUnit* frontPoint(){
        return this->p_head->next;
    }
};

#endif