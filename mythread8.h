#ifndef MYTHREAD8_H
#define MYTHREAD8_H

#include <QThread>

class MyThread8 : public QThread
{
    Q_OBJECT
public:
    explicit MyThread8(QObject *parent = 0);
    void run();

    /*
    int user_sync_type;
    float move_time;
    float omega_limit;
    float unit;
    float angle;
    int group;
    int step_span;
    int delay_time;
    float scale;

    int joint_use[6];
    float gear_ratio[6];
    float pos_range[6];
    int vel_limit[6];
    */

    char* cmd;
    int coordinate_type;
    char* filename;
    int modify_Z_use;
    int middleCoordUse;
    int shiftUse;

    float tray_length;
    float tray_width;
    int number_L;
    int number_W;
    int rowW;
    int columnL;

    double coord_1[3];
    double coord_2[3];
    double coord_3[3];
    double coord_4[3];
    double coord_5[3];
    double coord_6[3];
    double normAx_n[3];
    int coord_o[3];

    void tray_coordinate(double coord_o[3], double coord_w[3], double coord_l[3], double coord_d[3], double coord_5[3], double coord_6[3], int numberL, int numberW, int column, int row, char filename[80], int coord_o2[3], int middleCoordUsed, int shiftUsed);


signals:
    void NumberChanged8(int number);
public slots:

};

#endif // MYTHREAD8_H
