#ifndef DIALOG_H
#define DIALOG_H
#include "mythread1.h"
#include "mythread2.h"
#include "mythread3.h"
#include "mythread4.h"
#include "mythread5.h"
#include "mythread6.h"
#include "mythread7.h"
#include "mythread8.h"

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    MyThread1 *mThread;
    MyThread2 *mThread2;
    MyThread3 *mThread3;
    MyThread4 *mThread4;
    mythread5 *mThread5;
    MyThread6 *mThread6;
    MyThread7 *mThread7;
    MyThread8 *mThread8;

    int group;
    int torque_tmp;
    int valve_tmp;
    //static Ui::Dialog * ui;

private slots:
   // void on_pushButton_clicked();
    void tableclicked(int ,int);
    void tableclicked2(int ,int);
    void onNumberChanged(int);
    void onNumberChanged2(int);
    void onNumberChanged3(int);
    void onNumberChanged4(int);
    void onNumberChanged5(int);
    void onNumberChanged6(int);
    void onNumberChanged7(int);
    void onNumberChanged8(int);
    void socket_initialization();
    void clear_table();
    void ini_IKFK();
    void arm_init();
    int browse();
    void browse_config();
    void load_pos();
    void optimize_loadfile(int);
    void socket_load();
    void plain_end();
    void call_suck_on(int);
    void call_suck_off(int);
    void call_gpio_on(int);//***gpio
    void call_gpio_off(int);//***gpio
    void setconfig(QString itemPath);
    void IKFK_parameter(char C,int type, bool Check);
    float joint_pos_range(int type);
    float joint_gear_ratio(int type);
    int joint_vel_limit(int type);
    int cw(int type);

    void modbus_initialization();

    void planning_initialization();

    void getnumber(int);
    void GetTemp(int , int );
    void GetError(int, double);

    void Modbus_mode(int);

    //void on_pushButton_1_clicked();

    //void on_pushButton_2_clicked();

    //void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    //void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_4_clicked();

    //void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();


    //void on_pushButton_10_clicked();

    //void on_pushButton_11_clicked();

    //void on_pushButton_12_clicked();

    //void on_pushButton_14_clicked();

    //void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_13_clicked();

    //void on_pushButton_17_clicked();

    //void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_48_clicked();

    void on_pushButton_47_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_49_clicked();

    void on_pushButton_50_clicked();

    //void on_pushButton_51_clicked();

    void on_pushButton_52_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();


    void on_pushButton_41_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_42_clicked();

    void on_pushButton_45_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_46_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_35_clicked();

    //void on_pushButton_36_clicked();

    void on_pushButton_read_group_clicked();

    void on_pushButton_start_group_clicked();

    void on_pushButton_play_group_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_add_group_clicked();

    //void on_add_group_clicked();

    void on_pushButton_initialize_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

    //void on_pushButton_15_clicked();

    void on_pushButton_ready_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_GPIO_on_clicked();

    void on_pushButton_GPIO_off_clicked();


    void on_pushButton_mythread2_stop_clicked();

    void on_pushButton_enter_clicked();
    void on_pushButton_modify_zvec_clicked();
    void on_pushButton_generate_clicked();
    void on_pushButton_51_clicked();



   // void on_comboBox_gpioChoose_activated(const QString &arg1);

    void on_pushButton_choose_mode_clicked();

    void on_pushButton_cylinder_down_clicked();
    void on_pushButton_cylinder_up_clicked();
    void on_pushButton_left_vacuum_clicked();
    void on_pushButton_left_flow_clicked();
    void on_pushButton_right_vacuum_clicked();
    void on_pushButton_right_flow_clicked();

    void on_pushButton_36_clicked();

    void on_pushButton_37_clicked();


private:
    Ui::Dialog *ui;
    QString get_PLC_Dev();

//protected:
    //Ui::Dialog *ui;
};

#endif // DIALOG_H
