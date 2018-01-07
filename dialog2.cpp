#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "dialog.h"
#include "dialog2.h"
#include "ui_dialog2.h"
#include <QFileDialog>
#include "ui_dialog.h"
#include "mythread1.h"
#include "mythread2.h"
#include "mythread3.h"
#include "mythread4.h"
#include <QDebug>

#include <wiringPi.h> //this
#include <pthread.h> // and this
#include <termios.h>
#include <unistd.h>
// LED Pin - wiringPi pin 0 is BCM_GPIO 17.
#define	relayport	7

using namespace std;

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
    ui->label->setText("12345");
    ui->lineEdit->setText("1");
    ui->lineEdit_6->setText("2");
    ui->lineEdit_7->setText("10");

    mThread4 = new MyThread4(this);
    connect(mThread4, SIGNAL(NumberChanged4(int)), this, SLOT(onNumberChanged4(int)));

    mThread6 = new MyThread2(this);
    connect(mThread6, SIGNAL(NumberChanged2(int)), this, SLOT(onNumberChanged2(int)));

    mThread6->filename="./readpos.txt";
    mThread6->times=1;
    mThread6->vig[0]=40;
    mThread6->vig[1]=50;
    mThread6->vig[2]=30;

    mThread6->vpg[0]=900;
    mThread6->vpg[1]=1800;
    mThread6->vpg[2]=1200;

    mThread6->ppg[0]=60;
    mThread6->ppg[1]=90;
    mThread6->ppg[2]=60;
}

Dialog2::~Dialog2()
{
    delete ui;
}
void Dialog2::plain_end()
{

    QTextCursor c = ui->plainTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->plainTextEdit->setTextCursor(c);
}
void Dialog2::ini_IKFK()
{
    mThread4->unit=ui->lineEdit->text().toFloat();
    mThread4->user_sync_type=ui->lineEdit_6->text().toInt();
}

void Dialog2::onNumberChanged2(int Number)
{
    ui->label->setText(QString::number(Number));

    if(Number == 0)
    {
        QString s = QString("Fail to open USB2Dyanmixel!\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 1)
    {
        QString s = QString("Success\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 2)
    {
        QString s = QString("Success to torque off\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 3)
    {
        QString s = QString("Fail to torque off\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 4)
    {
        QString s = QString("Success to torque on\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 5)
    {
        QString s = QString("Fail to torque on\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 6)
    {
        QString s = QString("Success to reboot\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 7)
    {
        QString s = QString("Fail to reboot\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    plain_end();
}
void Dialog2::onNumberChanged4(int Number)
{
    ui->label->setText(QString::number(Number));

    if(Number == 0)
    {
        QString s = QString("exit!\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 1)
    {
        QString s = QString("W\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 2)
    {
        QString s = QString("S\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 3)
    {
        QString s = QString("A\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 4)
    {
        QString s = QString("D\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 5)
    {
        QString s = QString("R\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 6)
    {
        QString s = QString("F\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 7)
    {
        QString s = QString("W\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 8)
    {
        QString s = QString("S\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 9)
    {
        QString s = QString("A\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 10)
    {
        QString s = QString("D\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 11)
    {
        QString s = QString("R\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 12)
    {
        QString s = QString("F\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 13)
    {
        QString s = QString("1\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 14)
    {
        QString s = QString("2\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 15)
    {
        QString s = QString("3\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 16)
    {
        QString s = QString("4\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 17)
    {
        QString s = QString("5\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 18)
    {
        QString s = QString("6\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }

    //plain_end();
}
void Dialog2::on_pushButton_clicked()
{
    mThread6->cmd=qstrdup(qPrintable(QString("readpos")));
    mThread6->cmd="readpos";
    qDebug()  << "CMD:" << mThread6->cmd;
    mThread6->start();

    while(1)
    {
        if(mThread6->isFinished())
        {
            break;
        }
    }

    cout << "mThread2->state:" << mThread6->state<<endl;

    if(mThread6->state==1)
    {
        sleep(1);

        if(ui->radioButton_3->isChecked())
        {
            mThread4->mode = 1;
            mThread4->coord[0] = ui->lineEdit_3->text().toDouble();
            mThread4->coord[1] = ui->lineEdit_4->text().toDouble();
            mThread4->coord[2] = ui->lineEdit_5->text().toDouble();
        }
        ini_IKFK();
        mThread4->start();
        QString s = QString("readpos ok\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else
    {
        QString s = QString("readpos fail\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
}

void Dialog2::on_pushButton_10_clicked()
{
    ini_IKFK();
    mThread4->C = 'w';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_13_clicked()
{
    ini_IKFK();
    mThread4->C = 's';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_12_clicked()
{
    ini_IKFK();
    mThread4->C = 'a';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_14_clicked()
{
    ini_IKFK();
    mThread4->C = 'd';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_9_clicked()
{
    ini_IKFK();
    mThread4->C = 'r';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_11_clicked()
{
    ini_IKFK();
    mThread4->C = 'f';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_2_clicked()
{

    ini_IKFK();
    mThread4->C = 'q';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_3_clicked()
{
    ini_IKFK();
    mThread4->C = 'w';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_6_clicked()
{
    ini_IKFK();
    mThread4->C = 's';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_4_clicked()
{
    ini_IKFK();
    mThread4->C = 'd';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_7_clicked()
{
    ini_IKFK();
    mThread4->C = 'a';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_5_clicked()
{
    ini_IKFK();
    mThread4->C = 'r';
    mThread4->type=0;
    mThread4->Check = true;
}
void Dialog2::on_pushButton_8_clicked()
{
    ini_IKFK();
    mThread4->C = 'f';
    mThread4->type=0;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_16_clicked()
{
    ini_IKFK();
    mThread4->C = '1';
    mThread4->type=1;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_18_clicked()
{
    ini_IKFK();
    mThread4->C = '2';
    mThread4->type=1;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_20_clicked()
{
    ini_IKFK();
    mThread4->C = '3';
    mThread4->type=1;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_22_clicked()
{
    ini_IKFK();
    mThread4->C = '4';
    mThread4->type=1;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_24_clicked()
{
    ini_IKFK();
    mThread4->C = '5';
    mThread4->type=1;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_26_clicked()
{
    ini_IKFK();
    mThread4->C = '6';
    mThread4->type=1;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_15_clicked()
{
    ini_IKFK();
    mThread4->C = '1';
    mThread4->type=2;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_17_clicked()
{
    ini_IKFK();
    mThread4->C = '2';
    mThread4->type=2;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_19_clicked()
{
    ini_IKFK();
    mThread4->C = '3';
    mThread4->type=2;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_21_clicked()
{
    ini_IKFK();
    mThread4->C = '4';
    mThread4->type=2;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_23_clicked()
{
    ini_IKFK();
    mThread4->C = '5';
    mThread4->type=2;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_25_clicked()
{
    ini_IKFK();
    mThread4->C = '6';
    mThread4->type=2;
    mThread4->Check = true;
}

void Dialog2::on_pushButton_27_clicked()
{
    mThread6->cmd="syncw";
    mThread6->omega_limit=ui->lineEdit_7->text().toFloat();
    mThread6->start();

    while(1)
    {
        if(mThread6->isFinished())
        {
            QString s1 = QString("Move..\n");
            ui->plainTextEdit->textCursor().insertText(s1);
            break;
        }
    }
}

void Dialog2::on_pushButton_28_clicked()
{
    mThread6->cmd="init";
    qDebug()  << "CMD:" << mThread6->cmd;
    mThread6->start();
}
void suck_open()
{
    wiringPiSetup () ;
    pinMode (relayport, OUTPUT) ;

    digitalWrite (relayport, HIGH);  //high
}
void suck_close()
{
    wiringPiSetup () ;
    pinMode (relayport, OUTPUT) ;

    digitalWrite (relayport, LOW);  //high
}
void Dialog2::on_pushButton_29_clicked()
{
    suck_open();
    fstream fst2;
    fst2.open("readpos_IKFK.txt", ios::out|ios::app);
    fst2 << "pumpon\t1\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    fst2.close();
}

void Dialog2::on_pushButton_30_clicked()
{
    suck_close();
    fstream fst2;
    fst2.open("readpos_IKFK.txt", ios::out|ios::app);
    fst2 << "pumpoff\t1\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    fst2.close();
}

void Dialog2::on_pushButton_31_clicked()
{
    ini_IKFK();
    mThread4->C = 't';
    mThread4->coord[0] = ui->lineEdit_3->text().toDouble();
    mThread4->coord[1] = ui->lineEdit_4->text().toDouble();
    mThread4->coord[2] = ui->lineEdit_5->text().toDouble();
    mThread4->Check = true;
}
