#define pul_l_L 28
#define dir_l_L 14
#define pul_l_R 13
#define dir_l_R 17
#define pul_r_L 33
#define dir_r_L 20
#define pul_r_R 12
#define dir_r_R 23
#define pul_h_1 10
#define dir_h_1 45
#define pul_h_2 10
#define dir_h_2 45



void steppermotor(int to_angle,int motorNum)
{
    static int pul;
    static int dir;
    static float current_angle;
    static int current_dir;

    static float current_angle_l_L = 0;
    static float current_angle_l_R = 0;
    static float current_angle_r_L = 0;
    static float current_angle_r_R = 0;
    static float current_position_h = 0;

    static int current_dir_l_L = -1;
    static int current_dir_l_R = -1;
    static int current_dir_r_L = -1;
    static int current_dir_r_R = -1;
    static int current_dir_h = -1;

    if(motorNum == 1) {pul = pul_l_L;dir = dir_l_L;current_dir = current_dir_l_L;current_angle = current_angle_l_L;}
    if(motorNum == 2) {pul = pul_l_R;dir = dir_l_R;current_dir = current_dir_l_R;current_angle = current_angle_l_R;}
    if(motorNum == 3) {pul = pul_r_L;dir = dir_r_L;current_dir = current_dir_r_L;current_angle = current_angle_r_L;}
    if(motorNum == 4) {pul = pul_r_R;dir = dir_r_R;current_dir = current_dir_r_R;current_angle = current_angle_r_R;}
    if(motorNum == 5) {pul = pul_h_1;dir = dir_h_1;current_dir = current_dir_h;current_angle = current_position_h;}

    static int i[7] = {1,1,1,1,1,1,1};
    static int to_dir;
    if(to_angle - current_angle > 0) to_dir = 1;
    if(to_angle - current_angle < 0) to_dir = 0;

    static unsigned long time[7] = {0,0,0,0,0,0,0};

    if(current_dir != to_dir && current_dir != -1){
        digitalWrite(pul, 0);
        if((micros()-time[motorNum]) > 1000000){
            current_dir = to_dir;
            time[motorNum] = micros();
        }
    }
    else
    {
        if(to_dir == 1){
            digitalWrite(dir, 1);
            current_dir = 1;
            if((micros() - time[motorNum]) >= 250){
                digitalWrite(pul, i[motorNum]%2);
                current_angle = current_angle + (i[motorNum]%2 == 1)*0.225;
                i[motorNum] += 1;
                time[motorNum] = micros();
            } 
        }
        if(to_dir == 0){
            digitalWrite(dir, 0);
            current_dir = 0;
            if((micros() - time[motorNum]) >= 250){
                digitalWrite(pul, i[motorNum]%2);
                current_angle = current_angle - (i[motorNum]%2 == 1)*0.225;
                i[motorNum] += 1;
                time[motorNum] = micros();
            } 
        }
        if(to_angle-current_angle == 0){
            i[motorNum] = 1;
            current_dir = -1;//此时方向为任意
        }
    }
    if(motorNum == 1) {current_dir_l_L = current_dir;current_angle_l_L = current_angle;}
    if(motorNum == 2) {current_dir_l_R = current_dir;current_angle_l_R = current_angle;}
    if(motorNum == 3) {current_dir_r_L = current_dir;current_angle_r_L = current_angle;}
    if(motorNum == 4) {current_dir_r_R = current_dir;current_angle_r_R = current_angle;}
    if(motorNum == 5) {current_dir_h = current_dir;current_position_h = current_angle;}
}
void setup()
{
    pinMode(pul_l_L,OUTPUT);
    pinMode(dir_l_L,OUTPUT);
    pinMode(pul_l_R,OUTPUT);
    pinMode(dir_l_R,OUTPUT);
    pinMode(pul_r_L,OUTPUT);
    pinMode(dir_r_L,OUTPUT);
    pinMode(pul_r_R,OUTPUT);
    pinMode(dir_r_R,OUTPUT);
    pinMode(pul_h_1,OUTPUT);
    pinMode(dir_h_1,OUTPUT);
    pinMode(pul_h_2,OUTPUT);
    pinMode(dir_h_2,OUTPUT);
}


void loop(){

      steppermotor(150,5);

    

}
