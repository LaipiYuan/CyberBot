#ifndef MYTHREAD3_H
#define MYTHREAD3_H

#include <QThread>

class MyThread3 : public QThread
{
    Q_OBJECT
public:
    explicit MyThread3(QObject *parent = 0);
    void run();
    char* host;
    char* port;
    char* str;
    char* filename;

signals:
    void NumberChanged3(int);
public slots:

};

#endif // MYTHREAD3_H
