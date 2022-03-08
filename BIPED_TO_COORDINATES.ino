#include <Multiservo.h>   //Подключение библиотеки multiservo
#include <Wire.h>         //Подключение библиотеки Wire
#include <SoftwareSerial.h>

// получаем углы при помощи DMP
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps_V6_12.h"
MPU6050 mpu;
float angleX = 0;
float angleY = 0;
float angleZ = 0;

float X = 0;
float Y = 0;
float Z = 0;

float mistake = 0.0;

int MoveNum = 1;//счетчик действий

float nextX=0; // целевая точка (20 сантиметров)
float nextY=0;
float MachineX=-0.1; // нынешнее положение
float MachineY=0;
float PrevX=0;//предыдущее положение
float PrevY=0;
float PrevAngle=180;//предыдущий угол поворота робота, старт в 180 градусах
float speedVecAngle = 0;
float RadiusVecAngle = 0;

float TargetVectorX=0;//целевой вектор(путь робота)
float TargetVectorY=0;

float nextDist=0; //сколько пройти до заданной точки
float nextAngle = 0; // на сколько повернуться до точки
int stepsCount = 0; // кол-во шагов до заданной точки
int stepDist = 13; //13 см за шаг проодит робот

bool flag = false; //флаг для режима робота


int ServCount = 14;    //count сущ[kaʊnt] - пересчет(сервоприводов)
//int MinDist = 200;
int MinTiming = 540;  //минимальный угол
int MaxTiming = 2400;  //максимальный угол
int StartTimeOut = 2000;
int ForwardStepCount = 4;  //подсчет движений для(шагов вперед)
int ReversStepCount = 4;
int RightStepCount = 4;
int LeftStepCount = 4;
int Entrop = 10;

int NumControl[] = {1,2, 3,5, 6,7, 15,16,17, 9,10,11, 12,13}; //массив Номеров пинов с сервоприводами
int StartPos[] = {2000,2000, 1130,1250, 1256,1176, 1200,1850,1280, 1500,700,1000, 900,1900, 1000};
int BalancePos[] = {2000,2000, 1130,1250, 1256,1176, 1200,1850,1280, 1500,700,1000, 900,1900, 1};//второй массив для изменения при наклоне
Multiservo Servos[14];//Массив кол-ва сервоприводов
int APos[14];        //начальная позиция угла сервы

int StepForward[] = {                       // шаги вперед. за 1 шаг проходит 13 см
1870,2050,1345, 1080,1120,1435,  1200,1850,1280, 1500,700,1000, 900,1900, 400,
1850,2050,860,  1445,1530,950,   1200,1850,1280, 1500,700,1000, 900,1900, 300,
2070,1810,990,  1530,1410,1000,  1200,1850,1280, 1500,700,1000, 900,1900, 300,
2080,1890,1360, 1070,945,1400,   1200,1850,1280, 1500,700,1000, 900,1900, 400
};

int StepRevers[] = {
2080,1890,1360, 1070,945,1400,   1200,1850,1280, 1500,700,1000, 900,1900, 400,
2070,1810,990,  1530,1410,1000,  1200,1850,1280, 1500,700,1000, 900,1900, 300,
1850,2050,860,  1445,1530,950,   1200,1850,1280, 1500,700,1000, 900,1900, 300,
1870,2050,1345, 1080,1120,1435,  1200,1850,1280, 1500,700,1000, 900,1900, 400
};

int StepRight[] = {
1870,2078,1049, 1465,1340,956,  1200,1850,1280, 1500,700,1000, 900,1900, 500,
2000,2000,1018, 1465,1340,956,  1200,1850,1280, 1500,700,1000, 900,1900, 500,
1901,1964,1351, 1226,1039,1257, 1200,1850,1280, 1500,700,1000, 900,1900, 500,
1790,2109,1351, 1226,1039,1257, 1200,1850,1280, 1500,700,1000, 900,1900, 500
};

int StepLeft[] = {
2000,1834,1276, 1129,1062,1319, 1200,1850,1280, 1500,700,1000, 900,1900, 500,
2000,1791,948,  1604,1537,888,  1200,1850,1280, 1500,700,1000, 900,1900, 500,
2113,1791,948,  1604,1537,888,  1200,1850,1280, 1500,700,1000, 900,1900, 500,
2113,1834,1300, 1075,1062,1319, 1200,1850,1280, 1500,700,1000, 900,1900, 500
};


void MovePos(int xPos[]){            //move сущ[muːv] движение, ход, перемещение
  int Delta=0;                        //разница.
  int MaxDelta=0;                     //максимальная разница.
  unsigned long a=0;  
  unsigned long b=0;
  unsigned long TimeIter=0;  
  unsigned long DeltaTime=0;            //разница во времени.
  unsigned long TimePoint=0;            //point - точка(во времени)
  for (int i=0; i<ServCount; i++){ 
    Delta=abs(APos[i]-xPos[i]);  
    if (MaxDelta < Delta) MaxDelta = Delta;
  }
  MaxDelta/=2;
  for (int i=0; i<MaxDelta; i++)
    for (int j=0; j<ServCount; j++){
      TimePoint = micros();                   //point - точка(во времени)
      Servos[j].writeMicroseconds(map(i,0,MaxDelta,APos[j],xPos[j]));// преобразование начальную и конечну позиция угла сервы
      a=xPos[ServCount]*1000.0;//последнее значение в массиве, т.к. счет с нуля
      b=MaxDelta*ServCount;
      TimeIter=a/b;
      DeltaTime=micros()-TimePoint;
      if (DeltaTime<TimeIter) delayMicroseconds(TimeIter-DeltaTime);
    }
  for (int i=0; i<ServCount; i++) APos[i]=xPos[i];
}
//--------------------------------------------ГИРОСКОП------------------------------------------
// НУЖНЫЕ ПЕРЕМЕННЫЕ
const float toDeg = 180.0 / M_PI;
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
// инициализация
void initDMP() {
  devStatus = mpu.dmpInitialize();
  mpu.setDMPEnabled(true);
  mpuIntStatus = mpu.getIntStatus();
  packetSize = mpu.dmpGetFIFOPacketSize();
}
// получение углов в angleX, angleY, angleZ
void getAngles() {
  //mistake+0.08;
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    angleX = ypr[2] * toDeg;
    angleY = ypr[1] * toDeg;
    angleZ = ypr[0] * toDeg + mistake;
    angleZ = map(angleZ,180,-180,0,360);
  }
}

void Orientation(){//измеряет угол между векторами направления робота и пункта назначения
  nextDist = sqrt( sq(nextX-MachineX) + sq(nextY-MachineY) ); //ноль это точка старта
  Serial.print("nextDist ");Serial.println(nextDist);
/*  float SpeedVecX = MachineX - PrevX;//вектор скорости относительно предыдущего положения
  float SpeedVecY = MachineY - PrevY;
  speedVecAngle = acos( (0*SpeedVecX + 1*SpeedVecY)/
        //sqrt(1)* //sq(0)+sq(1))*
        sqrt(sq(SpeedVecX)+sq(SpeedVecY))
        )*57.3; //угол машины относительно вектора j(0;1)
  RadiusVecAngle = acos( nextY/
       sqrt( sq(nextX) + sq(nextY) ) )* 57.3;     //угол радиус-вектора точки относительно вектора j(0;1)
       Serial.print("RadiusVecAngle ");Serial.print(RadiusVecAngle);
        if (nextX > 0){ //если точка в 1 или 4 четверти
        RadiusVecAngle = ((2*180) - RadiusVecAngle);}*/
        
float deltaX = MachineX - nextX; //координаты вектора от одной точки к другой
float deltaY = MachineY - nextY;
deltaX = abs(deltaX);
          nextAngle = acos( deltaX/
       sqrt( sq(deltaX) + sq(deltaY) ) )* 57.3;     //угол радиус-вектора точки относительно вектора i(1;0)
        if (nextY < MachineY){ //если точка ниже робота по Y
        nextAngle = ((2*180) - nextAngle);} 
        if (nextX < MachineX){ //если точка ниже робота по Y
        nextAngle = (180 - nextAngle);} 
  
  /*nextAngle = RadiusVecAngle-speedVecAngle;//на сколько градусов надо повернуть
  Serial.print(" nextAngle1 ");Serial.print(nextAngle);
  Serial.print(" PrevAngle ");Serial.print(PrevAngle);
  nextAngle = PrevAngle+nextAngle;//куда повернуть*/
  
  /*TargetVectorX=nextX-MachineX;//вычисляем кооординаты вектора движения робота
  TargetVectorY=nextY-MachineY;
  nextAngle = (atan2(MachineX*TargetVectorX-MachineY*TargetVectorY, MachineX*TargetVectorX+MachineY*TargetVectorY))*57.3; 
  nextAngle = (acos( (MachineX* nextX+MachineY* nextY)/(sqrt(sq(MachineX)+sq(MachineY))*sqrt(sq( nextX)+sq( nextY))) ))*57.3; 
  Serial.print("\tRadiusVecAngle ");Serial.print(RadiusVecAngle);*/
  Serial.print("\tdeltaX ");Serial.print(deltaX);
    Serial.print("\tdeltaY ");Serial.print(deltaY);
  Serial.print("\tnextAngle ");Serial.print(nextAngle);
  stepsCount = nextDist/stepDist; //кол-во шагов
  Serial.print("\tstepsCount ");Serial.println(stepsCount);
}

//--------------------------********--------------void setup--------------------**********--------------
void setup(){
  Serial.begin(9600);
  
  for (int i=0; i<ServCount; i++){  // пересчет(сервоприводов)
    Servos[i].attach(NumControl[i],MinTiming,MaxTiming); //att-прикреплять сервы по массиву Номеров пинов
    APos[i]=Servos[i].read();
  }                           
  
  MovePos(StartPos);
  delay(StartTimeOut);
  Serial.println("--------------POSITION-------------------");  
  Wire.setClock(400000);
  mpu.initialize();
  initDMP();
  getAngles(); 
  delay(5000);
  getAngles();
  X = angleX;
  Y = angleY;
  Z = angleZ;
  Serial.print(X); Serial.print(',');
  Serial.print(Y); Serial.print(',');
  Serial.println(Z);
  Serial.println("----------------START-------------------");
  delay(1000); 


}

void PlaySteps(int arSteps[], int StepCount){
  int arTMP[ServCount+1];
  for(int i=0; i<StepCount; i++){
    for(int j=0; j<ServCount+1; j++)
      arTMP[j]=arSteps[j+(ServCount+1)*i]+random(-Entrop,Entrop);
    MovePos(arTMP); 
  }
}

//***************************----------*************----void loop----*******************-----**********--------------
void loop(){
switch (MoveNum) {
  
    case 1:
      nextX=-20; 
      nextY=20;
      Orientation();
      MoveTo();    
      break;
      
    case 2:
      flag = false;
      nextX=20; 
      nextY=20;
      Orientation();
      MoveTo();
      break;

    case 3:
      flag = false;
      nextX=20; 
      nextY=0;
      Orientation();
      MoveTo();
      break;

      case 4:
      flag = false;
      nextX=1; 
      nextY=1;
      Orientation();
      MoveTo();
      break;
      
    default:
     MovePos(StartPos);
  }

}

void MoveTo(){
if(flag == false){    

  getAngles();
  Serial.print("                        ");
  Serial.print(angleX); Serial.print(',');  Serial.print(angleY); Serial.print(',');  Serial.println(angleZ);

  if (-30 < angleX && angleX < 30 ) {
    
    if(sqrt(sq(nextAngle-angleZ)) <= 7){
      Serial.print(' ');Serial.print(angleZ);Serial.println(" ok ");
      for(int i=0; i<stepsCount; i++) PlaySteps(StepForward,ForwardStepCount);
      MovePos(StartPos);
      getAngles();
      delay(500);
      flag = true;
      }
    else if ((nextAngle-7.0) > angleZ || (angleZ-nextAngle) > 180) {
      Serial.println("                             -------left ");
      PlaySteps(StepLeft,LeftStepCount);
      }
    else if ((nextAngle+7.0) < angleZ || (nextAngle-angleZ) > 180) {
      Serial.println("                                            right------ ");
      PlaySteps(StepRight,RightStepCount);
      }
   }

 else{
   Serial.print("CRASH  ");
    MovePos(StartPos);
    flag = true;
  }
}  
if(flag == true){
    PrevX = MachineX;
    PrevY = MachineY;
    PrevAngle = nextAngle;
    MachineX = nextX;
    MachineY = nextY;
    MoveNum++;
    Serial.print("MoveNum "); Serial.println(MoveNum);
    Serial.print("MachineX: "); Serial.print(MachineX);Serial.print("MachineY: ");Serial.println(MachineY);
    }
    
}
