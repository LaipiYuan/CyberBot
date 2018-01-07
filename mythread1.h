#ifndef MYTHREAD1_H
#define MYTHREAD1_H

#include <QThread>

class MyThread1 : public QThread
{
    Q_OBJECT
public:
    explicit MyThread1(QObject *parent = 0);
    void run();
    bool Stop;
    char* host;
    char* port;
    int times;
    char* filename;

signals:
    void NumberChanged(int);

public slots:

};

#endif // MYTHREAD1_H
