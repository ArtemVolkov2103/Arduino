#ifndef MODEL_MAIN_H
#define MODEL_MAIN_H

#include <QMainWindow>
#include <QWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/qserialport.h>
#include <QSerialPort>
#include <QTimer>
#include <QTime>
#include <QGridLayout>
#include <QLabel>
#include <QListView>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Model_main; }
QT_END_NAMESPACE

class Model_main : public QMainWindow
{
    Q_OBJECT

public:
    Model_main(QWidget *parent = nullptr);
    ~Model_main();
 QString Values[12];

public slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void Slayder(int);
    void Slayder2(int);
    void Slayder3(int);
    void Slayder4(int);
    void Slayder5(int);
    void Slayder6(int);
    void Slayder7(int);
    void Slayder8(int);
    void Slayder9(int);
    void Slayder10(int);
    void Slayder11(int);
    void Slayder12(int);
    void SlayderUpdate();

private:
    Ui::Model_main *ui;
    QSerialPort serialPort;
    QTimer *timer;
    QStringListModel *Model;
       QListView *ListView;
       QStringList List;
private slots:
    void slotTimerAlarm();

    //void on_horizontalSlider_sliderMoved(int position);
};
#endif // MODEL_MAIN_H
