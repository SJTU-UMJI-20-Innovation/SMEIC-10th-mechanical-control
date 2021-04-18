#include <Servo.h>


Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
int IN1 = 7;，
int IN2 = 8;
int ENA = 2;
bool flag1 = true;
bool flag2 = true;
bool flag3 = true;
void CaCO3(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  static bool flag = true;
  if(!(millis()%25) && flag){
    digitalWrite(ENA, HIGH);
    flag = false;
  }
  if(millis()%25 > 5){
    flag = true;
    digitalWrite(ENA, LOW);
  }
}

void setup() {
  // put your setup code here, to run once:
  servo1.attach(3);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(9);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  servo1.write(60);
  servo2.write(60);
  servo3.write(60);
  servo4.write(60);
}





void loop() {
  
  CaCO3();
  if(millis()>2500 && flag1){
    servo1.write(120);
    servo2.write(120);
    servo3.write(120);
    servo4.write(120);

  }
  if(millis()>3000 && flag1){
    servo1.write(140);//140为完全打开
    servo2.write(140);
    servo3.write(140);
    servo4.write(140);
    flag1 = false;
  }
if(millis()>4000 && flag2){
    servo1.write(100);
    servo2.write(100);
    servo3.write(100);
    servo4.write(100);

  }
  
  if(millis()>4500 && flag2){
    servo1.write(60);//60为完全关闭
    servo2.write(60);
    servo3.write(60);
    servo4.write(60);
    flag2 = false;
  }

  if(millis()>6500 && flag3 == true){
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);
    servo4.write(90);
    flag3 = false;
  }
}
