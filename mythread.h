#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>

class mythread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = 0);
    void run();
signals:
    void NumberChanged(int);

public slots:
};

#endif // MYTHREAD_H

//

