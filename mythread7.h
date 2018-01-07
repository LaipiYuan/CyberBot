#ifndef MYTHREAD7_H
#define MYTHREAD7_H

#include <QThread>
#include <QObject>
#include <QWidget>

class MyThread7 : public QThread
{
    Q_OBJECT
public:
    explicit MyThread7(QObject *parent = 0);
    void run();
    bool Exit;

    int baudrate;
    char* parity;
    int id;

    bool leftabsorb7;
    bool rightabsorb7;
    bool leftdeflate7;
    bool rightdeflate7;
    bool down7;
    bool up7;

    //bool signal;
    //char* com;


private:
    QString dev;

signals:
    void NumberChanged7(int number);

public slots:
    //function
    void set_PLC_Dev(QString dev);
};

#endif // MYTHREAD7_H
