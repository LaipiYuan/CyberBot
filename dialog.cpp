#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>

#include "dialog.h"
#include "dialog2.h"
#include <math.h>
#include <QFileDialog>
#include "ui_dialog.h"
#include "mythread1.h"
#include "mythread2.h"
#include "mythread3.h"
#include "mythread4.h"
#include "mythread5.h"
#include "mythread6.h"
#include "mythread7.h"
#include "mythread8.h"
#include <QModelIndex>
#include <QDebug>
#include <QMessageBox>
#include <wait.h>
#include <QTime>

#include <wiringPi.h> //thi
#include <pthread.h> // and this
#include <termios.h>
#include <unistd.h>
// LED Pin - wiringPi pin 0 is BCM_GPIO 17.
#define	relayport	7
#define NUM 4
#define PI 3.14159265

using namespace std;

int j=0;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    valve_tmp=0;
    torque_tmp=0;
    group = 0;
    ui->setupUi(this);

    ui->toolBox->setCurrentIndex(1);

    ui->lineEdit->setText("22");    //omega limit
    ui->lineEdit_2->setText("1");

    ui->lineEdit_6->setText("7000");
    ui->lineEdit_4->setText("1");
    ui->lineEdit_5->setText("0.3");   //move time
    ui->lineEdit_step_span->setText("0");
    ui->lineEdit_delay_time->setText("20");
    ui->lineEdit_EM_span->setText("1");
    ui->lineEdit_current_time->setText("0");    //#@!

    ui->lineEdit_ox->setText("1");
    ui->lineEdit_oy->setText("0");
    ui->lineEdit_oz->setText("0");

    ui->lineEdit_8->setText("0");
    ui->lineEdit_9->setText("30");
    ui->lineEdit_10->setText("5.3");

    ui->lineEdit_adj_joint1->setText("0");
    ui->lineEdit_adj_joint2->setText("0");
    ui->lineEdit_adj_joint3->setText("0");
    ui->lineEdit_adj_joint4->setText("0");
    ui->lineEdit_group->setText("0");

    ui->lineEdit_angle->setText("10");
    ui->lineEdit_13->setText("1");
    ui->lineEdit_14->setText("1");
    ui->lineEdit_20->setText("5.3");


    ui->checkBox->setChecked(1);
    ui->checkBox_2->setChecked(1);
    ui->checkBox_3->setChecked(1);
    ui->checkBox_4->setChecked(1);


    ui->tableWidget->setWindowTitle("Path");
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(16);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->pushButton_delete->setIcon(QIcon("delete.png"));
    ui->pushButton_3->setIcon(QIcon("clear2.png"));
    ui->pushButton_add_group->setIcon(QIcon("add_groups3.png"));
    ui->pushButton_27->setIcon(QIcon("play.png"));
    ui->pushButton_13->setIcon(QIcon("load.png"));
    ui->pushButton_52->setIcon(QIcon("save.png"));

//    ui->lineEdit_joint_type1->setText("1");
//    ui->lineEdit_joint_type2->setText("1");
//    ui->lineEdit_joint_type3->setText("1");
//    ui->lineEdit_joint_type4->setText("1");


    ui->comboBox->insertItem(0,"H54W200S500");
    ui->comboBox->insertItem(1,"H54W100S500");
    ui->comboBox->insertItem(2,"L54W50S500");
    ui->comboBox->insertItem(3,"L54W50S290");
    ui->comboBox->insertItem(4,"L54W30S500");
    ui->comboBox->insertItem(5,"L54W30S400");
    ui->comboBox->insertItem(6,"L42W10S300");

    ui->comboBox_2->insertItem(0,"H54W200S500");
    ui->comboBox_2->insertItem(1,"H54W100S500");
    ui->comboBox_2->insertItem(2,"L54W50S500");
    ui->comboBox_2->insertItem(3,"L54W50S290");
    ui->comboBox_2->insertItem(4,"L54W30S500");
    ui->comboBox_2->insertItem(5,"L54W30S400");
    ui->comboBox_2->insertItem(6,"L42W10S300");

    ui->comboBox_3->insertItem(0,"H54W200S500");
    ui->comboBox_3->insertItem(1,"H54W100S500");
    ui->comboBox_3->insertItem(2,"L54W50S500");
    ui->comboBox_3->insertItem(3,"L54W50S290");
    ui->comboBox_3->insertItem(4,"L54W30S500");
    ui->comboBox_3->insertItem(5,"L54W30S400");
    ui->comboBox_3->insertItem(6,"L42W10S300");

    ui->comboBox_4->insertItem(0,"H54W200S500");
    ui->comboBox_4->insertItem(1,"H54W100S500");
    ui->comboBox_4->insertItem(2,"L54W50S500");
    ui->comboBox_4->insertItem(3,"L54W50S290");
    ui->comboBox_4->insertItem(4,"L54W30S500");
    ui->comboBox_4->insertItem(5,"L54W30S400");
    ui->comboBox_4->insertItem(6,"L42W10S300");


    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(2);
    ui->comboBox_3->setCurrentIndex(3);
    ui->comboBox_4->setCurrentIndex(6);


    ui->comboBox_cw1->insertItem(0, "CW");
    ui->comboBox_cw1->insertItem(1, "CCW");

    ui->comboBox_cw2->insertItem(0, "CW");
    ui->comboBox_cw2->insertItem(1, "CCW");

    ui->comboBox_cw3->insertItem(0, "CW");
    ui->comboBox_cw3->insertItem(1, "CCW");

    ui->comboBox_cw4->insertItem(0, "CW");
    ui->comboBox_cw4->insertItem(1, "CCW");


    ui->comboBox_cw1->setCurrentIndex(0);
    ui->comboBox_cw2->setCurrentIndex(0);
    ui->comboBox_cw3->setCurrentIndex(1);
    ui->comboBox_cw4->setCurrentIndex(1);

    ui->comboBox_posture->insertItem(0, "Left");
    ui->comboBox_posture->insertItem(1, "Right");
    ui->comboBox_posture->setCurrentIndex(0);   //4ax Robot Posture

    /*
    ui->comboBox_ScrewVector->insertItem(0, "(-1, 1 ,0)");
    ui->comboBox_ScrewVector->insertItem(0, "(-1, 0 ,0)");
    ui->comboBox_ScrewVector->insertItem(0, "(-1, -1 ,0)");
    ui->comboBox_ScrewVector->insertItem(0, "(0, -1 ,0)");
    */

    ui->comboBox_coord_order->insertItem(0, "Coord 1, R.D.");   //Right and Down, @Robot reference
    ui->comboBox_coord_order->insertItem(1, "Coord 2, R.U.");   //Right and Up
    ui->comboBox_coord_order->insertItem(2, "Coord 3, L.D.");   //Left and Down
    ui->comboBox_coord_order->insertItem(3, "Coord 4, L.U.");   //Left and Up
    ui->comboBox_coord_order->insertItem(4, "Coord 5, M.D.");   //Middle and Down
    ui->comboBox_coord_order->insertItem(5, "Coord 6, M.U.");   //Middle and Up
    ui->comboBox_coord_order->insertItem(6, "Coord cell");

    ui->comboBox_row->insertItem(0, "1");    //the row of initiall coordinate
    ui->comboBox_row->insertItem(1, "2");
    ui->comboBox_row->insertItem(2, "3");
    ui->comboBox_row->setCurrentIndex(1);

    ui->comboBox_column->insertItem(0, "2");    //the column of middle coordinate
    ui->comboBox_column->insertItem(1, "3");
    ui->comboBox_column->insertItem(2, "4");
    ui->comboBox_column->insertItem(3, "5");
    ui->comboBox_column->insertItem(4, "6");
    ui->comboBox_column->insertItem(5, "7");
    ui->comboBox_column->setCurrentIndex(2);

    ui->checkBox_coord5and6->setChecked(0);

    ui->comboBox_gpioChoose->insertItem(0, "GPIO 1");
    ui->comboBox_gpioChoose->insertItem(1, "GPIO 2");
    ui->comboBox_gpioChoose->insertItem(2, "GPIO 3");
    ui->comboBox_gpioChoose->insertItem(3, "GPIO 4");

    ui->comboBox_mode_choose->insertItem(0,"left absorb");
    ui->comboBox_mode_choose->insertItem(1,"together work");
    ui->comboBox_mode_choose->insertItem(2,"right deflate");

    ui->comboBox_coordinate_type->insertItem(0, "tested tray");
    ui->comboBox_coordinate_type->insertItem(1, "pass tray");
    ui->comboBox_coordinate_type->insertItem(2, "fail tray");
    ui->comboBox_coordinate_type->insertItem(3, "cell");

    ui->lineEdit_tray_length->setText("33.3");
    ui->lineEdit_tray_width->setText("39.9");
    ui->lineEdit_numberL->setText("7");
    ui->lineEdit_numberW->setText("6");

    ui->checkBox_modify_zvec->setChecked(0);
    ui->checkBox_shift->setChecked(0);


    ui->comboBox_Modbus_mode->insertItem(0, "Teaching");
    ui->comboBox_Modbus_mode->insertItem(1, "Play");

    ui->lineEdit_7->setText("/dev/ttyUSB0");    //modbus dev
    ui->lineEdit_11->setText("115200");           //modbus baudRate
    ui->lineEdit_12->setText("N");              //modbus parity
    ui->lineEdit_15->setText("1");              //set slave(PLC) ID

    ui->lineEdit_16->setText("1");             //cell number
    ui->lineEdit_17->setText("1");             //tested tray number
    ui->lineEdit_18->setText("1");             //pass tray number
    ui->lineEdit_19->setText("1");             //fail tray number


    for(int i=0;i<11;i++)
    {
       ui->tableWidget->setColumnWidth(i,100);
    }

    QStringList header;
    header.append("P1");
    header.append("V1");
    header.append("P2");
    header.append("V2");
    header.append("P3");
    header.append("V3");
    header.append("P4");
    header.append("V4");
    header.append("MT");
    header.append("POS");
    header.append("OV");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->lineEdit_vig_1->setText("20");  //14
    ui->lineEdit_vig_2->setText("364"); //364
    ui->lineEdit_vig_3->setText("16");  //16
    ui->lineEdit_vig_4->setText("40");  //40

    ui->lineEdit_vpg_1->setText("399"); //399
    ui->lineEdit_vpg_2->setText("200"); //144
    ui->lineEdit_vpg_3->setText("300"); //256
    ui->lineEdit_vpg_4->setText("440"); //440

    ui->lineEdit_ppg_1->setText("32");  //32
    ui->lineEdit_ppg_2->setText("32");  //32
    ui->lineEdit_ppg_3->setText("40");  //32
    ui->lineEdit_ppg_4->setText("64");  //64


    ui->lineEdit_dev->setText("/dev/ttyUSB1");

    connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(tableclicked(int,int)));
    connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableclicked2(int,int)));
    connect(ui->comboBox_Modbus_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(Modbus_mode(int)));

    mThread = new MyThread1(this);
    connect(mThread, SIGNAL(NumberChanged(int)), this, SLOT(onNumberChanged(int)));

    mThread2 = new MyThread2(this);
 //   connect(mThread2,SIGNAL(test(QString)),ui->pte_console,SLOT(appendPlainText(QString)));
    connect(mThread2, SIGNAL(NumberChanged2(int)), this, SLOT(onNumberChanged2(int)));
    connect(mThread2, SIGNAL(sendnumber(int)), this, SLOT(getnumber(int)));
    connect(mThread2, SIGNAL(SendTemp(int , int )), this, SLOT(GetTemp(int , int ))); //$#@!
    connect(mThread2, SIGNAL(SendError(int , double )), this, SLOT(GetError(int , double )));

    mThread3 = new MyThread3(this);
    connect(mThread3, SIGNAL(NumberChanged3(int)), this, SLOT(onNumberChanged3(int)));

    mThread4 = new MyThread4(this);
    connect(mThread4, SIGNAL(NumberChanged4(int)), this, SLOT(onNumberChanged4(int)));

    mThread5 = new mythread5(this);
    connect(mThread5, SIGNAL(NumberChanged5(int)), this, SLOT(onNumberChanged5(int)));

    mThread6 = new MyThread6(this);
    //ui->lineEdit_7->setText("/dev/ttyUSB1");    //modbus dev
    connect(mThread6, SIGNAL(NumberChanged6(int)), this, SLOT(onNumberChanged6(int)));
    //connect(ui->lineEdit_7, SIGNAL(textEdited(QString)), mThread6, SLOT(set_PLC_Dev(QString)));

    mThread7 = new MyThread7(this);
    //mThread7->setDev(get_PLC_Dev()); //default
    connect(mThread7, SIGNAL(NumberChanged7(int)), this, SLOT(onNumberChanged7(int)));
    //connect(ui->lineEdit_7, SIGNAL(textEdited(QString)), mThread7, SLOT(set_PLC_Dev(QString)));

    mThread8 = new MyThread8(this);
    connect(mThread8, SIGNAL(NumberChanged8(int)), this, SLOT(onNumberChanged8(int)));

    mThread2->ID = "robot1";

}

QString Dialog::get_PLC_Dev()
{
    return ui->lineEdit_7->text();
}

void Dialog::tableclicked(int row,int column)
{
    string str0 = ui->tableWidget->item(row,0)->text().toStdString();
    const char *k=str0.c_str();
    if(strcmp(k,"group")==0)
    {
        ;
    }
    else
    {
        qDebug() << row << "\t" << column;

        char index[40]={};
        char index2[40]={};

        string str = ui->tableWidget->item(row,9)->text().toStdString();
        string str2 = str.substr(1,str.size()-2);
        qDebug() << "str:" <<str2.c_str();

        strcpy(index,str2.c_str());


        char *delim = ",";
        char * pch;

        pch = strtok(index,delim);


        qDebug() << pch;
        ui->lineEdit_8->setText(QString(pch));
        pch = strtok (NULL, delim);

        qDebug() << pch;
        ui->lineEdit_9->setText(QString(pch));
        pch = strtok (NULL, delim);

        qDebug() << pch;
        ui->lineEdit_10->setText(QString(pch));
        pch = strtok (NULL, delim);


        //
        string str3=ui->tableWidget->item(row,10)->text().toStdString();
        string str4=str3.substr(1,str3.size()-2);

        strcpy(index2,str4.c_str());


        char * pch2;

        pch2 = strtok(index2,delim);

        qDebug()<<pch2;
        ui->lineEdit_ox->setText(QString(pch2));
        pch2 = strtok (NULL, delim);

        qDebug()<<pch2;
        ui->lineEdit_oy->setText(QString(pch2));
        pch2 = strtok (NULL, delim);

        qDebug()<<pch2;
        ui->lineEdit_oz->setText(QString(pch2));
        pch2 = strtok (NULL, delim);
    }


}

void Dialog::tableclicked2(int row,int column)
{
    QString s1 = QString("So fast, Don't do that..\n");
    ui->plainTextEdit->textCursor().insertText(s1);
}

Dialog::~Dialog()
{
    delete ui;
}



void Dialog::arm_init() //mythread2 ini
{
    mThread2->dev = qstrdup(qPrintable(ui->lineEdit_dev->text()));
    mThread2->filename=qstrdup(qPrintable(ui->lineEdit_3->text()));
    mThread2->times=ui->lineEdit_4->text().toInt();
    mThread2->step_span=ui->lineEdit_step_span->text().toInt();
    mThread2->delay_time = ui->lineEdit_delay_time->text().toInt();
    mThread2->omega_limit=ui->lineEdit->text().toFloat();
    mThread2->move_time = ui->lineEdit_5->text().toFloat();
    mThread2->group = ui->lineEdit_group->text().toInt();
    mThread2->scale = ui->lineEdit_2->text().toFloat();
    mThread2->state = 0;
    mThread2->height1=ui->lineEdit_20->text().toFloat();


    mThread2->joint_use[0] = ui->checkBox->isChecked();
    mThread2->joint_use[1] = ui->checkBox_2->isChecked();
    mThread2->joint_use[2] = ui->checkBox_3->isChecked();
    mThread2->joint_use[3] = ui->checkBox_4->isChecked();

    //qDebug() << "joint1" << mThread2->joint_use[0];
    //qDebug() << "joint2" << mThread2->joint_use[1];
    //qDebug() << "joint3" << mThread2->joint_use[2];
    //qDebug() << "joint4" << mThread2->joint_use[3];

    mThread2->vig[0]=ui->lineEdit_vig_1->text().toInt();
    mThread2->vig[1]=ui->lineEdit_vig_2->text().toInt();
    mThread2->vig[2]=ui->lineEdit_vig_3->text().toInt();
    mThread2->vig[3]=ui->lineEdit_vig_4->text().toInt();

    mThread2->vpg[0]=ui->lineEdit_vpg_1->text().toInt();
    mThread2->vpg[1]=ui->lineEdit_vpg_2->text().toInt();
    mThread2->vpg[2]=ui->lineEdit_vpg_3->text().toInt();
    mThread2->vpg[3]=ui->lineEdit_vpg_4->text().toInt();

    mThread2->ppg[0]=ui->lineEdit_ppg_1->text().toInt();
    mThread2->ppg[1]=ui->lineEdit_ppg_2->text().toInt();
    mThread2->ppg[2]=ui->lineEdit_ppg_3->text().toInt();
    mThread2->ppg[3]=ui->lineEdit_ppg_4->text().toInt();

   // qDebug() << "combobox:" << ui->comboBox->currentText();
   // qDebug() << "combobox:" << ui->comboBox->currentIndex();

    //qDebug() << "1:" << joint_gear_ratio(ui->comboBox->currentIndex());
   // qDebug() << "1:" << joint_pos_range(ui->comboBox->currentIndex());
    mThread2->gear_ratio[0] = joint_gear_ratio(ui->comboBox->currentIndex());
    mThread2->pos_range[0] = joint_pos_range(ui->comboBox->currentIndex());
    mThread2->vel_limit[0] = joint_vel_limit(ui->comboBox->currentIndex());

   // qDebug() << "2:" << joint_gear_ratio(ui->comboBox_2->currentIndex());
   // qDebug() << "2:" << joint_pos_range(ui->comboBox_2->currentIndex());
    mThread2->gear_ratio[1] = joint_gear_ratio(ui->comboBox_2->currentIndex());
    mThread2->pos_range[1] = joint_pos_range(ui->comboBox_2->currentIndex());
    mThread2->vel_limit[1] = joint_vel_limit(ui->comboBox_2->currentIndex());

   // qDebug() << "3:" << joint_gear_ratio(ui->comboBox_3->currentIndex());
    //qDebug() << "3:" << joint_pos_range(ui->comboBox_3->currentIndex());
    mThread2->gear_ratio[2] = joint_gear_ratio(ui->comboBox_3->currentIndex());
    mThread2->pos_range[2] = joint_pos_range(ui->comboBox_3->currentIndex());
    mThread2->vel_limit[2] = joint_vel_limit(ui->comboBox_3->currentIndex());

   // qDebug() << "4:" << joint_gear_ratio(ui->comboBox_4->currentIndex());
   // qDebug() << "4:" << joint_pos_range(ui->comboBox_4->currentIndex());
    mThread2->gear_ratio[3] = joint_gear_ratio(ui->comboBox_4->currentIndex());
    mThread2->pos_range[3] = joint_pos_range(ui->comboBox_4->currentIndex());
    mThread2->vel_limit[3] = joint_vel_limit(ui->comboBox_4->currentIndex());

}

void Dialog::ini_IKFK() //mythread4 ini
{
    mThread4->unit = ui->lineEdit_14->text().toFloat();
    mThread4->angle = ui->lineEdit_angle->text().toFloat();
    mThread4->omega_limit = ui->lineEdit->text().toFloat();
    mThread4->user_sync_type = ui->lineEdit_13->text().toInt();
    mThread4->move_time = ui->lineEdit_5->text().toFloat();

    mThread4->ox = ui->lineEdit_ox->text().toFloat();
    mThread4->oy = ui->lineEdit_oy->text().toFloat();
    mThread4->oz = ui->lineEdit_oz->text().toFloat();

    mThread4->posture = ui->comboBox_posture->currentIndex();

    mThread4->gear_ratio[0] = joint_gear_ratio(ui->comboBox->currentIndex());
    mThread4->pos_range[0] = joint_pos_range(ui->comboBox->currentIndex());
    mThread4->gear_ratio[1] = joint_gear_ratio(ui->comboBox_2->currentIndex());
    mThread4->pos_range[1] = joint_pos_range(ui->comboBox_2->currentIndex());
    mThread4->gear_ratio[2] = joint_gear_ratio(ui->comboBox_3->currentIndex());
    mThread4->pos_range[2] = joint_pos_range(ui->comboBox_3->currentIndex());
    mThread4->gear_ratio[3] = joint_gear_ratio(ui->comboBox_4->currentIndex());
    mThread4->pos_range[3] = joint_pos_range(ui->comboBox_4->currentIndex());

    mThread4->adj_joint[0] = ui->lineEdit_adj_joint1->text().toFloat();
    mThread4->adj_joint[1] = ui->lineEdit_adj_joint2->text().toFloat();
    mThread4->adj_joint[2] = ui->lineEdit_adj_joint3->text().toFloat();
    mThread4->adj_joint[3] = ui->lineEdit_adj_joint4->text().toFloat();

    mThread4->cw[0] = cw(ui->comboBox_cw1->currentIndex());
    mThread4->cw[1] = cw(ui->comboBox_cw2->currentIndex());
    mThread4->cw[2] = cw(ui->comboBox_cw3->currentIndex());
    mThread4->cw[3] = cw(ui->comboBox_cw4->currentIndex());

    //qDebug()<< "mThread4->cw[0]:" << mThread4->cw[0] ;
    //qDebug()<< "mThread4->cw[1]:" << mThread4->cw[1] ;
    //qDebug()<< "mThread4->cw[2]:" << mThread4->cw[2] ;
    //qDebug()<< "mThread4->cw[3]:" << mThread4->cw[3] ;

}


void Dialog::planning_initialization()  //mythread8 ini
{
    /*
    mThread8->unit = ui->lineEdit_14->text().toFloat();
    mThread8->angle = ui->lineEdit_angle->text().toFloat();
    mThread8->omega_limit = ui->lineEdit->text().toFloat();
    mThread8->user_sync_type = ui->lineEdit_13->text().toInt();
    mThread8->move_time = ui->lineEdit_5->text().toFloat();

    mThread8->joint_use[0] = ui->checkBox->isChecked();
    mThread8->joint_use[1] = ui->checkBox_2->isChecked();
    mThread8->joint_use[2] = ui->checkBox_3->isChecked();
    mThread8->joint_use[3] = ui->checkBox_4->isChecked();
    mThread8->joint_use[4] = ui->checkBox_5->isChecked();
    mThread8->joint_use[5] = ui->checkBox_6->isChecked();

    qDebug() << "joint1" << mThread8->joint_use[0];
    qDebug() << "joint2" << mThread8->joint_use[1];
    qDebug() << "joint3" << mThread8->joint_use[2];
    qDebug() << "joint4" << mThread8->joint_use[3];
    qDebug() << "joint5" << mThread8->joint_use[4];
    qDebug() << "joint6" << mThread8->joint_use[5];

    qDebug() << "1:" << joint_pos_range(ui->comboBox->currentIndex());
    mThread8->gear_ratio[0] = joint_gear_ratio(ui->comboBox->currentIndex());
    mThread8->pos_range[0] = joint_pos_range(ui->comboBox->currentIndex());
    mThread8->vel_limit[0] = joint_vel_limit(ui->comboBox->currentIndex());

    qDebug() << "2:" << joint_gear_ratio(ui->comboBox_2->currentIndex());
    qDebug() << "2:" << joint_pos_range(ui->comboBox_2->currentIndex());
    mThread8->gear_ratio[1] = joint_gear_ratio(ui->comboBox_2->currentIndex());
    mThread8->pos_range[1] = joint_pos_range(ui->comboBox_2->currentIndex());
    mThread8->vel_limit[1] = joint_vel_limit(ui->comboBox_2->currentIndex());

    qDebug() << "3:" << joint_gear_ratio(ui->comboBox_3->currentIndex());
    qDebug() << "3:" << joint_pos_range(ui->comboBox_3->currentIndex());
    mThread8->gear_ratio[2] = joint_gear_ratio(ui->comboBox_3->currentIndex());
    mThread8->pos_range[2] = joint_pos_range(ui->comboBox_3->currentIndex());
    mThread8->vel_limit[2] = joint_vel_limit(ui->comboBox_3->currentIndex());

    qDebug() << "4:" << joint_gear_ratio(ui->comboBox_4->currentIndex());
    qDebug() << "4:" << joint_pos_range(ui->comboBox_4->currentIndex());
    mThread8->gear_ratio[3] = joint_gear_ratio(ui->comboBox_4->currentIndex());
    mThread8->pos_range[3] = joint_pos_range(ui->comboBox_4->currentIndex());
    mThread8->vel_limit[3] = joint_vel_limit(ui->comboBox_4->currentIndex());

    qDebug() << "5:" << joint_gear_ratio(ui->comboBox_5->currentIndex());
    qDebug() << "5:" << joint_pos_range(ui->comboBox_5->currentIndex());
    mThread8->gear_ratio[4] = joint_gear_ratio(ui->comboBox_5->currentIndex());
    mThread8->pos_range[4] = joint_pos_range(ui->comboBox_5->currentIndex());
    mThread8->vel_limit[4] = joint_vel_limit(ui->comboBox_5->currentIndex());

    qDebug() << "6:" << joint_gear_ratio(ui->comboBox_6->currentIndex());
    qDebug() << "6:" << joint_pos_range(ui->comboBox_6->currentIndex());
    mThread8->gear_ratio[5] = joint_gear_ratio(ui->comboBox_6->currentIndex());
    mThread8->pos_range[5] = joint_pos_range(ui->comboBox_6->currentIndex());
    mThread8->vel_limit[5] = joint_vel_limit(ui->comboBox_6->currentIndex());
    */

    mThread8->coordinate_type = ui->comboBox_coordinate_type->currentIndex();
    //mThread8->filename = qstrdup(qPrintable(ui->lineEdit_filename->text()));
    mThread8->modify_Z_use = ui->checkBox_modify_zvec->isChecked();
    mThread8->shiftUse = ui->checkBox_shift->isChecked();
    mThread8->middleCoordUse = ui->checkBox_coord5and6->isChecked();
    mThread8->rowW = ui->comboBox_row->currentIndex();
    mThread8->columnL = ui->comboBox_column->currentIndex() + 1;    //+1, beciuse initial number is colume 2

    mThread8->tray_length = ui->lineEdit_tray_length->text().toFloat();
    mThread8->tray_width = ui->lineEdit_tray_width->text().toFloat();
    mThread8->number_L = ui->lineEdit_numberL->text().toInt();
    mThread8->number_W = ui->lineEdit_numberW->text().toInt();

}


int Dialog::cw(int type)    //ini
{

    if(type == 0) //L54W50S290 103860.0
    {
        return 1;
    }
    else if(type == 1)//L54W50S500 180684.0
    {
        return -1;
    }

}

int Dialog::joint_vel_limit(int type)   //ini
{

    if(type == 0) //H54W200S500 8000
    {
        return 17000;
    }
    else if(type == 1) //H54W100S500 8000
    {
        return 17000;
    }
    else if(type == 2) //L54W50S500 8000
    {
        return 8000;
    }
    else if(type == 3)//L54W50S290 8000
    {
        return 8000;
    }
    else if(type == 4)//L54W30S500 8000
    {
        return 8000;
    }
    else if(type == 5)//L54W30S400 8000
    {
        return 8000;
    }
    else if(type == 6)//L42v 8000
    {
        return 400;    //400
    }
    else
    {
        return 0;
    }
}

float Dialog::joint_pos_range(int type) //ini
{

    if(type == 0) //H54W200S500, 250950
    {
        return 251000.0 ;
    }
    else if(type == 1) //H54W100S500, 250950
    {
        return 251000.0 ;
    }
    else if(type == 2)//L54W50S500 180684.0
    {
        return 180684.0;
    }
    else if(type == 3) //L54W50S290 103860.0
    {
        return 103860.0 ;
    }
    else if(type == 4)//L54W30S500 180684.0
    {
        return 180684.0;
    }
    else if(type == 5)//L54W30S400 144180.0
    {
        return 144180.0;
    }
    else if(type == 6)//L42v 2048.0
    {
        return 2048.0;
    }
    else
    {
        return 0;
    }
}

float Dialog::joint_gear_ratio(int type)    //ini
{

    if(type == 0) //H54W200S500
    {
        return 500.0 ;
    }
    else if(type == 1) //H54W100S500
    {
        return 500.0 ;
    }
    else if(type == 2)//L54W50S500 500
    {
        return 500.0;
    }
    else if(type == 3) //L54W50S290 290
    {
        return 290.0 ;
    }
    else if(type == 4)//L54W30S500 500
    {
        return 500.0;
    }
    else if(type == 5)//L54W30S400 400
    {
        return 400.0;
    }
    else if(type == 6)//L42v 0.114
    {
        return 0.114;
    }
    else
    {
        return 0;
    }
}

void Dialog::getnumber(int num) //#@!
{
    ui->lineEdit_current_time->setText(QString::number(num));
}


void Dialog::socket_initialization()    //mythread5 ini
{
    mThread2->step_span = mThread5->time_cell;
    mThread2->omega_limit = mThread5->omega_limit;
    mThread2->move_time = mThread5->move_time;

    qDebug()  << "mThread2->step_span:" << mThread2->step_span;
    qDebug()  << "mThread2->omega_limit:" << mThread2->omega_limit;
    qDebug()  << "mThread2->move_time:" << mThread2->move_time;
    mThread2->vig[0]=mThread5->vig[0];
    mThread2->vig[1]=mThread5->vig[1];
    mThread2->vig[2]=mThread5->vig[2];
    mThread2->vig[3]=mThread5->vig[3];

    mThread2->vpg[0]=mThread5->vpg[0];
    mThread2->vpg[1]=mThread5->vpg[1];
    mThread2->vpg[2]=mThread5->vpg[2];
    mThread2->vpg[3]=mThread5->vpg[3];

    mThread2->ppg[0]=mThread5->ppg[0];
    mThread2->ppg[1]=mThread5->ppg[1];
    mThread2->ppg[2]=mThread5->ppg[2];
    mThread2->ppg[3]=mThread5->ppg[3];

    qDebug()  << "mThread2->vig[0]:" << mThread2->vig[0];
    qDebug()  << "mThread2->vig[1]:" << mThread2->vig[1];
    qDebug()  << "mThread2->vig[2]:" << mThread2->vig[2];
    qDebug()  << "mThread2->vig[3]:" << mThread2->vig[3];

    qDebug()  << "mThread2->vpg[0]:" << mThread2->vpg[0];
    qDebug()  << "mThread2->vpg[1]:" << mThread2->vpg[1];
    qDebug()  << "mThread2->vpg[2]:" << mThread2->vpg[2];
    qDebug()  << "mThread2->vpg[3]:" << mThread2->vpg[3];

    qDebug()  << "mThread2->ppg[0]:" << mThread2->ppg[0];
    qDebug()  << "mThread2->ppg[1]:" << mThread2->ppg[1];
    qDebug()  << "mThread2->ppg[2]:" << mThread2->ppg[2];
    qDebug()  << "mThread2->ppg[3]:" << mThread2->ppg[3];

    mThread4->omega_limit = mThread5->omega_limit;
    mThread4->user_sync_type = mThread5->user_sync_type;
    mThread4->move_time = mThread5->move_time;

    mThread4->unit = mThread5->unit;
    mThread4->angle = mThread5->angle;

    qDebug()  << "mThread4->omega_limit:" << mThread4->omega_limit;
    qDebug()  << "mThread4->user_sync_type:" << mThread4->user_sync_type;
    qDebug()  << "mThread4->move_time:" << mThread4->move_time;
    qDebug()  << "mThread4->unit:" << mThread4->unit;
    qDebug()  << "mThread4->angle:" << mThread4->angle;

    mThread4->ox = mThread5->ox;
    mThread4->oy = mThread5->oy;
    mThread4->oz = mThread5->oz;

    qDebug()  << "ox:" << mThread4->ox;
    qDebug()  << "oy:" << mThread4->oy;
    qDebug()  << "oz:" << mThread4->oz;

    for (int i=0;i<4;i++)
    {
       qDebug() << i;
       qDebug() <<  mThread5->adj_joint[i];
       qDebug() <<  mThread5->gear_ratio[i];
       qDebug() <<  mThread5->pos_range[i];
       qDebug() <<  mThread5->cw[i];

       mThread4->adj_joint[i] = mThread5->adj_joint[i];
       mThread4->gear_ratio[i] = mThread5->gear_ratio[i];
       mThread4->pos_range[i] = mThread5->pos_range[i];
       mThread4->cw[i] = mThread5->cw[i];

       mThread2->gear_ratio[i] = mThread5->gear_ratio[i];
       mThread2->pos_range[i] = mThread5->pos_range[i];
    }

    for (int i=0;i<4;i++)
    {
        qDebug() << "m4 adj:" <<mThread4->adj_joint[i];
    }
    for (int i=0;i<4;i++)
    {
        qDebug() << "m4 gear:" <<mThread4->gear_ratio[i];
    }
    for (int i=0;i<4;i++)
    {
        qDebug() << "m4 pos:" <<mThread4->pos_range[i];
    }
    for (int i=0;i<4;i++)
    {
        qDebug() << "m4 cw:" <<mThread4->cw[i];
    }
    for (int i=0;i<4;i++)
    {
        qDebug() << "m2 gear:" <<mThread2->gear_ratio[i];
    }
    for (int i=0;i<4;i++)
    {
        qDebug() << "m2 pos:" <<mThread2->pos_range[i];
    }

}


void Dialog::modbus_initialization()    //mythread6, mythread7 ini
{
    //mThread6->dev = qstrdup(qPrintable(ui->lineEdit_7->text()));
    mThread6->baudrate = ui->lineEdit_11->text().toInt();
    //mThread6->parity = qstrdup(qPrintable(ui->lineEdit_12->text()));
    mThread6->id = ui->lineEdit_15->text().toInt();

    mThread7->baudrate = ui->lineEdit_11->text().toInt();
    mThread7->id = ui->lineEdit_15->text().toInt();

    mThread6->cell_max_num = ui->lineEdit_16->text().toInt();
    mThread6->tested_tray_num = ui->lineEdit_17->text().toInt();
    mThread6->pass_tray_num = ui->lineEdit_18->text().toInt();
    mThread6->fail_tray_num = ui->lineEdit_19->text().toInt();

}


void Dialog::plain_end()
{
    QTextCursor c = ui->plainTextEdit->textCursor();
    c.movePosition(QTextCursor::End);
    ui->plainTextEdit->setTextCursor(c);
}



void Dialog::onNumberChanged(int Number)
{
    //qDebug()  << Number;
    //ui->label->setText(QString::number(Number));

    if(Number == 0)
    {
        QString s = QString("Fail to recv\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 1)
    {
        QString s = QString("Recv success\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }

    plain_end();
}

void Dialog::onNumberChanged2(int Number)
{
    //ui->label->setText(QString::number(Number));

    if(Number == -1)
    {
        QString s = QString("Fail to open USB2Dyanmixel!\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if (Number == 0)
    {
        QString s = QString("Fail to modify gain value\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 1)
    {
        QString s = QString("Success\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 2)
    {
        QString s = QString("Success to torque off\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 3)
    {
        QString s = QString("Fail to torque off\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 4)
    {
        QString s = QString("Success to torque on\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 5)
    {
        QString s = QString("Fail to torque on\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 6)
    {
        QString s = QString("Success to reboot\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 7)
    {
        QString s = QString("Fail to reboot\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 8)
    {
        QString s = QString("Velocity over limit\n");
        ui->plainTextEdit->textCursor().insertText(s);

        QMessageBox message(QMessageBox::Warning, "Information", "Velocity Over Limit", QMessageBox::Ok,NULL);
        message.exec();
    }
    else if(Number == 9)
    {
        QString s = QString("Read Temperature Fail!\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 10)
    {
        QString s = QString("Position over limit\n");
        ui->plainTextEdit->textCursor().insertText(s);

        QMessageBox message(QMessageBox::Warning, "Information", "Position Over Limit", QMessageBox::Ok,NULL);
        message.exec();
    }
    plain_end();
}

void Dialog::onNumberChanged3(int Number)
{
    //ui->label->setText(QString::number(Number));

    if(Number == 0)
    {
        QString s = QString("Fail to send (code:1)\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 1)
    {
        QString s = QString("Send success\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 2)
    {
        QString s = QString("Fail to send (code:2)\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 3)
    {
        QString s = QString("Fail to send (code:3)\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    if(Number == 4)
    {
        QString s = QString("Fail to send (code:4)\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    plain_end();
}

void Dialog::onNumberChanged4(int Number)
{
   // qDebug()  << "this is number:"<< Number;
    //ui->label->setText(QString::number(Number));

    if(Number == -1)
    {
        QString s = "( "+QString::number(mThread4->coord[0])+","+QString::number(mThread4->coord[1])+","+QString::number(mThread4->coord[2])+" )\n";
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 0)
    {
        QString s = QString("exit!\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 1)
    {
        QString s = QString("W\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 2)
    {
        QString s = QString("S\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 3)
    {
        QString s = QString("A\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 4)
    {
        QString s = QString("D\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 5)
    {
        QString s = QString("R\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 6)
    {
        QString s = QString("F\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 7)
    {
        QString s = QString("W\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 8)
    {
        QString s = QString("S\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 9)
    {
        QString s = QString("A\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 10)
    {
        QString s = QString("D\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 11)
    {
        QString s = QString("R\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 12)
    {
        QString s = QString("F\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 13)
    {
        QString s = QString("1\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 14)
    {
        QString s = QString("2\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 15)
    {
        QString s = QString("3\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 16)
    {
        QString s = QString("4\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
    else if(Number == 17)
    {
        QString s = QString("The Height Over Limit\n");
        ui->plainTextEdit->textCursor().insertText(s);

        QMessageBox message(QMessageBox::Warning, "Information", "The Height Over Limit", QMessageBox::Ok,NULL);
        message.exec();
    }
    else if(Number == 18)
    {
        QString s = QString("The Distance Over the Minimum Limit\n");
        ui->plainTextEdit->textCursor().insertText(s);

        QMessageBox message(QMessageBox::Warning, "Information", "The Distance Over the Minimum Limit", QMessageBox::Ok,NULL);
        message.exec();
    }
    else if(Number == 19)
    {
        QString s = QString("Caution!\n");
        ui->plainTextEdit->textCursor().insertText(s);

        QMessageBox message(QMessageBox::Warning, "Information", "Caution!", QMessageBox::Ok,NULL);
        message.exec();
    }

    plain_end();
}

void Dialog::onNumberChanged5(int Number)
{
    if(Number == 0)
    {
        mThread5->Check= true;
        mThread5->Exit = false;
    }
    else if(Number == -1)
    {
        QString s1 = QString("Port inuse\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == -2) //bind success
    {
        qDebug() << "Socket successfully created";
        QString s1 = QString("Socket successfully created\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == -3) //quit
    {
        qDebug() << "Socket successfully closed";
        QString s1 = QString("Socket successfully closed\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }

    else if(Number == 1) //coordinatestep
    {
        IKFK_parameter(mThread5->option[0],mThread5->type,true);
        mThread4->start();

        while(1)
        {
            if(mThread4->isFinished())
            {

                QString s = "done\tmove:"+QString(mThread5->option)+" unit:"+QString::number(mThread5->unit)+"\n\t";
                mThread5->message=qstrdup(qPrintable(s));
                qDebug() << "message:" <<mThread5->message;
                mThread5->Check = true;
                mThread5->Exit = false;
                break;
            }
        }
    }
    else if(Number == 2) //play
    {
        mThread2->filename="./readpos.txt";
        mThread2->times=1;

        mThread2->cmd=qstrdup(qPrintable(QString("syncw")));
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
                QString s1 = QString("Move..\n");
                QString s = QString("done\tMove..\n");
                ui->plainTextEdit->textCursor().insertText(s1);

                char buffer[256] = "path\t";
                char temp[256];
                std::string filename("readpos.txt");
                std::ifstream in(filename.c_str());

                in.getline (temp,100);
                strcat(buffer,temp);

                mThread5->message=qstrdup(qPrintable(buffer));
                qDebug() << "message:" <<mThread5->message;
                plain_end();
                break;
            }
        }
    }
    else if(Number == 3) //singlejoint
    {
        IKFK_parameter(mThread5->option[0],mThread5->type,true);
        mThread4->start();

        while(1)
        {
            if(mThread4->isFinished())
            {
                //QString s = "done\t( "+QString::number(mThread4->coord[0])+","+QString::number(mThread4->coord[1])+","+QString::number(mThread4->coord[2])+" )\t";

                QString s = "done\tjoint:"+QString(mThread5->option)+" type:"+QString::number(mThread5->type)+" angle:"+QString::number(mThread5->angle)+"\n\t";
                mThread5->message=qstrdup(qPrintable(s));
                qDebug() << "message:" <<mThread5->message;
                mThread5->Check = true;
                mThread5->Exit = false;
                break;
            }
        }
    }
    else if (Number == 4) //coordinatevalue
    {
        IKFK_parameter('t',mThread5->type,true);

        mThread4->coord[0] = mThread5->coord[0];
        mThread4->coord[1] = mThread5->coord[1];
        mThread4->coord[2] = mThread5->coord[2];
        mThread4->start();

        while(1)
        {
            if(mThread4->isFinished())
            {

                QString s = "done\t( "+QString::number(mThread4->coord[0])+","+QString::number(mThread4->coord[1])+","+QString::number(mThread4->coord[2])+" )\n\t";
                mThread5->message=qstrdup(qPrintable(s));
                qDebug() << "message:" <<mThread5->message;
                mThread5->Check = true;
                mThread5->Exit = false;
                break;
            }
        }

    }
    else if (Number == 5) //socket_set_path
    {

        std::fstream fst;

        if(mThread5->file_start == 0)
        {
            fst.open(mThread5->filename, ios::out);
        }
        else if (mThread5->file_start == 1)
        {
            fst.open(mThread5->filename, ios::out|ios::app);
        }

        char *delim = "\t";
        char * pch;

        pch = strtok(mThread5->path_message,delim);

        while(pch != NULL)
        {
            qDebug() << "*************************************************************++++++++\n";
            qDebug() << pch;
            fst << pch << "\t";
            pch = strtok (NULL, delim);
            sleep(0.5);
        }

        fst << "\n";

        mThread2->times=1;
        mThread2->filename = mThread5->filename;

        QString s = "done\tupload done\n\t";
        qDebug() << s;
        mThread5->message=qstrdup(qPrintable(s));
        //qDebug() <<"+_+_+_+__+\n";


    }
    else if (Number == 6) //pumpon
    {
        call_suck_on(relayport);
        QString s = "done\tpumpon\n\t";
        mThread5->message=qstrdup(qPrintable(s));
    }
    else if (Number == 7) //pumpoff
    {
        call_suck_off(relayport);
        QString s = "done\tpumpoff\n\t";
        mThread5->message=qstrdup(qPrintable(s));
    }
    else if(Number == 8) //load file and play
    {
            mThread2->filename = "upload_readpos.txt";
            mThread2->cmd=qstrdup(qPrintable(QString("syncw")));
            qDebug()  << "CMD:" << mThread2->cmd;

            QString s1 = QString("Start\n");
            ui->plainTextEdit->textCursor().insertText(s1);
            mThread2->start();
    }
    else if(Number == 9) //toque on
    {
        mThread2->cmd=qstrdup(qPrintable(QString("tonall")));
        qDebug()  << "CMD:" << mThread2->cmd;
        mThread2->start();
        QString s = "done\ttoque on\n\t";
        mThread5->message=qstrdup(qPrintable(s));
    }
    else if(Number == 10) //toque off
    {
        mThread2->cmd=qstrdup(qPrintable(QString("toffall")));
        qDebug()  << "CMD:" << mThread2->cmd;
        mThread2->start();
        QString s = "done\ttoque off\n\t";
        mThread5->message=qstrdup(qPrintable(s));
    }
    else if(Number == 11) //group play
    {
        mThread2->cmd=qstrdup(qPrintable(QString("syncw_group")));
        mThread2->group = mThread5->group;

        QString s1 = QString("Start\n");
        ui->plainTextEdit->textCursor().insertText(s1);
        mThread2->stop=false;   //#@!
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
                char buffer[256] = "path\tmove\n\t\0";
                char buffer2[256] = "path\tfail\n\t\0";

                if(mThread2->state == 1)
                {
                    mThread5->message=qstrdup(qPrintable(buffer));
                    QString s1 = QString("Move..\n");
                    ui->plainTextEdit->textCursor().insertText(s1);
                }
                else
                {
                    mThread5->message=qstrdup(qPrintable(buffer2));
                    QString s2 = QString("Fail..\n");
                    ui->plainTextEdit->textCursor().insertText(s2);
                }
                qDebug() << "message:" <<mThread5->message;
                plain_end();
                break;
            }
        }
    }
    else if(Number == 12) //teach
    {
        qDebug() << "in #12 1";
        socket_initialization();
        //ini_IKFK();
        qDebug() << "in #12 2";

        if(!mThread4->isRunning())
        {
            mThread4->start();

        }
        else
        {
            QString s = QString("is running\n");
            ui->plainTextEdit->textCursor().insertText(s);
        }
        qDebug() << "in #12 3";
        IKFK_parameter('0',0,true);

        QString s = "done\tRunning\n\t";
        mThread5->message=qstrdup(qPrintable(s));

        qDebug() << "in #12 4";
    }
    else if (Number == 13) //initialization
    {
        socket_initialization();

        mThread2->cmd=qstrdup(qPrintable(QString("init")));
        qDebug()  << "CMD:" << mThread2->cmd;
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
                QString s = "done\tinitialization\n\t";
                mThread5->message=qstrdup(qPrintable(s));
                break;
            }
        }
    }
    else if (Number == 14) //reboot_all
    {
        mThread2->cmd = mThread5->case_option;
        qDebug()  << "CMD:" << mThread2->cmd;
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
                if(mThread2->state == 1)
                {
                    QString s = "done\treboot sucess\n\t";
                    mThread5->message=qstrdup(qPrintable(s));
                }
                else
                {
                    QString s = "done\treboot fail\n\t";
                    mThread5->message=qstrdup(qPrintable(s));
                }

                break;
            }
        }
    }
    else if (Number == 15) //set_path_group
    {
        mThread2->times=1;
        mThread2->filename = mThread5->filename;

        mThread2->cmd=qstrdup(qPrintable(QString("read_group")));
        qDebug()  << "CMD:" << mThread2->cmd;
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
                QString s = "done\tdone\n\t";
                mThread5->message=qstrdup(qPrintable(s));
                qDebug() << "message:" <<mThread5->message;
                break;
            }
        }
    }
    else if (Number == 16) //optimize_group
    {
        mThread2->times=1;
        mThread2->filename = mThread5->filename;

        mThread2->cmd=qstrdup(qPrintable(QString("optimize2")));
        qDebug()  << "CMD:" << mThread2->cmd;
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
                QString s = "done\toptimize\n\t";
                mThread5->message=qstrdup(qPrintable(s));
                qDebug() << "message:" <<mThread5->message;
                break;
            }
        }
    }
    else if (Number == 17) //set_path_group
    {
        char filename[50] = "./";
        char str[10] = ".txt";

        strcat(filename, mThread5->id);
        strcat(filename, str);
        std::ifstream in(filename);
        char buffer[256];

        while (!in.eof() )
        {
            in.getline (buffer,100);

            char *delim = "\t";
            char * pch;
            pch = strtok(buffer,delim);

            if (pch != NULL)
            {
                qDebug() <<"sub:" <<pch;

                QString s = "done\t"+QString(pch)+"\n";
                mThread5->message=qstrdup(qPrintable(s));
                qDebug() << "message:" <<mThread5->message;

                pch = strtok (NULL, delim);

            }
            break;
        }
    }
    else if (Number == 18) //dynamic_syncw_group
    {
        IKFK_parameter('t',mThread5->type,true);

        mThread4->coord[0] = mThread5->coord[0];
        mThread4->coord[1] = mThread5->coord[1];
        mThread4->coord[2] = mThread5->coord[2];
        mThread4->start();

        while(1)
        {
            if(mThread4->isFinished())
            {
                break;
            }
        }

//        mThread2->change_step_now = mThread5->change_step_now;
//        mThread2->change_step_pre = mThread5->change_step_pre;

        mThread2->isnewvalue = 1;
        mThread2->change_step_now = 0;
        mThread2->change_step_pre = 7;
        mThread2->change_group = mThread5->change_group;

        mThread2->change_pos[0] = mThread4->theta2value[0];
        mThread2->change_pos[1] = mThread4->theta2value[1];
        mThread2->change_pos[2] = mThread4->theta2value[2];
        mThread2->change_pos[3] = mThread4->theta2value[3];

        qDebug()  << "change";
        qDebug()  << mThread2->change_pos[0];
        qDebug()  << mThread2->change_pos[1];
        qDebug()  << mThread2->change_pos[2];
        qDebug()  << mThread2->change_pos[3];

        mThread2->cmd=qstrdup(qPrintable(QString("dynamic_syncw_group")));

        QString s1 = QString("Start\n");
        ui->plainTextEdit->textCursor().insertText(s1);
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
                break;
            }
        }
        //-----------------------------------------------------------------------------------------------
        IKFK_parameter('t',mThread5->type,true);

        mThread4->coord[0] = mThread5->coord[0];
        mThread4->coord[1] = mThread5->coord[1];
        mThread4->coord[2] = mThread5->coord[2]+9.7;
        mThread4->start();

        while(1)
        {
            if(mThread4->isFinished())
            {
                break;
            }
        }

        mThread2->isnewvalue = 1;
        mThread2->change_step_now = 2;
        mThread2->change_step_pre = 0;
        mThread2->change_group = mThread5->change_group;

        mThread2->change_pos[0] = mThread4->theta2value[0];
        mThread2->change_pos[1] = mThread4->theta2value[1];
        mThread2->change_pos[2] = mThread4->theta2value[2];
        mThread2->change_pos[3] = mThread4->theta2value[3];

        qDebug()  << "change";
        qDebug()  << mThread2->change_pos[0];
        qDebug()  << mThread2->change_pos[1];
        qDebug()  << mThread2->change_pos[2];
        qDebug()  << mThread2->change_pos[3];

        mThread2->cmd=qstrdup(qPrintable(QString("dynamic_syncw_group")));
        qDebug()  << "CMD:" << mThread2->cmd;

//        QString s1 = QString("Start\n");
//        ui->plainTextEdit->textCursor().insertText(s1);
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
//                QString s = "done\tdynamic syncw group\n\t";
//                mThread5->message=qstrdup(qPrintable(s));
//                qDebug() << "message:" <<mThread5->message;
                break;
            }
        }
        //-----------------------------------------------------------------------------------------------
        mThread2->isnewvalue = 0;
        mThread2->change_step_now = 3;
        mThread2->change_step_pre = 2;
        mThread2->change_group = mThread5->change_group;

        mThread2->change_pos[0] = 999999;
        mThread2->change_pos[1] = 999999;
        mThread2->change_pos[2] = 999999;
        mThread2->change_pos[3] = 999999;

        qDebug()  << "change";
        qDebug()  << mThread2->change_pos[0];
        qDebug()  << mThread2->change_pos[1];
        qDebug()  << mThread2->change_pos[2];
        qDebug()  << mThread2->change_pos[3];

        mThread2->cmd=qstrdup(qPrintable(QString("dynamic_syncw_group")));
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
                break;
            }
        }

        //-----------------------------------------------------------------------------------------------
        IKFK_parameter('t',mThread5->type,true);

        mThread4->coord[0] = mThread5->coord[0];
        mThread4->coord[1] = mThread5->coord[1];
        mThread4->coord[2] = mThread5->coord[2]+9.7;
        mThread4->start();

        while(1)
        {
            if(mThread4->isFinished())
            {
                break;
            }
        }

        mThread2->isnewvalue = 1;
        mThread2->change_step_now = 7;
        mThread2->change_step_pre = 6;
        mThread2->change_group = mThread5->change_group;

        mThread2->change_pos[0] = mThread4->theta2value[0];
        mThread2->change_pos[1] = mThread4->theta2value[1];
        mThread2->change_pos[2] = mThread4->theta2value[2];
        mThread2->change_pos[3] = mThread4->theta2value[3];
        qDebug()  << "change";
        qDebug()  << mThread2->change_pos[0];
        qDebug()  << mThread2->change_pos[1];
        qDebug()  << mThread2->change_pos[2];
        qDebug()  << mThread2->change_pos[3];

        mThread2->cmd=qstrdup(qPrintable(QString("dynamic_syncw_group")));
        qDebug()  << "CMD:" << mThread2->cmd;

//        QString s1 = QString("Start\n");
//        ui->plainTextEdit->textCursor().insertText(s1);
        mThread2->start();

        while(1)
        {
            if(mThread2->isFinished())
            {
                QString s = "done\tdynamic syncw group\n\t";
                mThread5->message=qstrdup(qPrintable(s));
                qDebug() << "message:" <<mThread5->message;
                break;
            }
        }
        //-----------------------------------------------------------------------------------------------
    }
    //sleep(3);

    if(Number >= 0)
    {
        while(1)
        {
            if(mThread5->Check == false){break;}
        }
    }

    mThread5->Check = true;
    mThread5->Exit = false;
    qDebug() << "numberChange5 out:";
}

void Dialog::onNumberChanged6(int Number)
{
    if(Number == 0)
    {
        ;
    }
    else if(Number == -1)
    {
        qDebug() << "Failed to allocate the mapping";
        QString s1 = QString("Failed to allocate the mapping\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == -2)
    {
        qDebug() << "Modbus connexion failed";
        QString s1 = QString("Modbus connexion failed\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == -3)
    {
        qDebug() << "Modbus connexion successfully";
        QString s1 = QString("Modbus connexion successfully\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == 1)
    {
        ui->lineEdit_allcount->setText(QString::number(mThread6->allcount));
        mThread2->cmd=qstrdup(qPrintable(QString("syncw_group")));
        mThread2->group = mThread6->group;
        mThread2->allcount2 = mThread6->allcount;
        bool warnning = false;

        QString s = QString("Start\n");
        ui->plainTextEdit->textCursor().insertText(s);

        mThread6->check = false;
        mThread2->stop = false;   //#@!
        mThread2->gpio = false;
        mThread2->start();

        //left absorb,right deflate
        while(mThread6->Exit == false && warnning == false)
        {
            if(mThread2->gpio==true)
            {
                mThread2->gpio=false;
                mThread6->mode_gpio=1;

                //QString s4 = QString("Modbus: Dialog receive GPIO True!\n");
                //ui->plainTextEdit->textCursor().insertText(s4);
                //delay(100);
                //cout<<"in dialog bofore 6 done!!!!!!!!!!!!!!!!\n";
                while(mThread6->Exit == false && warnning == false)
                {
                    cout<<"in dialog before 6 is done222222222222222\n";
                    //wait(0);
                    if(mThread6->done == 1)
                    {
                        //QString s5 = QString("Modbus: PLC motion Finished!\n");
                        //ui->plainTextEdit->textCursor().insertText(s5);
                        mThread6->done = 0;
                        mThread2->done2 = 1;
                        break;
                    }
                    else if(mThread6->done == 2)
                    {
                        mThread6->done = 0;
                        mThread2->done2 = 2;
                        mThread2->stop = true;
                        warnning = true;
                        break;
                    }
                }
                break;
            }

        }

        while(mThread6->Exit == false && warnning == false)
        {
            if(mThread2->isFinished())
            {
                mThread6->check = true;

                if(mThread2->state == 1)
                {
                    QString s1 = QString("Move Finished!\n");
                    ui->plainTextEdit->textCursor().insertText(s1);
                }
                else
                {
                    QString s2 = QString("Move Fail..\n");
                    ui->plainTextEdit->textCursor().insertText(s2);
                }

                plain_end();
                break;
            }
        }
    }
    else if(Number == 2)
    {
        mThread2->cmd=qstrdup(qPrintable(QString("syncw_group")));
        mThread2->group = mThread6->group;
        bool warnning=false;

        QString s = QString("Start\n");
        ui->plainTextEdit->textCursor().insertText(s);

        mThread6->check = false;
        mThread2->stop = false;   //#@!
        mThread2->gpio = false;
        mThread2->scan_ready = false;
        mThread2->start();

        //right absorb
        while(mThread6->Exit == false && warnning == false)
        {
            if(mThread2->gpio == true)
            {
                //QString s5 = QString("Modbus: Dialog receive GPIO True! mode2\n");
                //ui->plainTextEdit->textCursor().insertText(s5);

                mThread2->gpio=false;
                mThread6->mode_gpio=2;
                while(mThread6->Exit == false && warnning == false)
                {
                    cout<<"wait for together action is done!!"<<endl;
                    if(mThread6->done == 1)
                    {
                        //QString s7 = QString("Modbus: PLC motion Finished! mode2\n");
                        //ui->plainTextEdit->textCursor().insertText(s7);

                        mThread6->done = 0;
                        mThread2->done2 = 1;
                        break;
                    }
                    else if(mThread6->done == 2)
                    {
                        cout<<"dialog together is warnning!!!!!!!!!!!"<<endl;
                        warnning = true;
                        mThread2->done2 = 2;
                        mThread6->done = 0;
                        mThread2->stop = true;
                    }
                }
                break;
            }
        }

        //left deflate
        /*while(mThread6->Exit == false)
        {
            if(mThread2->gpio == true)
            {
                //QString s8 = QString("Modbus: Dialog receive GPIO True! mode3\n");
                //ui->plainTextEdit->textCursor().insertText(s8);
                mThread2->gpio=false;
                mThread6->mode_gpio=3;
                while(mThread6->Exit == false)
                {
                    wait(0);
                    if(mThread6->done == true)
                    {
                        //QString s9 = QString("Modbus: PLC motion Finished! mode3\n");
                        //ui->plainTextEdit->textCursor().insertText(s9);
                        mThread6->done = false;
                        mThread2->done2 = true;
                        break;
                    }
                }
                break;
            }
        }*/

        //Scan ready
        while(mThread6->Exit == false && warnning !=true)
        {
            //wait(0);
            cout<<"wait for scan ready is done!!"<<endl;
            ui->pte_console->appendPlainText("scan_ready=" + QString::number(mThread2->scan_ready) +
                                             "\t" + QTime::currentTime().toString());
            if(mThread2->scan_ready == true)
            {
                mThread6->scan_ready = true;
                QString s1 = QString("Modbus: send messgae *scan ready* to PLC\n");
                ui->plainTextEdit->textCursor().insertText(s1);
                plain_end();
                break;
            }
        }

        //Scan done
        while(mThread6->Exit == false && warnning != true)
        {
            cout<<"scan done before 6scandone is true\n";
            //wait(0);
            if(mThread6->Scandone_check == 1)
            {
                //cout<<"scan done in 6scandone is true\n";
                mThread2->Scandone_check = true;
                QString s2 = QString("Modbus: receive message *scan done* from PLC\n");
                ui->plainTextEdit->textCursor().insertText(s2);
                plain_end();
                break;
            }
            else if(mThread6->Scandone_check == 2)
            {
                warnning = true;
                mThread2->Scandone_check = 2;
                mThread6->Scandone_check = 0;
            }
        }

        while(mThread6->Exit == false && warnning != true)
        {
            if(mThread2->isFinished())
            {
                mThread6->check = true;
                if(mThread2->state == 1)
                {
                    QString s3 = QString("Modbus: Move Finished!\n");
                    ui->plainTextEdit->textCursor().insertText(s3);
                }
                else
                {
                    QString s4 = QString("Modbus: Move Fail..\n");
                    ui->plainTextEdit->textCursor().insertText(s4);
                }
                plain_end();
                break;
            }
        }
    }


}

void Dialog::onNumberChanged7(int Number)
{
    if(Number == 0)
    {
        ;
    }
    else if(Number == -1)
    {
        qDebug() << "Failed to allocate the mapping";
        QString s1 = QString("Failed to allocate the mapping\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == -2)
    {
        qDebug() << "Modbus connexion failed";
        QString s1 = QString("Modbus connexion failed\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == -3)
    {
        qDebug() << "Modbus connexion successfully";
        QString s1 = QString("Modbus connexion successfully\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }

}

void Dialog::onNumberChanged8(int Number)
{
    if(Number == 0)
    {
        qDebug() << "Mythread8 NONE";
        QString s1 = QString("Mythread8 NONE\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == 1)
    {
        qDebug() << "Generate OK";
        QString s1 = QString("Generate OK\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == 2)
    {
        qDebug() << "Modify the normal vector";
        QString s1 = QString("Modify the normal vector\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(Number == 5)
    {
        //qDebug() << "OPEN ERROR: shift failed!";
        QString s1 = QString("OPEN ERROR: shift failed!\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }


}


//void Dialog::on_pushButton_1_clicked()
//{
//    //mThread->host=qstrdup(qPrintable(ui->lineEdit_1->text()));
//    //mThread->port=qstrdup(qPrintable(ui->lineEdit_2->text()));
//    mThread->start();
//}

void Dialog::on_pushButton_initialize_clicked() //Initialize
{
    arm_init();

    mThread2->cmd=qstrdup(qPrintable(QString("init")));
    qDebug()  << "CMD:" << mThread2->cmd;

    mThread2->start();
}

void Dialog::on_pushButton_6_clicked()  //Save settings
{
    QFileDialog myFileDialog (this);

    QString itemPath = myFileDialog.getSaveFileName(this, tr ("Save File"), QDir::currentPath ());
    if (itemPath.isEmpty ())
    {
      ;
    }
    else
    {

      std::fstream fst;
      fst.open(itemPath.toStdString().c_str(), ios::out);

      fst << ui->lineEdit_step_span->text().toStdString()<<endl;
      fst << ui->lineEdit_4->text().toStdString()<<endl;
      fst << ui->lineEdit_5->text().toStdString()<<endl;
      fst << ui->lineEdit->text().toStdString()<<endl;
      fst << ui->lineEdit_2->text().toStdString()<<endl;
      fst << ui->lineEdit_vig_1->text().toStdString()<<endl;
      fst << ui->lineEdit_vig_2->text().toStdString()<<endl;
      fst << ui->lineEdit_vig_3->text().toStdString()<<endl;
      fst << ui->lineEdit_vig_4->text().toStdString()<<endl;
      fst << ui->lineEdit_vpg_1->text().toStdString()<<endl;
      fst << ui->lineEdit_vpg_2->text().toStdString()<<endl;
      fst << ui->lineEdit_vpg_3->text().toStdString()<<endl;
      fst << ui->lineEdit_vpg_4->text().toStdString()<<endl;
      fst << ui->lineEdit_ppg_1->text().toStdString()<<endl;
      fst << ui->lineEdit_ppg_2->text().toStdString()<<endl;
      fst << ui->lineEdit_ppg_3->text().toStdString()<<endl;
      fst << ui->lineEdit_ppg_4->text().toStdString()<<endl;
      fst << ui->lineEdit_ox->text().toStdString()<<endl;
      fst << ui->lineEdit_oy->text().toStdString()<<endl;
      fst << ui->lineEdit_oz->text().toStdString()<<endl;
      fst << ui->lineEdit_14->text().toStdString()<<endl;
      fst << ui->lineEdit_angle->text().toStdString()<<endl;
      fst << ui->lineEdit_6->text().toStdString()<<endl;
      fst << ui->checkBox->isChecked()<<endl;
      fst << ui->checkBox_2->isChecked()<<endl;
      fst << ui->checkBox_3->isChecked()<<endl;
      fst << ui->checkBox_4->isChecked()<<endl;
      fst << ui->comboBox->currentIndex()<<endl;
      fst << ui->comboBox_2->currentIndex()<<endl;
      fst << ui->comboBox_3->currentIndex()<<endl;
      fst << ui->comboBox_4->currentIndex()<<endl;
      fst << ui->comboBox_cw1->currentIndex()<<endl;
      fst << ui->comboBox_cw2->currentIndex()<<endl;
      fst << ui->comboBox_cw3->currentIndex()<<endl;
      fst << ui->comboBox_cw4->currentIndex()<<endl;

      fst << ui->lineEdit_adj_joint1->text().toStdString()<<endl;
      fst << ui->lineEdit_adj_joint2->text().toStdString()<<endl;
      fst << ui->lineEdit_adj_joint3->text().toStdString()<<endl;
      fst << ui->lineEdit_adj_joint4->text().toStdString()<<endl;

      fst << ui->comboBox_posture->currentIndex()<<endl;
      fst << ui->lineEdit_20->text().toStdString()<<endl;

      fst.close();

    }
}

void Dialog::setconfig (QString itemPath)   //Set from file, func
{

    std::string filename(itemPath.toStdString());
    std::ifstream in(filename.c_str());
    char buffer[256];
    if(in.is_open()){
        int line=0;
        while (!in.eof() )
        {
            in.getline (buffer,100);

            if(line == 0){ui->lineEdit_step_span->setText(QString(buffer));}
            else if(line == 1){ui->lineEdit_4->setText(QString(buffer));}
            else if(line == 2){ui->lineEdit_5->setText(QString(buffer));}
            else if(line == 3){ui->lineEdit->setText(QString(buffer));}
            else if(line == 4){ui->lineEdit_2->setText(QString(buffer));}
            else if(line == 5){ui->lineEdit_vig_1->setText(QString(buffer));}
            else if(line == 6){ui->lineEdit_vig_2->setText(QString(buffer));}
            else if(line == 7){ui->lineEdit_vig_3->setText(QString(buffer));}
            else if(line == 8){ui->lineEdit_vig_4->setText(QString(buffer));}

            else if(line == 9){ui->lineEdit_vpg_1->setText(QString(buffer));}
            else if(line == 10){ui->lineEdit_vpg_2->setText(QString(buffer));}
            else if(line == 11){ui->lineEdit_vpg_3->setText(QString(buffer));}
            else if(line == 12){ui->lineEdit_vpg_4->setText(QString(buffer));}

            else if(line == 13){ui->lineEdit_ppg_1->setText(QString(buffer));}
            else if(line == 14){ui->lineEdit_ppg_2->setText(QString(buffer));}
            else if(line == 15){ui->lineEdit_ppg_3->setText(QString(buffer));}
            else if(line == 16){ui->lineEdit_ppg_4->setText(QString(buffer));}

            else if(line == 17){ui->lineEdit_ox->setText(QString(buffer));}
            else if(line == 18){ui->lineEdit_oy->setText(QString(buffer));}
            else if(line == 19){ui->lineEdit_oz->setText(QString(buffer));}
            else if(line == 20){ui->lineEdit_14->setText(QString(buffer));}
            else if(line == 21){ui->lineEdit_angle->setText(QString(buffer));}
            else if(line == 22){ui->lineEdit_6->setText(QString(buffer));}
            else if(line == 23){ui->checkBox->setChecked(QString(buffer).toInt());}
            else if(line == 24){ui->checkBox_2->setChecked(QString(buffer).toInt());}
            else if(line == 25){ui->checkBox_3->setChecked(QString(buffer).toInt());}
            else if(line == 26){ui->checkBox_4->setChecked(QString(buffer).toInt());}

            else if(line == 27){ui->comboBox->setCurrentIndex(QString(buffer).toInt());}
            else if(line == 28){ui->comboBox_2->setCurrentIndex(QString(buffer).toInt());}
            else if(line == 29){ui->comboBox_3->setCurrentIndex(QString(buffer).toInt());}
            else if(line == 30){ui->comboBox_4->setCurrentIndex(QString(buffer).toInt());}

            else if(line == 31){ui->comboBox_cw1->setCurrentIndex(QString(buffer).toInt());}
            else if(line == 32){ui->comboBox_cw2->setCurrentIndex(QString(buffer).toInt());}
            else if(line == 33){ui->comboBox_cw3->setCurrentIndex(QString(buffer).toInt());}
            else if(line == 34){ui->comboBox_cw4->setCurrentIndex(QString(buffer).toInt());}

            else if(line == 35){ui->lineEdit_adj_joint1->setText(QString(buffer));}
            else if(line == 36){ui->lineEdit_adj_joint2->setText(QString(buffer));}
            else if(line == 37){ui->lineEdit_adj_joint3->setText(QString(buffer));}
            else if(line == 38){ui->lineEdit_adj_joint4->setText(QString(buffer));}

            else if(line == 39){ui->comboBox_posture->setCurrentIndex(QString(buffer).toInt());}
            else if(line == 40){ui->lineEdit_20->setText(QString(buffer));}


            line+=1;
        }

        if(line >= 39)
        {
            QString text = "Setting from file:"+itemPath+"\n";
            ui->lineEdit_3->setText(QString(itemPath));
            ui->plainTextEdit->textCursor().insertText(text);
            plain_end();
        }
    }

}

void Dialog::browse_config() //Set frome file, func
{
    QFileDialog myFileDialog (this);
    QString itemPath = myFileDialog.getOpenFileName (this, tr ("Open File"), QDir::currentPath ());
    if (itemPath.isEmpty ())
    {
      ;
    }
    else
    {
        setconfig (itemPath);
    }
}

void Dialog::on_pushButton_5_clicked()  //Set from file
{
    browse_config();
}




//Control
void Dialog::on_pushButton_49_clicked() //Calculate
{
    //qDebug() << "true1";

    ini_IKFK();
    mThread4->C = 't';
    //mThread4->type=2;
    mThread4->coord[0] = ui->lineEdit_8->text().toDouble();
    mThread4->coord[1] = ui->lineEdit_9->text().toDouble();
    mThread4->coord[2] = ui->lineEdit_10->text().toDouble();
    mThread4->Check = true;
    mThread4->start();
}

void Dialog::on_pushButton_27_clicked() //Play
{
    while(1)
    {
        if(mThread4->isFinished())
        {
            cout<<"before 2 is start,4 is finished\n";
            arm_init();
            mThread2->filename="./readpos.txt";
            mThread2->times = 1;
            mThread2->state = 0;
            mThread2->cmd=qstrdup(qPrintable(QString("syncw")));
            mThread2->start();
            break;
        }
    }

    while(1)
    {
        if(mThread2->isFinished())
        {
            sleep(1);

            std::string filename("readpos.txt");
            std::ifstream in(filename.c_str());
            char buffer[256];

            //int row=ui->tableWidget->rowCount();

            int row;
            if(ui->tableWidget->currentRow() == -1)
            {
                row = ui->tableWidget->rowCount();
                //qDebug()  << "no select" << row ;
            }
            else
            {
                row=ui->tableWidget->currentRow()+1;
            }
            //cout<< "row+:" <<endl;
           // qDebug() <<"row:" <<row;

            //ui->tableWidget->setRowCount(line+row-1);

            while (!in.eof() )
            {
                in.getline (buffer,100);

                char *delim = "\t";
                char * pch;
                pch = strtok(buffer,delim);
                //qDebug()  << buffer;

                int col=0;
                if(pch != NULL)
                {
                    ui->tableWidget->insertRow(row);
                }
                qDebug() <<"+++:" <<pch;
                while (pch != NULL)
                {
                    qDebug() <<"sub:" <<pch;
                    qDebug() << "Q:" << QString(pch) << "col:" <<col;
                    //ui->tableWidget->setItem(row,col,new QTableWidgetItem(pch));
                    //ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString("0")));
                    ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString(pch)));
                    pch = strtok (NULL, delim);
                    col+=1;
                }

                //if(c==1){ui->lineEdit_1->setText(QString(buffer));}
                row++;
            }
            //qDebug()  << "end:";
            ui->tableWidget->setCurrentCell(-1,-1);

            QString s1 = QString("Move..\n");
            ui->plainTextEdit->textCursor().insertText(s1);

            break;
        }
    }
}

void Dialog::on_pushButton_mythread2_stop_clicked()
{
     mThread2->stop = true;
}


void Dialog::IKFK_parameter(char C,int type, bool Check)
{

    mThread2->cmd=qstrdup(qPrintable(QString("readpos")));
    qDebug()  << "CMD:" << mThread2->cmd;
    mThread2->start();

    while(1)
    {
        if(mThread2->isFinished())
        {
            break;
        }
    }

    if(mThread2->state==1)
    {
       sleep(1);
       mThread4->C = C;
       mThread4->type=type;
       mThread4->Check = true;
       mThread4->start();
    }
    else
    {
        cout << "IKFK_parameter: readpos fail\n";
    }

}

void Dialog::on_pushButton_41_clicked() //+x
{
    //qDebug() << "true1";

    ini_IKFK();
    IKFK_parameter('w',0,true);
}

void Dialog::on_pushButton_44_clicked() //-x
{
    //qDebug() << "true1";

    ini_IKFK();
    IKFK_parameter('s',0,true);
}

void Dialog::on_pushButton_42_clicked() //+y
{
    ini_IKFK();
    IKFK_parameter('a',0,true);
}

void Dialog::on_pushButton_45_clicked() //-y
{
    ini_IKFK();
    IKFK_parameter('d',0,true);
}

void Dialog::on_pushButton_43_clicked() //+z
{
    ini_IKFK();
    IKFK_parameter('r',0,true);
}

void Dialog::on_pushButton_46_clicked() //-z
{
    ini_IKFK();
    IKFK_parameter('f',0,true);
}

void Dialog::on_pushButton_24_clicked() //+J1
{
    ini_IKFK();
    IKFK_parameter('1',1,true);
}

void Dialog::on_pushButton_26_clicked() //+J2
{
    ini_IKFK();
    IKFK_parameter('2',1,true);
}

void Dialog::on_pushButton_31_clicked() //+J3
{
    ini_IKFK();
    IKFK_parameter('3',1,true);
}

void Dialog::on_pushButton_33_clicked() //+J4
{
    ini_IKFK();
    IKFK_parameter('4',1,true);
}

void Dialog::on_pushButton_23_clicked() //-J1
{
    ini_IKFK();
    IKFK_parameter('1',2,true);
}

void Dialog::on_pushButton_25_clicked() //-J2
{
    ini_IKFK();
    IKFK_parameter('2',2,true);
}

void Dialog::on_pushButton_28_clicked() //-J3
{
    ini_IKFK();
    IKFK_parameter('3',2,true);
}

void Dialog::on_pushButton_32_clicked() //-J4
{
    ini_IKFK();
    IKFK_parameter('4',2,true);
}



void Dialog::call_suck_on(int ioport)
{
    wiringPiSetup () ;
    //pinMode (relayport, OUTPUT) ;
    //digitalWrite (relayport, HIGH);  //high

    pinMode(ioport, OUTPUT);
    digitalWrite(ioport, HIGH);
}

void Dialog::call_suck_off(int ioport)
{
    wiringPiSetup () ;
    //pinMode (relayport, OUTPUT) ;
    //digitalWrite (relayport, LOW);  //high

    pinMode(ioport, OUTPUT);
    digitalWrite(ioport, LOW);
}

void Dialog::on_pushButton_29_clicked() //Valve on
{
    call_suck_on(relayport);
    fstream fst2;
    fst2.open("readpos_IKFK.txt", ios::out|ios::app);
    fst2 << "pumpon\t" << ui->lineEdit_EM_span->text().toStdString() <<"\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    fst2.close();

    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
       // qDebug()  << "no select" << row ;
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
    }

   qDebug()  << "insert:" << row ;
   //QString s= QString::number(j);
   ui->tableWidget->insertRow(row);
   ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("pumpon")));
   ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString(ui->lineEdit_EM_span->text())));
   ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,12,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,13,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,14,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,15,new QTableWidgetItem(QString("0")));
   j=j+1;

   ui->tableWidget->setCurrentCell(-1,-1);
}

void Dialog::on_pushButton_30_clicked() //Valve off
{
    call_suck_off(relayport);
    fstream fst2;
    fst2.open("readpos_IKFK.txt", ios::out|ios::app);
    fst2 << "pumpoff\t" << ui->lineEdit_EM_span->text().toStdString() <<"\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    fst2.close();

    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
        //qDebug()  << "no select" << row ;
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
    }

   qDebug()  << "insert:" << row ;
   //QString s= QString::number(j);
   ui->tableWidget->insertRow(row);
   ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("pumpoff")));
   ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString(ui->lineEdit_EM_span->text())));
   ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,12,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,13,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,14,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,15,new QTableWidgetItem(QString("0")));
   j=j+1;

   ui->tableWidget->setCurrentCell(-1,-1);
}

void Dialog::on_pushButton_ready_clicked()  //***waiting for scan
{
    fstream fst2;
    fst2.open("readpos_IKFK.txt", ios::out|ios::app);
    fst2 << "ready\t" <<"\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    fst2.close();

    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
        //qDebug()  << "no select" << row ;
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
    }

   qDebug()  << "insert:" << row ;
   //QString s= QString::number(j);
   ui->tableWidget->insertRow(row);
   ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("ready")));
   ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,12,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,13,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,14,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,15,new QTableWidgetItem(QString("0")));
   j=j+1;

   ui->tableWidget->setCurrentCell(-1,-1);
}


void Dialog::call_gpio_on(int ioport)//***gpio
{
    wiringPiSetup () ;

    pinMode(ioport,OUTPUT);
    digitalWrite (ioport, HIGH);
}

void Dialog::call_gpio_off(int ioport)
{
    wiringPiSetup () ;

    pinMode (ioport, OUTPUT) ;
    digitalWrite (ioport, LOW);
}

void Dialog::on_pushButton_GPIO_on_clicked()
{
    call_gpio_on(ui->comboBox_gpioChoose->currentIndex()+1);
    fstream fst2;
    fst2.open("readpos_IKFK.txt", ios::out|ios::app);
    //fst2 << "gpioon\t" << ui->lineEdit_EM_span->text().toStdString() <<"\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    fst2 << "gpioon\t" << ui->lineEdit_EM_span->text().toStdString() << "\t" << ui->comboBox_gpioChoose->currentIndex()+1 <<"\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    fst2.close();

    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
        //qDebug()  << "no select" << row ;
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
    }
    cout << ui->comboBox_gpioChoose->currentIndex()<<endl;
    //int gpio_index = ui->comboBox_gpioChoose->currentIndex()+1;
    //cout << "-----------------------"<<gpio_index<<endl;
   qDebug()  << "insert:" << row ;
   //QString s= QString::number(j);
   ui->tableWidget->insertRow(row);
   ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("gpioon")));
   ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString(ui->lineEdit_EM_span->text())));
   ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(ui->comboBox_gpioChoose->currentIndex()+1)));
   ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,12,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,13,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,14,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,15,new QTableWidgetItem(QString("0")));
   j=j+1;

   ui->tableWidget->setCurrentCell(-1,-1);
}

void Dialog::on_pushButton_GPIO_off_clicked()
{
    call_gpio_off(ui->comboBox_gpioChoose->currentIndex()+1);
    fstream fst2;
    fst2.open("readpos_IKFK.txt", ios::out|ios::app);
    fst2 << "gpiooff\t" << ui->lineEdit_EM_span->text().toStdString() << "\t" << ui->comboBox_gpioChoose->currentIndex()+1 <<"\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    fst2.close();

    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
        //qDebug()  << "no select" << row ;
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
    }
    //int gpio_index = ui->comboBox_gpioChoose->currentIndex()+1;
    //cout << "-----------------------"<<gpio_index<<endl;
   qDebug()  << "insert:" << row ;
   //QString s= QString::number(j);
   ui->tableWidget->insertRow(row);
   ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("gpiooff")));
   ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString(ui->lineEdit_EM_span->text())));
   ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::number(ui->comboBox_gpioChoose->currentIndex()+1)));
   ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("0")));
   ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,12,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,13,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,14,new QTableWidgetItem(QString("0")));
   //ui->tableWidget->setItem(row,15,new QTableWidgetItem(QString("0")));
   j=j+1;

   ui->tableWidget->setCurrentCell(-1,-1);
}
//***gpioend


void Dialog::on_pushButton_choose_mode_clicked()// mode choose ~!@#$
{
    fstream fst2;
    fst2.open("readpos_IKFK.txt", ios::out|ios::app);
    if(ui->comboBox_mode_choose->currentIndex()==0)
    {
        fst2 << "leftabsorb" << "\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    }
    else if(ui->comboBox_mode_choose->currentIndex()==1)
    {
        fst2 << "together" << "\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    }
    else if(ui->comboBox_mode_choose->currentIndex()==2)
    {
        fst2 << "rightdeflate" << "\t0\t0\t0\t0\t0\t0\t0\t0\t0\t0\t\n";
    }
    fst2.close();

    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
        //qDebug()  << "no select" << row ;
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
    }
    qDebug()  << "insert:" << row ;
    ui->tableWidget->insertRow(row);
    switch(ui->comboBox_mode_choose->currentIndex())
    {
        case 0:
            ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("leftabsorb")));
            break;
        case 1:
            ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("together")));
            break;
        case 2:
            ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("rightdeflate")));
            break;
    }
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString("0")));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString("0")));
    ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("0")));
    ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("0")));
    ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("0")));
    ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("0")));
    ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("0")));
    ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("0")));
    ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("0")));
    ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("0")));
    //ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("0")));
    //ui->tableWidget->setItem(row,12,new QTableWidgetItem(QString("0")));
    //ui->tableWidget->setItem(row,13,new QTableWidgetItem(QString("0")));
    //ui->tableWidget->setItem(row,14,new QTableWidgetItem(QString("0")));
    //ui->tableWidget->setItem(row,15,new QTableWidgetItem(QString("0")));
    j=j+1;

    ui->tableWidget->setCurrentCell(-1,-1);
}




void Dialog::on_pushButton_9_clicked() //mode1
{
    ui->stackedWidget_mode->setCurrentIndex(0);
}

void Dialog::on_pushButton_10_clicked() //mode2
{
    ui->stackedWidget_mode->setCurrentIndex(1);
}

void Dialog::on_pushButton_11_clicked() //mode3
{
    ui->stackedWidget_mode->setCurrentIndex(2);
}

void Dialog::on_pushButton_14_clicked() //mode other
{
    //mThread6->check = false;
    ui->stackedWidget_mode->setCurrentIndex(3);
}

void Dialog::on_pushButton_51_clicked()  //mode4
{
    ui->stackedWidget_mode->setCurrentIndex(4);
}



void Dialog::GetTemp(int joint, int temp)  //temperature   //$#@!
{
    if(joint == 0)
    {
        ui->lineEdit_temp_J1->setText(QString::number(temp));
    }
    else if(joint == 1)
    {
        ui->lineEdit_temp_J2->setText(QString::number(temp));
    }
    else if(joint == 2)
    {
        ui->lineEdit_temp_J3->setText(QString::number(temp));
    }
    else if(joint == 3)
    {
        ui->lineEdit_temp_J4->setText(QString::number(temp));
    }

}

void Dialog::GetError(int joint2,double error)
{
    if(joint2 == 0)
    {
        ui->lineEdit_j1_error_2->setText(QString::number(error));
    }
    else if(joint2 == 1)
    {
        ui->lineEdit_j2_error_2->setText(QString::number(error));
    }
    else if(joint2 == 2)
    {
        ui->lineEdit_j3_error_2->setText(QString::number(error));
    }
    else if(joint2 == 3)
    {
        ui->lineEdit_j4_error_2->setText(QString::number(error));
    }
    
}


//Path Editor and Application Output
void Dialog::on_pushButton_delete_clicked() //delete
{
    switch(QMessageBox::question(this, "Delete", "<b>Do you want to delete current row?",
                                 QMessageBox::Yes | QMessageBox:: Cancel, QMessageBox::Cancel)){
        case QMessageBox::Yes:
            ui->tableWidget->removeRow(ui->tableWidget->currentRow());
            break;
        case QMessageBox::Cancel:
            break;
    }


    //ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void Dialog::on_pushButton_52_clicked() //Save path
{
    QFileDialog myFileDialog (this);
    //QString itemPath = myFileDialog.getOpenFileName (this, tr ("Open File"), QDir::currentPath ());

    QString itemPath = myFileDialog.getSaveFileName(this, tr ("Save File"), QDir::currentPath ());
    if (itemPath.isEmpty ())
    {
      ;
    }
    else
    {

      std::fstream fst;
      fst.open(itemPath.toStdString().c_str(), ios::out);

      qDebug()  << "indexi:" << ui->tableWidget->rowCount();
      for(int i=0;i<(ui->tableWidget->rowCount());i++)
      {
          qDebug()  << "indexj:" << i;
          for(int j=0;j<11;j++)
          {
              if(ui->tableWidget->item(i,j)->text() != "")
              {   qDebug()  << "111\n";
                  qDebug()  << "index:" << ui->tableWidget->item(i,j)->text();
                  fst << qstrdup(qPrintable(ui->tableWidget->item(i,j)->text()))<< "\t";
              }
              else
              {
                  qDebug()  << "null:";
                  fst << "\t";
              }
          }
          fst << "\n";
      }


      fst.close();

    }
}

void Dialog::on_pushButton_add_group_clicked()  //Groups
{
    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
   }

    //qDebug()  << "insert:" << ui->tableWidget->currentRow() ;
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("group")));
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("")));
    ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("")));
    //ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("")));
    //ui->tableWidget->setItem(row,12,new QTableWidgetItem(QString("")));
    //ui->tableWidget->setItem(row,13,new QTableWidgetItem(QString("")));
    //ui->tableWidget->setItem(row,14,new QTableWidgetItem(QString("")));
    //ui->tableWidget->setItem(row,15,new QTableWidgetItem(QString("")));
}

void Dialog::load_pos ()    //Load, func
{
//  QString directory = QFileDialog::getExistingDirectory (this, tr ("Find Files"), QDir::currentPath ());

  QFileDialog myFileDialog (this);
  QString itemPath = myFileDialog.getOpenFileName (this, tr ("Open File"), QDir::currentPath ());
  if (itemPath.isEmpty ())
  {
    ;
  }
  else
  {
    QString text= "Setting from file:"+itemPath+"\n";
    ui->lineEdit_3->setText(QString(itemPath));
    ui->plainTextEdit->textCursor().insertText(text);

    sleep(1);

    //cout<< "read ok\n" <<endl;
    //qDebug()  << "read ok:";


    std::string filename(itemPath.toStdString());
    //std::ifstream preread(filename.c_str());

//    char prebuffer[256];

//    int line=0;
//    while (!preread.eof() )
//    {
//        preread.getline (prebuffer,100);
//        line++;
//    }



    std::ifstream in(filename.c_str());
    char buffer[256];

    //int row=ui->tableWidget->rowCount();

    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
        //qDebug()  << "no select" << row ;
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
    }
    //cout<< "line+:" <<line <<endl;
    //cout<< "row+:" <<endl;
    //qDebug() <<"line:" <<line;
   // qDebug() <<"row:" <<row;

    //ui->tableWidget->setRowCount(line+row-1);

    while (!in.eof() )
    {
        in.getline (buffer,100);

        char *delim = "\t";
        char * pch;
        pch = strtok(buffer,delim);
        //qDebug()  << buffer;

        int col=0;
        if(pch != NULL)
        {
            ui->tableWidget->insertRow(row);
        }
        //qDebug() <<"+++:" <<pch;
        while (pch != NULL)
        {
            //qDebug() <<"sub:" <<pch;

            ui->tableWidget->setItem(row,col,new QTableWidgetItem(QString(pch)));

            if(strcmp(pch, "group") == 0)
            {
                //ui->tableWidget_3->insertRow(row);
                //ui->tableWidget_3->setItem(row,0,new QTableWidgetItem(QString("group")));
                ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString("")));
                ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString("")));
                ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("")));
                ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("")));
                ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("")));
                ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("")));
                ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("")));
                ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("")));
                ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("")));
                ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("")));
                //ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("")));
                //ui->tableWidget->setItem(row,12,new QTableWidgetItem(QString("")));
                //ui->tableWidget->setItem(row,13,new QTableWidgetItem(QString("")));
                //ui->tableWidget->setItem(row,14,new QTableWidgetItem(QString("")));
                //ui->tableWidget->setItem(row,15,new QTableWidgetItem(QString("")));

            }
            //ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString(pch)));
            pch = strtok (NULL, delim);
            col+=1;
        }

        //if(c==1){ui->lineEdit_1->setText(QString(buffer));}
        row++;
    }
    //qDebug()  << "end:";
    ui->tableWidget->setCurrentCell(-1,-1);


  }
  plain_end();
}

void Dialog::on_pushButton_13_clicked() //Load
{
    load_pos();
}


void Dialog::clear_table()  //Set path and Clear, func
{
    int count = ui->tableWidget->rowCount();

    for(int i = count; i>= 0; i--)
    {
        ui->tableWidget->removeRow(i);
    }
}

int Dialog::browse ()   //Set path, func
{
//  QString directory = QFileDialog::getExistingDirectory (this, tr ("Find Files"), QDir::currentPath ());

  QFileDialog myFileDialog (this);
  QString itemPath = myFileDialog.getOpenFileName (this, tr ("Open File"), QDir::currentPath ());
  if (itemPath.isEmpty ())
  {
    return 1;
  }
  else
  {
    QString cmd = "python robot6_3.py "+itemPath;
    QString text = "Setting from file:"+itemPath+"\n";
    ui->lineEdit_3->setText(QString(itemPath));
    ui->plainTextEdit->textCursor().insertText(text);

    std::string filename(itemPath.toStdString());
    std::ifstream preread(filename.c_str());

    char prebuffer[1024];

    int line=0;
    while (!preread.eof() )
    {
        preread.getline (prebuffer,1024);
        line++;
    }
    cout<<"line"<<line;

    std::ifstream in(filename.c_str());
    char buffer[1024];

    //int row=ui->tableWidget->rowCount();

    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
        //qDebug()  << "no select" << row ;
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
    }
   // cout<< "line+:" <<line <<endl;
    //cout<< "row+:" <<endl;
    //qDebug() <<"line:" <<line;
    //qDebug() <<"row:" <<row;

    //ui->tableWidget->setRowCount(line+row-1);

    while (!in.eof() )
    {
        in.getline (buffer,1024);

        char *delim = "\t";
        char * pch;
        pch = strtok(buffer,delim);
        //qDebug()  << buffer;

        int col=0;
        if(pch != NULL)
        {
            ui->tableWidget->insertRow(row);
        }
        qDebug() <<"+++:" <<pch;
        while (pch != NULL)
        {
            qDebug() <<"sub:" <<pch;

            ui->tableWidget->setItem(row,col,new QTableWidgetItem(QString(pch)));
            //ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString(pch)));
            pch = strtok (NULL, delim);
            col+=1;
        }

        //if(c==1){ui->lineEdit_1->setText(QString(buffer));}
        row++;
    }
    //qDebug()  << "end:";
    ui->tableWidget->setCurrentCell(-1,-1);

    //system(cmd.toStdString().c_str());

    plain_end();
    return 0;
  }

}

void Dialog::on_pushButton_3_clicked()  //Clear
{
    switch(QMessageBox::question(this, "Clear", "<b>Do you want to clear table?",
                                 QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel)){
        case QMessageBox::Yes:
            clear_table();
            break;
        case QMessageBox::Cancel:
            break;
    }


    //clear_table();
}

void Dialog::on_pushButton_read_group_clicked() //Set path, CMD=read_group
{
    clear_table();

    if(browse() == 0)
    {
        arm_init();
        mThread2->cmd=qstrdup(qPrintable(QString("read_group")));
        qDebug()  << "CMD:" << mThread2->cmd;
        mThread2->start();
    }
}

void Dialog::on_pushButton_start_group_clicked()    //Start, CMD=syncw_group
{

    if(ui->lineEdit_3->text()!="")
    {
        arm_init();

        mThread2->cmd=qstrdup(qPrintable(QString("syncw_group")));
        qDebug()  << "CMD:" << mThread2->cmd;

        QString s1 = QString("Start\n");
        ui->plainTextEdit->textCursor().insertText(s1);
        mThread2->stop=false;   //#@!
        mThread2->start();
    }
    else
    {
        QString text= "No file selected..\n";
        ui->plainTextEdit->textCursor().insertText(text);
    }

}


void Dialog::on_pushButton_12_clicked() //clear message
{
    ui->plainTextEdit->clear();
}



void Dialog::on_pushButton_clicked()    //CMD
{
    //if(ui->radioButton->isChecked())
   // {

        if(ui->lineEdit_CMD->text()!="")
        {
            arm_init();
            mThread2->cmd=qstrdup(qPrintable(ui->lineEdit_CMD->text()));
            qDebug()  << "CMD:" << mThread2->cmd;

            QString s1 = QString("Start\n");
            ui->plainTextEdit->textCursor().insertText(s1);
            mThread2->start();
        }
        else
        {
            ;
        }
    //}
}

void Dialog::on_pushButton_47_clicked() //Torque on
{
    arm_init();
    mThread2->cmd=qstrdup(qPrintable(QString("tonall")));
    qDebug()  << "CMD:" << mThread2->cmd;
    mThread2->start();
}

void Dialog::on_pushButton_48_clicked() //Torque off
{
    arm_init();
    mThread2->cmd=qstrdup(qPrintable(QString("toffall")));
    qDebug()  << "CMD:" << mThread2->cmd;
    mThread2->start();
}




//Planning
void Dialog::on_pushButton_enter_clicked()  //enter
{
    switch(QMessageBox::question(this, "Enter the coordinate", "<b>Sure?",
                                 QMessageBox::Yes | QMessageBox:: Cancel, QMessageBox::Cancel)){
        case QMessageBox::Yes:

            if(ui->comboBox_coord_order->currentIndex()==0)
            {
                ui->lineEdit_1x->setText(QString::number(ui->lineEdit_8->text().toDouble()));
                ui->lineEdit_1y->setText(QString::number(ui->lineEdit_9->text().toDouble()));
                ui->lineEdit_1z->setText(QString::number(ui->lineEdit_10->text().toDouble()));
                ui->lineEdit_ox->setText(QString::number(ui->lineEdit_ox->text().toInt()));
                ui->lineEdit_oy->setText(QString::number(ui->lineEdit_oy->text().toInt()));
                ui->lineEdit_oz->setText(QString::number(ui->lineEdit_oz->text().toInt()));

            }
            else if(ui->comboBox_coord_order->currentIndex()==1)
            {
                ui->lineEdit_2x->setText(QString::number(ui->lineEdit_8->text().toDouble()));
                ui->lineEdit_2y->setText(QString::number(ui->lineEdit_9->text().toDouble()));
                ui->lineEdit_2z->setText(QString::number(ui->lineEdit_10->text().toDouble()));
                ui->lineEdit_ox->setText(QString::number(ui->lineEdit_ox->text().toInt()));
                ui->lineEdit_oy->setText(QString::number(ui->lineEdit_oy->text().toInt()));
                ui->lineEdit_oz->setText(QString::number(ui->lineEdit_oz->text().toInt()));
            }
            else if(ui->comboBox_coord_order->currentIndex()==2)
            {
                ui->lineEdit_3x->setText(QString::number(ui->lineEdit_8->text().toDouble()));
                ui->lineEdit_3y->setText(QString::number(ui->lineEdit_9->text().toDouble()));
                ui->lineEdit_3z->setText(QString::number(ui->lineEdit_10->text().toDouble()));
                ui->lineEdit_ox->setText(QString::number(ui->lineEdit_ox->text().toInt()));
                ui->lineEdit_oy->setText(QString::number(ui->lineEdit_oy->text().toInt()));
                ui->lineEdit_oz->setText(QString::number(ui->lineEdit_oz->text().toInt()));
            }
            else if(ui->comboBox_coord_order->currentIndex()==3)
            {
                ui->lineEdit_4x->setText(QString::number(ui->lineEdit_8->text().toDouble()));
                ui->lineEdit_4y->setText(QString::number(ui->lineEdit_9->text().toDouble()));
                ui->lineEdit_4z->setText(QString::number(ui->lineEdit_10->text().toDouble()));
                ui->lineEdit_ox->setText(QString::number(ui->lineEdit_ox->text().toInt()));
                ui->lineEdit_oy->setText(QString::number(ui->lineEdit_oy->text().toInt()));
                ui->lineEdit_oz->setText(QString::number(ui->lineEdit_oz->text().toInt()));
            }
            else if(ui->comboBox_coord_order->currentIndex()==4)
            {
                ui->lineEdit_5x->setText(QString::number(ui->lineEdit_8->text().toDouble()));
                ui->lineEdit_5y->setText(QString::number(ui->lineEdit_9->text().toDouble()));
                ui->lineEdit_5z->setText(QString::number(ui->lineEdit_10->text().toDouble()));
                ui->lineEdit_ox->setText(QString::number(ui->lineEdit_ox->text().toInt()));
                ui->lineEdit_oy->setText(QString::number(ui->lineEdit_oy->text().toInt()));
                ui->lineEdit_oz->setText(QString::number(ui->lineEdit_oz->text().toInt()));
            }
            else if(ui->comboBox_coord_order->currentIndex()==5)
            {
                ui->lineEdit_6x->setText(QString::number(ui->lineEdit_8->text().toDouble()));
                ui->lineEdit_6y->setText(QString::number(ui->lineEdit_9->text().toDouble()));
                ui->lineEdit_6z->setText(QString::number(ui->lineEdit_10->text().toDouble()));
                ui->lineEdit_ox->setText(QString::number(ui->lineEdit_ox->text().toInt()));
                ui->lineEdit_oy->setText(QString::number(ui->lineEdit_oy->text().toInt()));
                ui->lineEdit_oz->setText(QString::number(ui->lineEdit_oz->text().toInt()));
            }
            else if(ui->comboBox_coord_order->currentIndex()==6)
            {
                fstream fst;
                fst.open("cell_coord.txt",ios::out|ios::app);
                fst<<ui->lineEdit_8->text().toStdString().c_str()<<"\t"<<ui->lineEdit_9->text().toStdString().c_str()<<"\t"<<ui->lineEdit_10->text().toStdString().c_str()<<"\t"<<ui->lineEdit_ox->text().toStdString().c_str()<<"\t"<<ui->lineEdit_oy->text().toStdString().c_str()<<"\t"<<ui->lineEdit_oz->text().toStdString().c_str()<<"\n";
                fst.close();

            }
            break;

        case QMessageBox::Cancel:
            break;
    }

}

void Dialog::on_pushButton_modify_zvec_clicked()    //modifty_z: modify the z vector of the tray
{

    planning_initialization();

    if(ui->checkBox_modify_zvec->isChecked() == 0)
    {
        QMessageBox message(QMessageBox::Warning, "Warning", "The BOX have to check!", QMessageBox::Ok,NULL);
        message.exec();

    }
    else if(ui->checkBox_modify_zvec->isChecked() == 1)
    {
        cout << "set coord" << endl;

        mThread8->coord_1[0] = ui->lineEdit_1x->text().toDouble();
        mThread8->coord_1[1] = ui->lineEdit_1y->text().toDouble();
        mThread8->coord_1[2] = ui->lineEdit_1z->text().toDouble();

        mThread8->coord_2[0] = ui->lineEdit_2x->text().toDouble();
        mThread8->coord_2[1] = ui->lineEdit_2y->text().toDouble();
        mThread8->coord_2[2] = ui->lineEdit_2z->text().toDouble();

        mThread8->coord_3[0] = ui->lineEdit_3x->text().toDouble();
        mThread8->coord_3[1] = ui->lineEdit_3y->text().toDouble();
        mThread8->coord_3[2] = ui->lineEdit_3z->text().toDouble();

        mThread8->coord_o[0]=ui->lineEdit_ox->text().toInt();
        mThread8->coord_o[1]=ui->lineEdit_oy->text().toInt();
        mThread8->coord_o[2]=ui->lineEdit_oz->text().toInt();
        mThread8->cmd=qstrdup(qPrintable(QString("modifyz")));
        mThread8->start();

        while(1)
        {
            if(mThread8->isFinished())
            {
                QString s = QString("Modify finished!\n");
                ui->plainTextEdit->textCursor().insertText(s);
            }
            break;
        }


    }
}

void Dialog::on_pushButton_generate_clicked()   //generate
{
    switch(QMessageBox::question(this, "Generate Coordinate file", "<b>Sure?",
                                 QMessageBox::Yes | QMessageBox:: Cancel, QMessageBox::Cancel)){
        case QMessageBox::Yes:

            planning_initialization();

            mThread8->coord_1[0] = ui->lineEdit_1x->text().toDouble();
            mThread8->coord_1[1] = ui->lineEdit_1y->text().toDouble();
            mThread8->coord_1[2] = ui->lineEdit_1z->text().toDouble();

            mThread8->coord_2[0] = ui->lineEdit_2x->text().toDouble();
            mThread8->coord_2[1] = ui->lineEdit_2y->text().toDouble();
            mThread8->coord_2[2] = ui->lineEdit_2z->text().toDouble();

            mThread8->coord_3[0] = ui->lineEdit_3x->text().toDouble();
            mThread8->coord_3[1] = ui->lineEdit_3y->text().toDouble();
            mThread8->coord_3[2] = ui->lineEdit_3z->text().toDouble();

            mThread8->coord_4[0] = ui->lineEdit_4x->text().toDouble();
            mThread8->coord_4[1] = ui->lineEdit_4y->text().toDouble();
            mThread8->coord_4[2] = ui->lineEdit_4z->text().toDouble();

            if(ui->checkBox_coord5and6->isCheckable() == 1)
            {
                mThread8->coord_5[0] = ui->lineEdit_5x->text().toDouble();
                mThread8->coord_5[1] = ui->lineEdit_5y->text().toDouble();
                mThread8->coord_5[2] = ui->lineEdit_5z->text().toDouble();

                mThread8->coord_6[0] = ui->lineEdit_6x->text().toDouble();
                mThread8->coord_6[1] = ui->lineEdit_6y->text().toDouble();
                mThread8->coord_6[2] = ui->lineEdit_6z->text().toDouble();
            }
            else{;}

            mThread8->coord_o[0]=ui->lineEdit_ox->text().toInt();
            mThread8->coord_o[1]=ui->lineEdit_oy->text().toInt();
            mThread8->coord_o[2]=ui->lineEdit_oz->text().toInt();

            if(ui->comboBox_coordinate_type->currentIndex() == 0)
            {
                QString s1 = QString("Coordinate planning type: tested tray\n");
                ui->plainTextEdit->textCursor().insertText(s1);

                mThread8->filename = "./tested_tray_coord.txt";
            }
            else if(ui->comboBox_coordinate_type->currentIndex() == 1)
            {
                QString s2 = QString("Coordinate planning type: pass tray\n");
                ui->plainTextEdit->textCursor().insertText(s2);

                mThread8->filename = "./pass_tray_coord.txt";
            }
            else if(ui->comboBox_coordinate_type->currentIndex() == 2)
            {
                QString s3 = QString("Coordinate planning type: fail tray\n");
                ui->plainTextEdit->textCursor().insertText(s3);

                mThread8->filename = "./fail_tray_coord.txt";
            }

            mThread8->cmd=qstrdup(qPrintable(QString("generate")));
            mThread8->start();

            while(1)
            {
                if(mThread8->isFinished())
                {
                    QString s = QString("Generate the file!\n");
                    ui->plainTextEdit->textCursor().insertText(s);
                }
                break;
            }

            break;

        case QMessageBox::Cancel:
            break;
    }

}



//Socket
void Dialog::on_pushButton_34_clicked() //Recv
{
    arm_init();
    mThread2->cmd=qstrdup(qPrintable(QString("readpos")));
    qDebug()  << "CMD:" << mThread2->cmd;
    mThread2->start();

    while(1)
    {
        if(mThread2->isFinished())
        {
            break;
        }
    }

    cout << "mThread2->state:" << mThread2->state<<endl;

    if(mThread2->state==1)
    {
        sleep(1);
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

    mThread5->Exit = false;
    mThread5->port = qstrdup(qPrintable(ui->lineEdit_6->text()));
    mThread5->Check = true;
    mThread5->start();
}

void Dialog::on_pushButton_35_clicked() //Stop
{

    mThread5->Exit = true;

    if(mThread5->isRunning())
    {
        mThread3->host="10.10.10.100";
        mThread3->port=qstrdup(qPrintable(ui->lineEdit_6->text()));
        mThread3->str="null";
        mThread3->start();

        qDebug() << "Close!";
    }
    else
    {
        qDebug() << "Stop";
    }

}




//Modbus
void Dialog::Modbus_mode(int mode)
{
    if(mode == 0)
    {
        ui->stackedWidget_Modbus_mode->setCurrentIndex(0);
    }
    else if(mode == 1)
    {
        ui->stackedWidget_Modbus_mode->setCurrentIndex(1);
    }
}

void Dialog::on_pushButton_17_clicked() //Recv
{
    /*
    arm_init();
    mThread2->cmd=qstrdup(qPrintable(QString("readpos")));
    qDebug()  << "CMD:" << mThread2->cmd;
    mThread2->start();

    while(1)
    {
        if(mThread2->isFinished())
        {
            break;
        }
    }

    cout << "mThread2->state:" << mThread2->state<<endl;

    if(mThread2->state==1)
    {
        sleep(1);
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
    */


    modbus_initialization();

    if(ui->comboBox_Modbus_mode->currentIndex() == 0)   //Teaching
    {
        mThread7->Exit = false;
        mThread7->set_PLC_Dev(get_PLC_Dev());
        mThread7->start();

        QString s1 = QString("mythread7 start\n");
        ui->plainTextEdit->textCursor().insertText(s1);
    }
    else if(ui->comboBox_Modbus_mode->currentIndex() == 1)  //Play
    {
        ini_IKFK();
        mThread4->C = 'b';
        //mThread4->type=2;
        mThread4->coord[0] = ui->lineEdit_8->text().toDouble();
        mThread4->coord[1] = ui->lineEdit_9->text().toDouble();
        mThread4->coord[2] = ui->lineEdit_10->text().toDouble();
        mThread4->Check = true;
        mThread4->start();
        while(1)
        {
            if(mThread4->isFinished());
            {
                arm_init();
                mThread2->times=1;
                mThread2->cmd=qstrdup(qPrintable(QString("readpos")));
                mThread2->start();
                break;
            }
        }
        while(1)
        {
            if(mThread2->isFinished())
            {
                fstream fstg;
                fstg.open("position_motion_first.txt_4ax",ios::out|ios::app);
                for(int i=0;i<6;i++)
                {
                    if(i!=3)
                    {
                        fstg<<fabs(mThread4->first_goal_theta[i]-mThread2->first_reality_theta[i])<<"\t";
                    }
                }
                fstg<<"\n";
                fstg.close();
                break;
            }
        }
        mThread6->Exit = false;
        mThread6->check = true;
        mThread6->set_PLC_Dev(get_PLC_Dev());
        mThread6->start();

        QString s2 = QString("mythread6 start\n");
        ui->plainTextEdit->textCursor().insertText(s2);
    }
}

void Dialog::on_pushButton_18_clicked() //Stop
{
    if(ui->comboBox_Modbus_mode->currentIndex() == 0)
    {
        mThread7->Exit = true;
    }
    else if(ui->comboBox_Modbus_mode->currentIndex() == 1)
    {
        mThread6->Exit = true;
        mThread2->stop = true;
    }
}


void Dialog::on_pushButton_cylinder_down_clicked()
{
    mThread7->down7=true;
}

void Dialog::on_pushButton_cylinder_up_clicked()
{
    mThread7->up7=true;
}

void Dialog::on_pushButton_left_vacuum_clicked()
{
    mThread7->leftabsorb7=true;
}

void Dialog::on_pushButton_left_flow_clicked()
{
    mThread7->leftdeflate7=true;
}

void Dialog::on_pushButton_right_vacuum_clicked()
{
    mThread7->rightabsorb7=true;
}

void Dialog::on_pushButton_right_flow_clicked()
{
    mThread7->rightdeflate7=true;
}




//Page
void Dialog::on_pushButton_7_clicked()  //Change
{

    if(ui->lineEdit_3->text()!="")
    {
        ini_IKFK();
        mThread4->C = 't';
        mThread4->coord[0] = ui->lineEdit_changex->text().toDouble();
        mThread4->coord[1] = ui->lineEdit_changey->text().toDouble();
        mThread4->coord[2] = ui->lineEdit_changez->text().toDouble();
        mThread4->Check = true;
        mThread4->start();

        while(1)
        {
            if(mThread4->isFinished())
            {
                arm_init();

                mThread2->change_step_now = ui->lineEdit_change_step_now->text().toInt();
                mThread2->change_step_pre = ui->lineEdit_change_step_pre->text().toInt();
                mThread2->change_group = ui->lineEdit_change_group->text().toInt();

                mThread2->change_pos[0] = mThread4->theta2value[0];
                mThread2->change_pos[1] = mThread4->theta2value[1];
                mThread2->change_pos[2] = mThread4->theta2value[2];
                mThread2->change_pos[3] = mThread4->theta2value[3];

                qDebug()  << "change";
                qDebug()  << mThread2->change_pos[0];
                qDebug()  << mThread2->change_pos[1];
                qDebug()  << mThread2->change_pos[2];
                qDebug()  << mThread2->change_pos[3];

                mThread2->cmd=qstrdup(qPrintable(QString("dynamic_syncw_group")));
                qDebug()  << "CMD:" << mThread2->cmd;

                QString s1 = QString("Start\n");
                ui->plainTextEdit->textCursor().insertText(s1);
                mThread2->start();
                break;
            }
        }
    }
    else
    {
        QString text= "No file selected..\n";
        ui->plainTextEdit->textCursor().insertText(text);
    }
}




//Other
void Dialog::optimize_loadfile (int type)   //Optimize and Opt(group), func
{
//  QString directory = QFileDialog::getExistingDirectory (this, tr ("Find Files"), QDir::currentPath ());

  QFileDialog myFileDialog (this);
  QString itemPath = myFileDialog.getOpenFileName (this, tr ("Open File"), QDir::currentPath ());
  if (itemPath.isEmpty ())
  {
    ;
  }
  else
  {

      if(type == 1)
      {
          mThread2->cmd=qstrdup(qPrintable(QString("optimize")));
          qDebug()  << "CMD:" << mThread2->cmd;
      }
      else if(type == 2)
      {
          mThread2->cmd=qstrdup(qPrintable(QString("optimize2")));
          qDebug()  << "CMD:" << mThread2->cmd;
      }


    QString text= "Loading file:"+itemPath+"\n";
    ui->lineEdit_3->setText(QString(itemPath));
    ui->plainTextEdit->textCursor().insertText(text);

    arm_init();
//    mThread2->filename=qstrdup(qPrintable(ui->lineEdit_3->text()));
//    mThread2->omega_limit=ui->lineEdit->text().toFloat();

    mThread2->start();

  }
  plain_end();
}

void Dialog::on_pushButton_19_clicked() //Optimize
{
    optimize_loadfile(1);
}

void Dialog::on_pushButton_2_clicked()  //Opt(group)
{
   optimize_loadfile(2);
}




//backup
void Dialog::on_pushButton_play_group_clicked() //Play(group)
{
    arm_init();
    mThread2->filename="./readpos.txt";
    mThread2->times=1;

    mThread2->cmd=qstrdup(qPrintable(QString("syncw")));
    mThread2->start();

    while(1)
    {
        if(mThread2->isFinished())
        {
            sleep(1);

            std::string filename("readpos.txt");
            std::ifstream in(filename.c_str());
            char buffer[256];

            //int row=ui->tableWidget->rowCount();

            int row;
            if(ui->tableWidget->currentRow() == -1)
            {
                row = ui->tableWidget->rowCount();
               // qDebug()  << "no select" << row ;
            }
            else
            {
                row=ui->tableWidget->currentRow()+1;
            }
            //cout<< "row+:" <<endl;
            //qDebug() <<"row:" <<row;

            //ui->tableWidget->setRowCount(line+row-1);

            while (!in.eof() )
            {
                in.getline (buffer,100);

                char *delim = "\t";
                char * pch;
                pch = strtok(buffer,delim);
                //qDebug()  << buffer;

                int col=0;
                if(pch != NULL)
                {
                    ui->tableWidget->insertRow(row);
                }
                //qDebug() <<"+++:" <<pch;
                while (pch != NULL)
                {
                    //qDebug() <<"sub:" <<pch;

                    ui->tableWidget->setItem(row,col,new QTableWidgetItem(QString(pch)));
                    //ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString(pch)));
                    pch = strtok (NULL, delim);
                    col+=1;
                }

                //if(c==1){ui->lineEdit_1->setText(QString(buffer));}
                row++;
            }
            //qDebug()  << "end:";
            ui->tableWidget->setCurrentCell(-1,-1);

            QString s1 = QString("Move..\n");
            ui->plainTextEdit->textCursor().insertText(s1);

            break;
        }
    }
}

void Dialog::on_pushButton_16_clicked() //Insert
{
     qDebug()  << "insert:" << ui->tableWidget->currentRow() ;
    ui->tableWidget->insertRow(ui->tableWidget->currentRow()+1);
    ui->tableWidget->setItem(ui->tableWidget->currentRow()+1,0,new QTableWidgetItem(QString("abc")));
}

void Dialog::on_pushButton_4_clicked()  //Set path(nong)
{
    clear_table();
    browse();
}

void Dialog::on_pushButton_21_clicked() //Teaching
{
    arm_init();
    mThread2->cmd=qstrdup(qPrintable(QString("readpos")));
    qDebug()  << "CMD:" << mThread2->cmd;
    mThread2->start();

    while(1)
    {
        if(mThread2->isFinished())
        {
            break;
        }
    }

    //cout << "mThread2->state:" << mThread2->state<<endl;

    if(mThread2->state==1)
    {
        sleep(1);
        QString s = QString("readpos done\n");
        ui->plainTextEdit->textCursor().insertText(s);

//        mThread4->mode = 1;
//        mThread4->coord[0] = ui->lineEdit_8->text().toDouble();
//        mThread4->coord[1] = ui->lineEdit_9->text().toDouble();
//        mThread4->coord[2] = ui->lineEdit_10->text().toDouble();

//        cout << mThread4->coord[0] << "\t" << ->coord[1] << "\t" << ->coord[2] << endl;

        ini_IKFK();

        if(!mThread4->isRunning())
        {
            mThread4->start();

        }
        else
        {
            QString s = QString("is running\n");
            ui->plainTextEdit->textCursor().insertText(s);
        }
        IKFK_parameter('0',0,true);
    }
    else
    {
        QString s = QString("readpos fail\n");
        ui->plainTextEdit->textCursor().insertText(s);
    }
}

void Dialog::on_pushButton_50_clicked() //Record
{
    sleep(1);

    cout<< "read ok\n" <<endl;
    qDebug()  << "read ok:";
    std::string filename("./readpos.txt");
    std::ifstream preread(filename.c_str());

    char prebuffer[256];

    int line=0;
    while (!preread.eof() )
    {
        preread.getline (prebuffer,100);
        line++;
    }



    std::ifstream in(filename.c_str());
    char buffer[256];

    //int row=ui->tableWidget->rowCount();

    int row;
    if(ui->tableWidget->currentRow() == -1)
    {
        row = ui->tableWidget->rowCount();
        //qDebug()  << "no select" << row ;
    }
    else
    {
        row=ui->tableWidget->currentRow()+1;
    }
    //cout<< "line+:" <<line <<endl;
    //cout<< "row+:" <<endl;
    //qDebug() <<"line:" <<line;
    //qDebug() <<"row:" <<row;

    //ui->tableWidget->setRowCount(line+row-1);

    while (!in.eof() )
    {
        in.getline (buffer,100);

        char *delim = "\t";
        char * pch;
        pch = strtok(buffer,delim);
        //qDebug()  << buffer;

        int col=0;
        if(pch != NULL)
        {
            ui->tableWidget->insertRow(row);
        }
        //qDebug() <<"+++:" <<pch;
        while (pch != NULL)
        {
            //qDebug() <<"sub:" <<pch;

            ui->tableWidget->setItem(row,col,new QTableWidgetItem(QString(pch)));
            //ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString(pch)));
            pch = strtok (NULL, delim);
            col+=1;
        }

        //if(c==1){ui->lineEdit_1->setText(QString(buffer));}
        row++;
    }
    //qDebug()  << "end:";
    ui->tableWidget->setCurrentCell(-1,-1);
}

void Dialog::on_pushButton_22_clicked() //Stop
{
    // mThread4->terminate();
     qDebug() << "true";
     ini_IKFK();

     //mThread4->Case_option = "exit";
     mThread4->C = 'q';
     mThread4->type=0;
     mThread4->Check = true;
}

void Dialog::on_pushButton_20_clicked() //Start(nong)
{
    if(ui->lineEdit_3->text()!="")
    {
        arm_init();
//        mThread2->filename=qstrdup(qPrintable(ui->lineEdit_3->text()));
//        mThread2->times=ui->lineEdit_4->text().toInt();
//        mThread2->step_span=ui->lineEdit_step_span->text().toFloat();

        mThread2->cmd=qstrdup(qPrintable(QString("syncw")));
        qDebug()  << "CMD:" << mThread2->cmd;

        QString s1 = QString("Start\n");
        ui->plainTextEdit->textCursor().insertText(s1);
        mThread2->start();
    }
    else
    {
        QString text= "No file selected..\n";
        ui->plainTextEdit->textCursor().insertText(text);
    }

}

void Dialog::on_pushButton_8_clicked()  //Teaching(none)
{
    ;
}




void Dialog::socket_load () //no function called
{
//  QString directory = QFileDialog::getExistingDirectory (this, tr ("Find Files"), QDir::currentPath ());

  QFileDialog myFileDialog (this);
  QString itemPath = myFileDialog.getOpenFileName (this, tr ("Open File"), QDir::currentPath ());
  if (itemPath.isEmpty ())
  {
    ;
  }
  else
  {
    QString text= "Loading from file:"+itemPath+"\n";
    //ui->lineEdit_3->setText(QString(itemPath));
    //ui->label_5->setText(QString(itemPath));
    mThread3->filename=qstrdup(qPrintable(itemPath));
    qDebug()  << "this->filename" << mThread->filename;
    ui->plainTextEdit->textCursor().insertText(text);
  }
  plain_end();
}






//void Dialog::on_pushButton_9_clicked()
//{
//    Dialog2 *dialog2 = new Dialog2;
//    dialog2->setAttribute(Qt::WA_DeleteOnClose);
//    dialog2->setWindowTitle(tr("Control"));
//    dialog2->show();
//}



//void Dialog::on_pushButton_10_clicked()
//{
//    mThread2->cmd=qstrdup(qPrintable(QString("readpos")));
//    qDebug()  << "CMD:" << mThread2->cmd;
//    mThread2->start();

//    while(1)
//    {
//        if(mThread2->isFinished())
//        {
//            //cout << "mThread2->state:" << mThread2->state<<endl;
//            if(mThread2->state==1)
//            {
//                sleep(1);

//                //cout<< "read ok\n" <<endl;
//                //qDebug()  << "read ok:";
//                std::string filename("readpos.txt");
//                std::ifstream preread(filename.c_str());

//                char prebuffer[256];

//                int line=0;
//                while (!preread.eof() )
//                {
//                    preread.getline (prebuffer,100);
//                    line++;
//                }



//                std::ifstream in(filename.c_str());
//                char buffer[256];

//                //int row=ui->tableWidget->rowCount();

//                int row;
//                if(ui->tableWidget->currentRow() == -1)
//                {
//                    row = ui->tableWidget->rowCount();
//                   // qDebug()  << "no select" << row ;
//                }
//                else
//                {
//                    row=ui->tableWidget->currentRow()+1;
//                }
//                //cout<< "line+:" <<line <<endl;
//                //cout<< "row+:" <<endl;
//                //qDebug() <<"line:" <<line;
//                //qDebug() <<"row:" <<row;

//                //ui->tableWidget->setRowCount(line+row-1);

//                while (!in.eof() )
//                {
//                    in.getline (buffer,100);

//                    char *delim = "\t";
//                    char * pch;
//                    pch = strtok(buffer,delim);
//                    //qDebug()  << buffer;

//                    int col=0;
//                    if(pch != NULL)
//                    {
//                        ui->tableWidget->insertRow(row);
//                    }
//                    //qDebug() <<"+++:" <<pch;
//                    while (pch != NULL)
//                    {
//                        //qDebug() <<"sub:" <<pch;

//                        ui->tableWidget->setItem(row,col,new QTableWidgetItem(QString(pch)));
//                        //ui->tableWidget->setItem(row, col, new QTableWidgetItem(QString(pch)));
//                        pch = strtok (NULL, delim);
//                        col+=1;
//                    }

//                    //if(c==1){ui->lineEdit_1->setText(QString(buffer));}
//                    row++;
//                }
//                //qDebug()  << "end:";
//                ui->tableWidget->setCurrentCell(-1,-1);

//            }
//            break;

//        }

//    }
//}
//void Dialog::on_pushButton_12_clicked()
//{
//    mThread2->cmd=qstrdup(qPrintable(QString("tonall")));
//    qDebug()  << "CMD:" << mThread2->cmd;
//    mThread2->start();
//}

//void Dialog::on_pushButton_14_clicked()
//{
//    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
//}

//void Dialog::on_pushButton_15_clicked()
//{
//    std::fstream fst;
//    fst.open("readpos2.txt", ios::out);

//    qDebug()  << "index:" << ui->tableWidget->rowCount();
//    for(int i=0;i<(ui->tableWidget->rowCount());i++)
//    {
//        qDebug()  << "indexj:" << i;
//        for(int j=0;j<12;j++)
//        {
//            qDebug()  << "index:" << ui->tableWidget->item(i,j)->text();
//            fst << qstrdup(qPrintable(ui->tableWidget->item(i,j)->text()))<< "\t";


//        }
//        fst << "\n";
//    }


//    fst.close();

//}



//void Dialog::on_pushButton_17_clicked()
//{
//    int row;
//    if(ui->tableWidget->currentRow() == -1)
//    {
//        row = ui->tableWidget->rowCount();
//        //qDebug()  << "no select" << row ;
//    }
//    else
//    {
//        row=ui->tableWidget->currentRow()+1;
//    }

//   qDebug()  << "insert:" << row ;
//   //QString s= QString::number(j);
//   ui->tableWidget->insertRow(row);
//   ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("pumpon")));
//   ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString("1")));
//   ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("0")));
//   j=j+1;

//   ui->tableWidget->setCurrentCell(-1,-1);
//}

//void Dialog::on_pushButton_18_clicked()
//{
//    int row;
//    if(ui->tableWidget->currentRow() == -1)
//    {
//        row = ui->tableWidget->rowCount();
//        //qDebug()  << "no select" << row ;
//    }
//    else
//    {
//        row=ui->tableWidget->currentRow()+1;
//    }

//   qDebug()  << "insert:" << row ;
//   QString s= QString::number(j);
//   ui->tableWidget->insertRow(row);
//   ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString("pumpoff")));
//   ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString("1")));
//   ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,10,new QTableWidgetItem(QString("0")));
//   ui->tableWidget->setItem(row,11,new QTableWidgetItem(QString("0")));
//   j=j+1;

//   ui->tableWidget->setCurrentCell(-1,-1);
//}





void Dialog::on_pushButton_36_clicked()
{
    ini_IKFK();
    arm_init();
    if(ui->comboBox_coordinate_type->currentIndex()==0)
    {
        char filename_save[80];
        char filename_open[80];
        strcpy(filename_save,"test_tray_action.txt");
        strcpy(filename_open,"tested_tray_coord.txt");
        remove(filename_save);
        int width_tray=ui->lineEdit_numberW->text().toInt();
        int length_tray=ui->lineEdit_numberL->text().toInt();
        int all_count=width_tray*length_tray;
        double tray_coord[all_count][6];
        tray_coord[all_count][6]={0};
        double tray_coord_fin[all_count*2][3];
        tray_coord_fin[all_count*2][3]={0};
        int tray_pos[2*all_count][8];
        tray_pos[2*all_count][8]={0};

        char line[1024];
        char * pch;
        int count=0;
        fstream fst;
        fst.open(filename_open,ios::in);
        if(!fst)
        {
            cout<<"open error"<<endl;
        }
        while(fst.getline(line, sizeof(line), '\n'))
        {
            if (line[strlen(line)] == '\0')
            {
                int col = 0;
                pch=strtok(line,"\t");
                while(pch != NULL)
                {
                    if(col>5)
                    {
                        break;
                    }
                    else
                    {
                        tray_coord[count][col]=atof(pch);
                        col++;
                    }
                    pch = strtok(NULL, "\t");
                }
            }
            count++;
        }
        fst.close();

        for(int i=0;i<all_count;i++)
        {
            mThread4->coord_cul[0]=tray_coord[i][0];
            mThread4->coord_cul[1]=tray_coord[i][1];
            mThread4->coord_cul[2]=tray_coord[i][2];
            cout<<"coord_cul[0]="<<mThread4->coord_cul[0]<<endl;
            cout<<"coord_cul[1]="<<mThread4->coord_cul[1]<<endl;
            cout<<"coord_cul[2]="<<mThread4->coord_cul[2]<<endl;
            tray_coord_fin[2*i][0]=tray_coord[i][0];
            tray_coord_fin[2*i][1]=tray_coord[i][1];
            tray_coord_fin[2*i][2]=tray_coord[i][2];
            tray_pos[2*i][7]=1;
            mThread4->ox=tray_coord[i][3];
            mThread4->oy=tray_coord[i][4];
            mThread4->oz=tray_coord[i][5];
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst0;
                    fst0.open("readpos.txt", ios::in);
                    while(fst0.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                            pch=strtok(line,"\t");
                            int col2=0;
                            while(pch!=NULL)
                            {
                                if(col2>7)
                                {
                                    break;
                                }
                                else
                                {
                                    tray_pos[i*2][col2/2]=atoi(pch);
                                    if(col2<5)
                                    {
                                        tray_pos[i*2][NUM+col2/2]=tray_coord[i][3+col2/2];
                                        cout<<"tray_pos["<<i*2<<"]["<<4+col2/2<<"]="<<tray_coord[i][3+col2/2]<<endl;
                                    }
                                    col2=col2+2;
                                }
                                pch = strtok(NULL, "\t");
                                pch = strtok(NULL, "\t");
                            }
                           break;
                        }
                    }
                    fst0.close();
                    break;
                }
            }

            /*mThread4->coord_cul[2]=tray_coord[i][2];
            tray_coord_fin[3*i+1][0]=tray_coord[i][0];
            tray_coord_fin[3*i+1][1]=tray_coord[i][1];
            tray_coord_fin[3*i+1][2]=tray_coord[i][2];
            tray_pos[3*i+1][7]=1;
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst1;
                    fst1.open("readpos.txt", ios::in);
                    while(fst1.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                           pch=strtok(line,"\t");
                           int col2=0;
                           while(pch!=NULL)
                           {
                               if(col2>7)
                               {
                                   break;
                               }
                               else
                               {
                                   tray_pos[i*3+1][col2/2]=atoi(pch);
                                   if(col2<5)
                                   {
                                       tray_pos[i*3+1][NUM+col2/2]=tray_coord[i][3+col2/2];
                                   }
                                   col2=col2+2;
                               }
                               pch = strtok(NULL, "\t");
                               pch = strtok(NULL, "\t");
                           }
                          break;
                        }
                    }
                    fst1.close();
                    break;
                }
            }*/

            mThread4->coord_cul[0]=25;
            mThread4->coord_cul[1]=0;
            mThread4->coord_cul[2]=ui->lineEdit_20->text().toFloat();
            tray_coord_fin[2*i+1][0]=25;
            tray_coord_fin[2*i+1][1]=0;
            tray_coord_fin[2*i+1][2]=ui->lineEdit_20->text().toFloat();
            tray_pos[2*i+1][7]=0;
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst2;
                    fst2.open("readpos.txt", ios::in);
                    while(fst2.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                           pch=strtok(line,"\t");
                           int col2=0;
                           while(pch!=NULL)
                           {
                               if(col2>7)
                               {
                                   break;
                               }
                               else
                               {
                                   tray_pos[i*2+1][col2/2]=atoi(pch);
                                   if(col2<5)
                                   {
                                       tray_pos[i*2+1][NUM+col2/2]=tray_coord[i][3+col2/2];
                                   }
                                   col2=col2+2;
                               }
                               pch = strtok(NULL, "\t");
                               pch = strtok(NULL, "\t");
                           }
                          break;
                        }
                    }
                    fst2.close();
                    break;
                }
            }
        }

        cout<<"before write !!!!!!!!!!!!!!"<<endl;
        for(int m=0;m<2*all_count;m++)
        {
            for(int n=0;n<NUM;n++)
            {
                mThread2->Goalpos[n]=tray_pos[m][n];
                if(m%2!=0)
                {
                    mThread2->Nowpos[n]=tray_pos[m-1][n];
                }
                mThread2->Goalo[n]=tray_pos[m][n+NUM];
            }
            cout<<"11111111111111111111111"<<endl;
            if(m%2==0)
            {
                cout<<"222222222222222222222"<<endl;
                mThread2->omega_limit=ui->lineEdit->text().toFloat();
                fstream fst123;
                fst123.open("test_tray_action.txt",ios::out|ios::app);
                if(!fst123)
                {
                    cout<<"fst123 open error11111111111"<<endl;
                }
                else
                {
                    fst123<<"group"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\n";
                }
                fst123.close();
                for(int n=0;n<NUM;n++)
                {
                    mThread2->Nowpos[n]=0;
                }
            }
            mThread2->filename="./readpos.txt";
            mThread2->filename1="./test_tray_action.txt";
            mThread2->times = 1;
            mThread2->state = 0;
            mThread2->cmd=qstrdup(qPrintable(QString("change")));
            mThread2->start();

            while(1)
            {
                if(mThread2->isFinished())
                {
                    fstream fst3;
                    fst3.open("test_tray_action.txt",ios::out|ios::app);
                    fst3<<"("<<tray_coord_fin[m][0]<<","<<tray_coord_fin[m][1]<<","<<tray_coord_fin[m][2]<<")"<<"\t";
                    fst3<<"("<<tray_pos[m][4]<<","<<tray_pos[m][5]<<","<<tray_pos[m][6]<<")"<<"\t";
                    fst3<<"\n";
                    if(tray_pos[m][7]==1)
                    {
                        fst3<<"leftabsorb"<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<"\n";
                    }
                    fst3.close();
                    break;
                }
            }
        }
    }
    else if(ui->comboBox_coordinate_type->currentIndex()==1)
    {
        char filename_save[80];
        char filename_open[80];
        strcpy(filename_save,"pass_tray_action.txt");
        strcpy(filename_open,"tested_tray_coord.txt");
        remove(filename_save);
        int width_tray=ui->lineEdit_numberW->text().toInt();
        int length_tray=ui->lineEdit_numberL->text().toInt();
        int all_count=width_tray*length_tray;
        double tray_coord[all_count][6];
        tray_coord[all_count][6]={0};
        double tray_coord_fin[all_count*2][3];
        tray_coord_fin[all_count*2][3]={0};
        int tray_pos[2*all_count][8];
        tray_pos[2*all_count][8]={0};

        char line[1024];
        char * pch;
        int count=0;
        fstream fst;
        fst.open(filename_open,ios::in);
        if(!fst)
        {
            cout<<"open error"<<endl;
        }
        while(fst.getline(line, sizeof(line), '\n'))
        {
            if (line[strlen(line)] == '\0')
            {
                int col = 0;
                pch=strtok(line,"\t");
                while(pch != NULL)
                {
                    if(col>5)
                    {
                        break;
                    }
                    else
                    {
                        tray_coord[count][col]=atof(pch);
                        col++;
                    }
                    pch = strtok(NULL, "\t");
                }
            }
            count++;
        }
        fst.close();

        for(int i=0;i<all_count;i++)
        {
            mThread4->coord_cul[0]=tray_coord[i][0];
            mThread4->coord_cul[1]=tray_coord[i][1];
            mThread4->coord_cul[2]=ui->lineEdit_20->text().toFloat();
            cout<<"coord_cul[0]="<<mThread4->coord_cul[0]<<endl;
            cout<<"coord_cul[1]="<<mThread4->coord_cul[1]<<endl;
            cout<<"coord_cul[2]="<<mThread4->coord_cul[2]<<endl;
            tray_coord_fin[2*i][0]=tray_coord[i][0];
            tray_coord_fin[2*i][1]=tray_coord[i][1];
            tray_coord_fin[2*i][2]=ui->lineEdit_20->text().toFloat();
            tray_pos[2*i][7]=1;
            mThread4->ox=tray_coord[i][3];
            mThread4->oy=tray_coord[i][4];
            mThread4->oz=tray_coord[i][5];
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst0;
                    fst0.open("readpos.txt", ios::in);
                    while(fst0.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                            pch=strtok(line,"\t");
                            int col2=0;
                            while(pch!=NULL)
                            {
                                if(col2>7)
                                {
                                    break;
                                }
                                else
                                {
                                    tray_pos[i*2][col2/2]=atoi(pch);
                                    if(col2<5)
                                    {
                                        tray_pos[i*2][NUM+col2/2]=tray_coord[i][3+col2/2];
                                        cout<<"tray_pos["<<i*2<<"]["<<4+col2/2<<"]="<<tray_coord[i][3+col2/2]<<endl;
                                    }
                                    col2=col2+2;
                                }
                                pch = strtok(NULL, "\t");
                                pch = strtok(NULL, "\t");
                            }
                           break;
                        }
                    }
                    fst0.close();
                    break;
                }
            }

            mThread4->coord_cul[0]=25;
            mThread4->coord_cul[1]=0;
            mThread4->coord_cul[2]=4;
            mThread4->ox=0;
            mThread4->oy=1;
            mThread4->oz=0;
            tray_coord_fin[2*i+1][0]=25;
            tray_coord_fin[2*i+1][1]=0;
            tray_coord_fin[2*i+1][2]=4;
            tray_pos[2*i+1][7]=0;
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst1;
                    fst1.open("readpos.txt", ios::in);
                    while(fst1.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                           pch=strtok(line,"\t");
                           int col2=0;
                           while(pch!=NULL)
                           {
                               if(col2>7)
                               {
                                   break;
                               }
                               else
                               {
                                   tray_pos[i*2+1][col2/2]=atoi(pch);
                                   if(col2<5)
                                   {
                                       tray_pos[i*2+1][NUM+col2/2]=tray_coord[i][3+col2/2];
                                   }
                                   col2=col2+2;
                               }
                               pch = strtok(NULL, "\t");
                               pch = strtok(NULL, "\t");
                           }
                          break;
                        }
                    }
                    fst1.close();
                    break;
                }
            }
            tray_pos[i*2+1][4]=0;
            tray_pos[i*2+1][5]=1;
            tray_pos[i*2+1][6]=0;
        }




        for(int m=0;m<2*all_count;m++)
        {
            for(int n=0;n<NUM;n++)
            {
                mThread2->Goalpos[n]=tray_pos[m][n];
                if(m%2!=0)
                {
                    mThread2->Nowpos[n]=tray_pos[m-1][n];
                }
                mThread2->Goalo[n]=tray_pos[m][n+NUM];
            }
            if(m%2==0)
            {
                mThread2->omega_limit=ui->lineEdit->text().toFloat();
                fstream fst2;
                fst2.open("pass_tray_action.txt",ios::out|ios::app);
                if(!fst2)
                {
                    cout<<"fst2 open error"<<endl;
                }
                else
                {
                    fst2<<"group"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\n";
                }
                fst2.close();
                for(int n=0;n<NUM;n++)
                {
                    mThread2->Nowpos[n]=0;
                }
            }
            mThread2->filename="./readpos.txt";
            mThread2->filename1="./pass_tray_action.txt";
            mThread2->times = 1;
            mThread2->state = 0;
            mThread2->cmd=qstrdup(qPrintable(QString("change")));
            mThread2->start();

            while(1)
            {
                if(mThread2->isFinished())
                {
                    fstream fst3;
                    fst3.open("pass_tray_action.txt",ios::out|ios::app);
                    fst3<<"("<<tray_coord_fin[m][0]<<","<<tray_coord_fin[m][1]<<","<<tray_coord_fin[m][2]<<")"<<"\t";
                    fst3<<"("<<tray_pos[m][4]<<","<<tray_pos[m][5]<<","<<tray_pos[m][6]<<")"<<"\t";
                    fst3<<"\n";
                    if(tray_pos[m][7]==1)
                    {
                        fst3<<"rightdeflate"<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<"\n";
                    }
                    fst3.close();
                    break;
                }
            }
        }
    }
    else if(ui->comboBox_coordinate_type->currentIndex()==2)
    {
        char filename_save[80];
        char filename_open[80];
        strcpy(filename_save,"fail_tray_action.txt");
        strcpy(filename_open,"fail_tray_coord.txt");
        remove(filename_save);
        int width_tray=ui->lineEdit_numberW->text().toInt();
        int length_tray=ui->lineEdit_numberL->text().toInt();
        int all_count=width_tray*length_tray;
        double tray_coord[all_count][6];
        tray_coord[all_count][6]={0};
        double tray_coord_fin[all_count*2][3];
        tray_coord_fin[all_count*2][3]={0};
        int tray_pos[2*all_count][8];
        tray_pos[2*all_count][8]={0};

        char line[1024];
        char * pch;
        int count=0;
        fstream fst;
        fst.open(filename_open,ios::in);
        if(!fst)
        {
            cout<<"open error"<<endl;
        }
        while(fst.getline(line, sizeof(line), '\n'))
        {
            if (line[strlen(line)] == '\0')
            {
                int col = 0;
                pch=strtok(line,"\t");
                while(pch != NULL)
                {
                    if(col>5)
                    {
                        break;
                    }
                    else
                    {
                        tray_coord[count][col]=atof(pch);
                        col++;
                    }
                    pch = strtok(NULL, "\t");
                }
            }
            count++;
        }
        fst.close();

        for(int i=0;i<all_count;i++)
        {
            mThread4->coord_cul[0]=tray_coord[i][0];
            mThread4->coord_cul[1]=tray_coord[i][1];
            mThread4->coord_cul[2]=ui->lineEdit_20->text().toFloat();
            cout<<"coord_cul[0]="<<mThread4->coord_cul[0]<<endl;
            cout<<"coord_cul[1]="<<mThread4->coord_cul[1]<<endl;
            cout<<"coord_cul[2]="<<mThread4->coord_cul[2]<<endl;
            tray_coord_fin[2*i][0]=tray_coord[i][0];
            tray_coord_fin[2*i][1]=tray_coord[i][1];
            tray_coord_fin[2*i][2]=ui->lineEdit_20->text().toFloat();
            tray_pos[2*i][7]=1;
            mThread4->ox=tray_coord[i][3];
            mThread4->oy=tray_coord[i][4];
            mThread4->oz=tray_coord[i][5];
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst0;
                    fst0.open("readpos.txt", ios::in);
                    while(fst0.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                            pch=strtok(line,"\t");
                            int col2=0;
                            while(pch!=NULL)
                            {
                                if(col2>7)
                                {
                                    break;
                                }
                                else
                                {
                                    tray_pos[i*2][col2/2]=atoi(pch);
                                    if(col2<5)
                                    {
                                        tray_pos[i*2][NUM+col2/2]=tray_coord[i][3+col2/2];
                                        cout<<"tray_pos["<<i*2<<"]["<<4+col2/2<<"]="<<tray_coord[i][3+col2/2]<<endl;
                                    }
                                    col2=col2+2;
                                }
                                pch = strtok(NULL, "\t");
                                pch = strtok(NULL, "\t");
                            }
                           break;
                        }
                    }
                    fst0.close();
                    break;
                }
            }

            mThread4->coord_cul[0]=25;
            mThread4->coord_cul[1]=0;
            mThread4->coord_cul[2]=4;
            mThread4->ox=0;
            mThread4->oy=1;
            mThread4->oz=0;
            tray_coord_fin[2*i+1][0]=25;
            tray_coord_fin[2*i+1][1]=0;
            tray_coord_fin[2*i+1][2]=4;
            tray_pos[2*i+1][7]=0;
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst1;
                    fst1.open("readpos.txt", ios::in);
                    while(fst1.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                           pch=strtok(line,"\t");
                           int col2=0;
                           while(pch!=NULL)
                           {
                               if(col2>7)
                               {
                                   break;
                               }
                               else
                               {
                                   tray_pos[i*2+1][col2/2]=atoi(pch);
                                   if(col2<5)
                                   {
                                       tray_pos[i*2+1][NUM+col2/2]=tray_coord[i][3+col2/2];
                                   }
                                   col2=col2+2;
                               }
                               pch = strtok(NULL, "\t");
                               pch = strtok(NULL, "\t");
                           }
                          break;
                        }
                    }
                    fst1.close();
                    break;
                }
            }
            tray_pos[i*2+1][4]=0;
            tray_pos[i*2+1][5]=1;
            tray_pos[i*2+1][6]=0;
        }




        for(int m=0;m<2*all_count;m++)
        {
            for(int n=0;n<NUM;n++)
            {
                mThread2->Goalpos[n]=tray_pos[m][n];
                if(m%2!=0)
                {
                    mThread2->Nowpos[n]=tray_pos[m-1][n];
                }
                mThread2->Goalo[n]=tray_pos[m][n+NUM];
            }
            if(m%2==0)
            {
                mThread2->omega_limit=ui->lineEdit->text().toFloat();
                fstream fst2;
                fst2.open("fail_tray_action.txt",ios::out|ios::app);
                if(!fst2)
                {
                    cout<<"fst2 open error"<<endl;
                }
                else
                {
                    fst2<<"group"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\n";
                }
                fst2.close();
                for(int n=0;n<NUM;n++)
                {
                    mThread2->Nowpos[n]=0;
                }
            }
            mThread2->filename="./readpos.txt";
            mThread2->filename1="./fail_tray_action.txt";
            mThread2->times = 1;
            mThread2->state = 0;
            mThread2->cmd=qstrdup(qPrintable(QString("change")));
            mThread2->start();

            while(1)
            {
                if(mThread2->isFinished())
                {
                    fstream fst3;
                    fst3.open("fail_tray_action.txt",ios::out|ios::app);
                    fst3<<"("<<tray_coord_fin[m][0]<<","<<tray_coord_fin[m][1]<<","<<tray_coord_fin[m][2]<<")"<<"\t";
                    fst3<<"("<<tray_pos[m][4]<<","<<tray_pos[m][5]<<","<<tray_pos[m][6]<<")"<<"\t";
                    fst3<<"\n";
                    if(tray_pos[m][7]==1)
                    {
                        fst3<<"rightdeflate"<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<"\n";
                    }
                    fst3.close();
                    break;
                }
            }
        }
    }
    else if(ui->comboBox_coordinate_type->currentIndex()==3)
    {
        char filename_save[80];
        char filename_open[80];
        strcpy(filename_save,"cell_action.txt");
        strcpy(filename_open,"cell_coord.txt");
        remove(filename_save);

        int cell_count=ui->lineEdit_16->text().toInt();
        double cell_coord[cell_count][6];
        cell_coord[cell_count][6]={0};
        double cell_coord_fin[cell_count*4][3];
        cell_coord_fin[cell_count*4][3]={0};
        int cell_pos[4*cell_count][8];
        cell_pos[4*cell_count][8]={0};
        
        char line[1024];
        line[1024]={0};
        char * pch;
        int count=0;
        fstream fst;
        fst.open(filename_open,ios::in);
        if(!fst)
        {
            cout<<"open error"<<endl;
        }
        while(fst.getline(line, sizeof(line), '\n'))
        {
            if (line[strlen(line)] == '\0')
            {
                int col = 0;
                pch=strtok(line,"\t");
                while(pch != NULL)
                {
                    if(col>5)
                    {
                        break;
                    }
                    else
                    {
                        cell_coord[count][col]=atof(pch);
                        col++;
                    }
                    pch = strtok(NULL, "\t");
                }
            }
            count++;
        }
        fst.close();
        
        
        for(int i=0;i<cell_count;i++)
        {
            mThread4->coord_cul[0]=cell_coord[i][0];
            mThread4->coord_cul[1]=cell_coord[i][1];
            mThread4->coord_cul[2]=ui->lineEdit_20->text().toFloat();
            cell_coord_fin[4*i][0]=cell_coord[i][0];
            cell_coord_fin[4*i][1]=cell_coord[i][1];
            cell_coord_fin[4*i][2]=ui->lineEdit_20->text().toFloat();
            cell_pos[4*i][7]=0;
            mThread4->ox=cell_coord[i][3];
            mThread4->oy=cell_coord[i][4];
            mThread4->oz=cell_coord[i][5];
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst0;
                    fst0.open("readpos.txt", ios::in);
                    while(fst0.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                            pch=strtok(line,"\t");
                            int col2=0;
                            while(pch!=NULL)
                            {
                                if(col2>7)
                                {
                                    break;
                                }
                                else
                                {
                                    cell_pos[i*4][col2/2]=atoi(pch);
                                    if(col2<5)
                                    {
                                        cell_pos[i*4][NUM+col2/2]=cell_coord[i][3+col2/2];
                                    }
                                    col2=col2+2;
                                }
                                pch = strtok(NULL, "\t");
                                pch = strtok(NULL, "\t");
                            }
                           break;
                        }
                    }
                    fst0.close();
                    break;
                }
            }
            
            mThread4->coord_cul[0]=cell_coord[i][0];
            mThread4->coord_cul[1]=cell_coord[i][1];
            mThread4->coord_cul[2]=cell_coord[i][2];
            cell_coord_fin[4*i+1][0]=cell_coord[i][0];
            cell_coord_fin[4*i+1][1]=cell_coord[i][1];
            cell_coord_fin[4*i+1][2]=cell_coord[i][2];
            cell_pos[4*i+1][7]=1;
            mThread4->ox=cell_coord[i][3];
            mThread4->oy=cell_coord[i][4];
            mThread4->oz=cell_coord[i][5];
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst0;
                    fst0.open("readpos.txt", ios::in);
                    while(fst0.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                            pch=strtok(line,"\t");
                            int col2=0;
                            while(pch!=NULL)
                            {
                                if(col2>7)
                                {
                                    break;
                                }
                                else
                                {
                                    cell_pos[i*4+1][col2/2]=atoi(pch);
                                    if(col2<5)
                                    {
                                        cell_pos[i*4+1][NUM+col2/2]=cell_coord[i][3+col2/2];
                                    }
                                    col2=col2+2;
                                }
                                pch = strtok(NULL, "\t");
                                pch = strtok(NULL, "\t");
                            }
                           break;
                        }
                    }
                    fst0.close();
                    break;
                }
            }
            
            mThread4->coord_cul[0]=cell_coord[i][0]-3.2;
            mThread4->coord_cul[1]=cell_coord[i][1]+7.5;
            mThread4->coord_cul[2]=cell_coord[i][2]+2;
            cell_coord_fin[4*i+2][0]=cell_coord[i][0]-3.2;
            cell_coord_fin[4*i+2][1]=cell_coord[i][1]+7.5;
            cell_coord_fin[4*i+2][2]=cell_coord[i][2]+2;
            mThread4->ox=1;
            mThread4->oy=0;
            mThread4->oz=0;
            cell_pos[4*i+2][7]=2;
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                if(mThread4->isFinished()==1)
                {
                    fstream fst0;
                    fst0.open("readpos.txt", ios::in);
                    while(fst0.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                            pch=strtok(line,"\t");
                            int col2=0;
                            while(pch!=NULL)
                            {
                                if(col2>7)
                                {
                                    break;
                                }
                                else
                                {
                                    cell_pos[i*4+2][col2/2]=atoi(pch);
                                    if(col2<5)
                                    {
                                        cell_pos[i*4+2][NUM+col2/2]=cell_coord[i][3+col2/2];
                                    }
                                    col2=col2+2;
                                }
                                pch = strtok(NULL, "\t");
                                pch = strtok(NULL, "\t");
                            }
                           break;
                        }
                    }
                    fst0.close();
                    break;
                }
            }
            cout<<"!!!!!!!!!!!!!!!!!111"<<endl;
            cell_pos[i*4+2][4]=1;
            cell_pos[i*4+2][5]=0;
            cell_pos[i*4+2][6]=0;

            mThread4->coord_cul[0]=cell_coord[i][0]-3.2;
            mThread4->coord_cul[1]=cell_coord[i][1]+7.5;
            mThread4->coord_cul[2]=ui->lineEdit_20->text().toFloat();
            cell_coord_fin[4*i+3][0]=cell_coord[i][0]-3.2;
            cell_coord_fin[4*i+3][1]=cell_coord[i][1]+7.5;
            cell_coord_fin[4*i+3][2]=ui->lineEdit_20->text().toFloat();
            mThread4->ox=1;
            mThread4->oy=0;
            mThread4->oz=0;
            cell_pos[4*i+3][7]=0;
            mThread4->C = 'c';
            mThread4->Check = true;
            mThread4->start();
            while(1)
            {
                //cout<<"22222222222222222222222\n\n\n";
                if(mThread4->isFinished()==1)
                {
                    fstream fst0;
                    fst0.open("readpos.txt", ios::in);
                    while(fst0.getline(line, sizeof(line), '\n'))
                    {
                        if (line[strlen(line)] == '\0')
                        {
                            pch=strtok(line,"\t");
                            int col2=0;
                            while(pch!=NULL)
                            {
                                if(col2>7)
                                {
                                    break;
                                }
                                else
                                {
                                    cell_pos[i*4+3][col2/2]=atoi(pch);
                                    if(col2<5)
                                    {
                                        cell_pos[i*4+3][NUM+col2/2]=cell_coord[i][3+col2/2];
                                    }
                                    col2=col2+2;
                                }
                                pch = strtok(NULL, "\t");
                                pch = strtok(NULL, "\t");
                            }
                           break;
                        }
                    }
                    fst0.close();
                    break;
                }
            }
            cell_pos[i*4+3][4]=1;
            cell_pos[i*4+3][5]=0;
            cell_pos[i*4+3][6]=0;

        }

        for(int m=0;m<4*cell_count;m++)
        {
            for(int n=0;n<NUM;n++)
            {
                mThread2->Goalpos[n]=cell_pos[m][n];
                if(m%4!=0)
                {
                    mThread2->Nowpos[n]=cell_pos[m-1][n];
                }
                mThread2->Goalo[n]=cell_pos[m][n+NUM];
            }
            if(m%4==0)
            {
                fstream fst2;
                fst2.open(filename_save,ios::out|ios::app);
                if(!fst2)
                {
                    cout<<"fst2 open error"<<endl;
                }
                else
                {
                    fst2<<"group"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\t"<<"\n";
                }
                fst2.close();
                for(int n=0;n<NUM;n++)
                {
                    mThread2->Nowpos[n]=0;
                }
            }
            mThread2->filename="./readpos.txt";
            mThread2->filename1=filename_save;
            mThread2->times = 1;
            mThread2->state = 0;
            mThread2->cmd=qstrdup(qPrintable(QString("change")));
            mThread2->start();

            while(1)
            {
                if(mThread2->isFinished())
                {
                    fstream fst3;
                    fst3.open(filename_save,ios::out|ios::app);
                    fst3<<"("<<cell_coord_fin[m][0]<<","<<cell_coord_fin[m][1]<<","<<cell_coord_fin[m][2]<<")"<<"\t";
                    fst3<<"("<<cell_pos[m][4]<<","<<cell_pos[m][5]<<","<<cell_pos[m][6]<<")"<<"\t";
                    fst3<<"\n";
                    if(cell_pos[m][7]==1)
                    {
                            fst3<<"together"<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<"\n";
                    }
                    else if(cell_pos[m][7]==2)
                    {

                        fst3<<"ready"<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<0<<"\t"<<"\n";
                    }

                    fst3.close();
                    break;
                }
            }
        }


    }

}


//safty return ro the start point
void Dialog::on_pushButton_37_clicked()
{

    ini_IKFK();
    arm_init();

    mThread2->filename="./readpos.txt";
    mThread2->times = 1;
    mThread2->state = 0;
    mThread2->cmd=qstrdup(qPrintable(QString("find_point")));
    mThread2->start();
    while(1)
    {
        if(mThread2->isFinished())
        {
            for(int i=0;i<4;i++)
            {
                mThread4->now_theta_dialog[i]=mThread2->now_theta_dialog[i];
            }
            mThread4->mode=2;
            mThread4->Check = true;
            mThread4->start();

            while(1)
            {
                if(mThread4->isFinished())
                {
                    cout<<"go to now point up"<<endl;
                    mThread4->mode=0;
                    mThread2->filename="./readpos.txt";
                    mThread2->times = 1;
                    mThread2->state = 0;
                    mThread2->cmd=qstrdup(qPrintable(QString("syncw")));
                    mThread2->start();
                    while(1)
                    {
                        if(mThread2->isFinished())
                        {
                            delay(3000);
                            cout<<"first step is done"<<endl;
                            mThread4->C = 't';
                            mThread4->coord[0]=25;
                            mThread4->coord[1]=0;
                            mThread4->coord[2]=ui->lineEdit_20->text().toFloat();
                            mThread4->ox = 1;
                            mThread4->oy = 0;
                            mThread4->oz = 0;
                            mThread4->Check = true;
                            mThread4->start();
                            while(1)
                            {
                                if(mThread4->isFinished())
                                {
                                    cout<<"go to the first point up"<<endl;
                                    mThread2->filename="./readpos.txt";
                                    mThread2->times = 1;
                                    mThread2->state = 0;
                                    mThread2->cmd=qstrdup(qPrintable(QString("syncw")));
                                    mThread2->start();
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }


    while(1)
    {
        if(mThread2->isFinished())
        {
            delay(3000);
            mThread4->C = 't';
            //mThread4->type=2;
            //mThread4->coord[0] = ui->lineEdit_8->text().toDouble();
            //mThread4->coord[1] = ui->lineEdit_9->text().toDouble();
            //mThread4->coord[2] = ui->lineEdit_10->text().toDouble();
            mThread4->coord[0]=25;
            mThread4->coord[1]=0;
            mThread4->coord[2]=3.5;
            //mThread4->ox = ui->lineEdit_ox->text().toFloat();
            //mThread4->oy = ui->lineEdit_oy->text().toFloat();
            //mThread4->oz = ui->lineEdit_oz->text().toFloat();
            mThread4->ox = 0;
            mThread4->oy = 1;
            mThread4->oz = 0;
            mThread4->Check = true;
            mThread4->start();

            while(1)
            {
                if(mThread4->isFinished())
                {
                    //cout<<"before 2 is start,4 is finished\n";
                    mThread2->filename="./readpos.txt";
                    mThread2->times = 1;
                    mThread2->state = 0;
                    mThread2->cmd=qstrdup(qPrintable(QString("syncw")));
                    mThread2->start();

                    while(1)
                    {
                        if(mThread2->isFinished())
                        {
                            mThread6->start_point=1;
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }

}


