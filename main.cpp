#include "dialog.h"
#include <QApplication>
#include "mythread1.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;

    w.setWindowTitle(QString("CyberRobot"));
    //w.setWindowFlags(w.windowFlags()& ~Qt::WindowMaximizeButtonHint&~Qt::WindowMinimizeButtonHint);
    w.window()->resize(1024,720);
    w.show();
    a.quit();

    return a.exec();
}
