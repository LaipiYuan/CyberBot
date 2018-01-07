#ifndef MYTHREAD6_H
#define MYTHREAD6_H

#include <QThread>
#include <QObject>
#include <QWidget>

class MyThread6 : public QThread
{
    Q_OBJECT
public:
    explicit MyThread6(QObject *parent = 0);
    void run();
    bool Exit;
    int Scandone_check;    //=>mythread2

    int baudrate;
    char* parity;
    int id;
    int cell_max_num;
    int tested_tray_num;
    int pass_tray_num;
    int fail_tray_num;


    int group;          //=>dialog

    bool move;          //<=mythread2
    bool check;         //<=dialog
    bool scan_ready;    //<=mythread2
    int mode_gpio;
    int done;
    int allcount;
    int start_point;

    //bool signal;
    //char* com;


private:
    QString dev;

signals:
    void NumberChanged6(int number);

public slots:
    //function
    void set_PLC_Dev(QString dev);
};

#endif // MYTHREAD6_H
