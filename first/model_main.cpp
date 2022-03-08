#include "model_main.h"
#include "ui_model_main.h"

#include <ui_model_main.h>

#include <QMessageBox>
#include <QWidget>
#include <QString>
#include <QListView>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QSerialPort>
#include <QDebug>
#include <QFile>
#include <QtGui>
#include <QFileDialog>


QSerialPort serialPort;
QStringList strList;
QString StartPos[12] = {"2000","2000", "1130", "1250", "1256", "1176",  "1200","1850","1280", "1500","600","1800"};
QString Values[12] = {"2000","2000", "1130", "1250", "1256", "1176",  "1200","1850","1280", "1500","600","1800"};//массив значений с слайдеров
QString Values2[12];
QString Values3[12];
QString Values4[12];
//QFile file("C:\\test.txt");


Model_main::Model_main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Model_main)
{

    ui->setupUi(this);

    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(Slayder(int)));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),this,SLOT(Slayder2(int)));
    connect(ui->horizontalSlider_3,SIGNAL(valueChanged(int)),this,SLOT(Slayder3(int)));
    connect(ui->horizontalSlider_4,SIGNAL(valueChanged(int)),this,SLOT(Slayder4(int)));
    connect(ui->horizontalSlider_5,SIGNAL(valueChanged(int)),this,SLOT(Slayder5(int)));
    connect(ui->horizontalSlider_6,SIGNAL(valueChanged(int)),this,SLOT(Slayder6(int)));

    connect(ui->horizontalSlider_7,SIGNAL(valueChanged(int)),this,SLOT(Slayder7(int)));
    connect(ui->horizontalSlider_8,SIGNAL(valueChanged(int)),this,SLOT(Slayder8(int)));
    connect(ui->horizontalSlider_9,SIGNAL(valueChanged(int)),this,SLOT(Slayder9(int)));
    connect(ui->horizontalSlider_10,SIGNAL(valueChanged(int)),this,SLOT(Slayder10(int)));
    connect(ui->horizontalSlider_11,SIGNAL(valueChanged(int)),this,SLOT(Slayder11(int)));
    connect(ui->horizontalSlider_12,SIGNAL(valueChanged(int)),this,SLOT(Slayder12(int)));



    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            ui->cmbPort->addItem(serialPortInfo.portName());
        }

    // создали экземпляр для общения по последовательному порту

    // указали имя к какому порту будем подключаться
    serialPort.setPortName(this->ui->cmbPort->currentText());
    // указали скорость
    serialPort.setBaudRate(QSerialPort::Baud9600);

    // пробуем подключится
    if (!serialPort.open(QIODevice::ReadWrite)) {
        // если подключится не получится, то покажем сообщение с ошибкой
        QMessageBox::warning(this, "Ошибка", "Не удалось подключится к порту");
        return;
    }
    serialPort.waitForBytesWritten(); // ждем пока дойдет

    // и не знаю с чем тут связано, но, чтобы сообщение дошло
    // надо обязательно прочитать не пришло ли нам чего в ответ
    //
    // функция waitForReadyRead(10) проверят не появилось
    // в ближайшие 10 миллисекунд чего-нибудь в ответ
    while (serialPort.waitForReadyRead(10)) {
        // и если появилось мы просто это читаем в пустоту
        serialPort.readAll();
        // сам while необходим для того что ответ приходит порциями
        // и мы хотим считать все что отправилось
    }

    /* Инициализируем Таймер и подключим его к слоту,
         * который будет обрабатывать timeout() таймера
         * */
    timer = new QTimer();
       connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
       timer->start(50);
}



Model_main::~Model_main()
{
    delete ui;
}

void Model_main::slotTimerAlarm()// Ежесекундно обновляем данные
{
    serialPort.open(QIODevice::ReadWrite);
    //if(serialPort.open(QIODevice::ReadWrite)){
    if(Values[0]==""){
    for(int i=0; i<12; i++){//если значение со слайдера еще не пришло
         Values[i] = StartPos[i];
        }
    }
    for(int i=0; i<12; i++){
        serialPort.write(Values[i].toUtf8());
        serialPort.write(",");
    }
  serialPort.write("@");

  serialPort.waitForBytesWritten(); // ждем пока дойдет

  // и не знаю с чем тут связано, но, чтобы сообщение дошло
  // надо обязательно прочитать не пришло ли нам чего в ответ
  //
  // функция waitForReadyRead(10) проверят не появилось
  // в ближайшие 10 миллисекунд чего-нибудь в ответ
  while (serialPort.waitForReadyRead(10)) {
      // и если появилось мы просто это читаем в пустоту
      serialPort.readAll();
      // сам while необходим для того что ответ приходит порциями
      // и мы хотим считать все что отправилось
  }
  serialPort.close();

}

void Model_main::Slayder(int a){
    QSlider *horizontalSlider =(QSlider *)sender();
    ui->valuelabel_1->setNum(horizontalSlider->value());
    ui->spinBox->setValue(horizontalSlider->value());
    QString b = QString::number(a);
    Values[0]=b;

}
void Model_main::Slayder2(int a){
    QSlider *horizontalSlider_2 =(QSlider *)sender();
    ui->valuelabel_2->setNum(horizontalSlider_2->value());
    ui->spinBox_2->setValue(horizontalSlider_2->value());
    QString b = QString::number(a);
    Values[1]=b;

}
void Model_main::Slayder3(int a){
    QSlider *horizontalSlider_3 =(QSlider *)sender();
    ui->valuelabel_3->setNum(horizontalSlider_3->value());
    ui->spinBox_3->setValue(horizontalSlider_3->value());
     QString b = QString::number(a);
    Values[2]=b;
}
void Model_main::Slayder4(int a){
    QSlider *horizontalSlider_4 =(QSlider *)sender();
    ui->valuelabel_4->setNum(horizontalSlider_4->value());
    ui->spinBox_4->setValue(horizontalSlider_4->value());
    QString b = QString::number(a);
    Values[3]=b;
}
void Model_main::Slayder5(int a){
    QSlider *horizontalSlider_5 =(QSlider *)sender();
    ui->valuelabel_5->setNum(horizontalSlider_5->value());
    ui->spinBox_5->setValue(horizontalSlider_5->value());
    QString b = QString::number(a);
    Values[4]=b;
}
void Model_main::Slayder6(int a){
    QSlider *horizontalSlider_6 =(QSlider *)sender();
    ui->valuelabel_6->setNum(horizontalSlider_6->value());
    ui->spinBox_6->setValue(horizontalSlider_6->value());
    QString b = QString::number(a);
    Values[5]=b;
}
void Model_main::Slayder7(int a){
    QSlider *horizontalSlider_7 =(QSlider *)sender();
    ui->valuelabel_7->setNum(horizontalSlider_7->value());
    ui->spinBox_7->setValue(horizontalSlider_7->value());
    QString b = QString::number(a);
    Values[6]=b;
}
void Model_main::Slayder8(int a){
    QSlider *horizontalSlider_8=(QSlider *)sender();
    ui->valuelabel_8->setNum(horizontalSlider_8->value());
    ui->spinBox_8->setValue(horizontalSlider_8->value());
     QString b = QString::number(a);
   Values[7]=b;

}
void Model_main::Slayder9(int a){
    QSlider *horizontalSlider_9 =(QSlider *)sender();
    ui->valuelabel_9->setNum(horizontalSlider_9->value());
    ui->spinBox_9->setValue(horizontalSlider_9->value());
     QString b = QString::number(a);
   Values[8]=b;

}
void Model_main::Slayder10(int a){
    QSlider *horizontalSlider_10 =(QSlider *)sender();
    ui->valuelabel_10->setNum(horizontalSlider_10->value());
    ui->spinBox_10->setValue(horizontalSlider_10->value());
     QString b = QString::number(a);
   Values[9]=b;

}
void Model_main::Slayder11(int a){
    QSlider *horizontalSlider_11 =(QSlider *)sender();
    ui->valuelabel_11->setNum(horizontalSlider_11->value());
    ui->spinBox_11->setValue(horizontalSlider_11->value());
     QString b = QString::number(a);
   Values[10]=b;

}
void Model_main::Slayder12(int a){
    QSlider *horizontalSlider_12 =(QSlider *)sender();
    ui->valuelabel_12->setNum(horizontalSlider_12->value());
    ui->spinBox_12->setValue(horizontalSlider_12->value());
     QString b = QString::number(a);
   Values[11]=b;
}

void Model_main::SlayderUpdate(){}

void Model_main::on_pushButton_clicked()//record
{

}
void Model_main::on_pushButton_2_clicked()//import
{
    QString fileName = QFileDialog::getOpenFileName(this,
                        QString::fromUtf8("Открыть файл"),
                        QDir::currentPath(),
                        "Text (*.txt *.TXT)");
    QFile file(fileName);//путь до файла
        if(file.open(QIODevice::ReadOnly |QIODevice::Text))
        {
            QStringList LIST;
            int j=0;
            ui->label_13->clear();
            ui->label_14->clear();
            ui->label_15->clear();
            ui->label_16->clear();
            while(!file.atEnd())
            {
                //читаем строку
                QString str = file.readLine();
                //Делим строку на слова разделенные пробелом
                QStringList lst = str.split(",");
                LIST.append(lst);
                  // выводим значения
                for (int i=0;i<12;i++) {
                        Values[i]=lst.at(i);
                        switch (j) {
                             case 0: ui->label_13->setText(ui->label_13->text() + Values[i] + " ");break;
                             case 1: ui->label_14->setText(ui->label_14->text() + Values[i] + " ");break;
                             case 2: ui->label_15->setText(ui->label_15->text() + Values[i] + " ");break;
                             case 3: ui->label_16->setText(ui->label_16->text() + Values[i] + " ");break;
                         }
                    }
                j++;
            }

             qDebug()<<LIST[0].toInt();
             ui->horizontalSlider->setSliderPosition(LIST[0].toInt());//правильное обращение к созданному слайдеру
             ui->valuelabel_1->setNum(LIST[0].toInt());
             ui->horizontalSlider_2->setSliderPosition(LIST[1].toInt());
             ui->valuelabel_2->setNum(LIST[1].toInt());
             ui->horizontalSlider_3->setSliderPosition(LIST[2].toInt());
             ui->valuelabel_3->setNum(LIST[2].toInt());
             ui->horizontalSlider_4->setSliderPosition(LIST[3].toInt());
             ui->valuelabel_4->setNum(LIST[3].toInt());
             ui->horizontalSlider_5->setSliderPosition(LIST[4].toInt());
             ui->valuelabel_5->setNum(LIST[4].toInt());
             ui->horizontalSlider_6->setSliderPosition(LIST[5].toInt());
             ui->valuelabel_6->setNum(LIST[5].toInt());
             ui->horizontalSlider_7->setSliderPosition(LIST[6].toInt());
             ui->valuelabel_7->setNum(LIST[6].toInt());
             ui->horizontalSlider_8->setSliderPosition(LIST[7].toInt());
             ui->valuelabel_8->setNum(LIST[7].toInt());
             ui->horizontalSlider_9->setSliderPosition(LIST[8].toInt());
             ui->valuelabel_9->setNum(LIST[8].toInt());
             ui->horizontalSlider_10->setSliderPosition(LIST[9].toInt());
             ui->valuelabel_10->setNum(LIST[9].toInt());
             ui->horizontalSlider_11->setSliderPosition(LIST[10].toInt());
             ui->valuelabel_11->setNum(LIST[10].toInt());
             ui->horizontalSlider_12->setSliderPosition(LIST[11].toInt());
             ui->valuelabel_12->setNum(LIST[11].toInt());
             List.clear();
         }

         else
         {
             qDebug()<< "the file was not opened";
         }
}
void Model_main::on_pushButton_3_clicked()//default
{}
void Model_main::on_pushButton_4_clicked()//connect
{
    /*foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
            ui->cmbPort->addItem(serialPortInfo.portName());
        }*/
    serialPort.setPortName(this->ui->cmbPort->currentText());
    serialPort.setBaudRate(QSerialPort::Baud9600);
    // пробуем подключится
    if (!serialPort.open(QIODevice::ReadWrite)) {
        // если подключится не получится, то покажем сообщение с ошибкой
        QMessageBox::warning(this, "Ошибка", "Не удалось подключится к порту");
        return;
    }


}
void Model_main::on_pushButton_5_clicked()//disconnect
{
    serialPort.close();
}
void Model_main::on_pushButton_6_clicked(){

}//Save



