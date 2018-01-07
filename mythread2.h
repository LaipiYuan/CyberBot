#ifndef MYTHREAD2_H
#define MYTHREAD2_H

#include <QThread>

class MyThread2 : public QThread
{
    Q_OBJECT
public:
    explicit MyThread2(QObject *parent = 0);
    void run();
    char* host;
    char* port;
    char* filename;
    char* filename1;
    char* ID;
    char* dev;
    int step_span;
    int delay_time;
    int group;
    int state;
    int allcount2;
    char* cmd;
    float omega_limit;
    float move_time;
    float scale;
    //float move_time=0;
    int times;
    int vig[4];
    int vpg[4];
    int ppg[4];
    int joint_use[4];
    float gear_ratio[4];
    float pos_range[4];
    int vel_limit[4];
    int isnewvalue;
    int change_pos[4];
    int change_step_now;
    int change_step_pre;
    int change_group;
    int num;
    double first_reality_theta[4];
    int Goalpos[4];
    int Nowpos[4];
    int Goalo[3];
    double now_theta_dialog[4];
    int height1;

    bool move;
    bool check;
    volatile bool scan_ready;        //=>mythread6
    int Scandone_check;    //<=mythread6

    bool stop;
    int done2;
    bool gpio;


    int syncw(int retimes,char filename[80]);
    int readpos();
    int sync2write(int length, int step, int GoalPos[100][4], int GoalVelo[100][4], int retimes);
    int group_sync2write(int length, int step, int retimes, int group);
    void motion(double readtime,int step,int motion_velo[4],int motion_posi[4]);



signals:
    void NumberChanged2(int);
    void sendnumber(int);
    void SendTemp(int, int);
    void SendError(int,double);
    void test(QString value);

public slots:

};

#endif // MYTHREAD2_H
