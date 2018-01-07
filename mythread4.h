#ifndef MYTHREAD4_H
#define MYTHREAD4_H
#include "mythread2.h"

#include <QThread>

class MyThread4 : public QThread
{
    Q_OBJECT
public:
    explicit MyThread4(QObject *parent = 0);

    //MyThread2 *mThread5;
    void run();
    bool Check;
    double theta[4];
    double first_goal_theta[4];
    double theta2value[4];
    double coord[3];
    double value;
    //double ax, ay, az;
    double ox, oy, oz;
    int mode;
    char* Case_option;
    float omega_limit;
    char C;
    int type;
    int user_sync_type;
    float move_time;
    float unit;
    float angle;
    float gear_ratio[4];
    float pos_range[4];
    float adj_joint[4];
    int cw[4];
    int posture;    //4ax posture, 1:right, 2:left
    double coord_cul[3];
    int o_cul[3];
    double now_theta_dialog[4];
    double now_coord_dialog[3];



signals:
    void NumberChanged4(int);

public slots:

};

#endif // MYTHREAD4_H
