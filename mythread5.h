#ifndef MYTHREAD5_H
#define MYTHREAD5_H

#include <QThread>

class mythread5 : public QThread
{
    Q_OBJECT
public:
    explicit mythread5(QObject *parent = 0);
    void run();
    char* port;
    char filename[100];
    char path_message[100];
    int file_start;
    char* message;
    bool Check;
    char case_option[50];
    char id[50];
    char option[20];
    int type;
    bool Exit;
    double coord[3];

    int change_step_now;
    int change_step_pre;
    int change_group;

    int replay_times;
    float time_cell;
    float omega_limit;
    int user_sync_type;
    float move_time;
    float unit;
    float angle;
    int group;

    int vig[4];
    int vpg[4];
    int ppg[4];

    float ax;
    float ay;
    float az;
    float ox;
    float oy;
    float oz;

    float gear_ratio[4];
    float pos_range[4];
    float adj_joint[4];
    int cw[4];
    int vel_limit[4];
signals:
    void NumberChanged5(int);
public slots:


};

#endif // MYTHREAD5_H
