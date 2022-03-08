//Beepod_Model.ino

//Подключение к проекту библиотеки Servo
#include <Multiservo.h>   //Подключение библиотеки multiservo
#include <Wire.h>         //Подключение библиотеки Wire
#include <SoftwareSerial.h>
#include <Servo.h>

//Номера пинов с сервоприводами

int NumControl[] = {15,16,17, 9,10,11};

//Исходное положение

int StartPos[] = {1200,1850,1280, 1500,700,1000};

Multiservo Servos[6];//Массив сервоприводов

int APos[6];//Массив хранит предыдущие координаты сервоприводов для

//расчета дискретных шагов приращения для перехода на заданную позицию

int BPos[6];//Массив для передачи параметров новой позиции

int ServCount = 6;//Константа хранит количество сервоприводов

int MinTiming = 500;//Минимальная длительность импульса

int MaxTiming = 2400;//Максимальная длительность импульса

String St;//Глобальная строчная переменная для приема шифрованной

//пачки хранящей информацию о новой позиции

 

//Функция производит плавный одновременный переход сервоприводов

//из текущей позиции в заданную параметром xPos

void MovePos(int xPos[]){

int Delta=0;//Переменная для хранения величины приращения

int MaxDelta=0;//Переменная для хранения максимальной величины приращения

//В этом цикле определяется максимальная величина приращения

for (int i=0; i<ServCount; i++){

//Абсолютное приращение i-того элемента

    Delta=abs(APos[i]-xPos[i]);

//если i-тое приращение больше текущего значения максимального

//Приравниваем i-тое максимальному 

    if (MaxDelta < Delta) MaxDelta = Delta;

}

MaxDelta/=10;//Уменьшаем значение в 10 раз

//Этот цикл опредляет количество дискретных шагов, за которые

//будет выполнен полный переход из APos в xPos

for (int i=0; i<MaxDelta; i++)

//В данном цикле происходит проход по всем 6 сервоприводам в

//рамках действий одного дискретного шага

    for (int j=0; j<ServCount; j++){

//j-тый сервопривод перемещается на i-том шаге на

//величину равную (1/количество шагов)*общая величина приращения

      Servos[j].writeMicroseconds(map(i,0,MaxDelta,APos[j],xPos[j]));

//Пауза предназначенна для обеспечения плавного движения

//при совершении переходов с большой амплитудой

      delayMicroseconds(1000);

    }

//В этом цикле новая позиция сервоприводов станет предыдущей

//для следующего вызова функции

  for (int i=0; i<ServCount; i++)

    APos[i]=xPos[i];

}

//Начальная инициализация

void setup(){

//Инициализация сервоприводов

for (int i=0; i<ServCount; i++){

    Servos[i].attach(NumControl[i],MinTiming,MaxTiming);

//Вычисление и присвоение "предыдущей позиции"

    APos[i]=Servos[i].read();

} 

//Эта скорость должна соответствовать установленной в программе

Serial.begin(115200);

//Переход в стартовую позицию

MovePos(StartPos);

}

void loop(){

  byte a=0;

  byte b=0;

//Если в буфере Serial-порта есть данные

while (Serial.available()){

//Чтение 1 символа из буфера

    char Ch=Serial.read();

//если не символ @ то добавляем символ к строке

//наличие сиввола @ означает конец шифрованного послания

    if (Ch!=64) St+=Ch;

    else{

//В этом цикле шифрованое послание расшифровывается и

//перемещается в BPos

      for(int i=0; i<ServCount; i++){

        if (St[i*2]<=90) a=St[i*2]-65; else a=St[i*2]-71;

        if (St[i*2+1]<=90) b=St[i*2+1]-65; else b=St[i*2+1]-71;

        BPos[i]=(a*52)+b+MinTiming;

      }

//Вызов перемещения в новую позицию

      MovePos(BPos);

//Очистка буфера приема строки

      St = "";

    }

  }

} 
