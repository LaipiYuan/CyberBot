#ifndef DIALOG2_H
#define DIALOG2_H
#include "mythread1.h"
#include "mythread2.h"
#include "mythread3.h"
#include "mythread4.h"

#include <QDialog>

namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = 0);
    ~Dialog2();
    MyThread4 *mThread4;
    MyThread2 *mThread6;


private slots:
    void ini_IKFK();
    void onNumberChanged4(int);
    void onNumberChanged2(int);

    void on_pushButton_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_13_clicked();

    void plain_end();


    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

private:
    Ui::Dialog2 *ui;
};

#endif // DIALOG2_H
