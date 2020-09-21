#include "TofDemo.h"
#include <QApplication>
#include "user_globalfunc.h"

user_globalfunc *globalData;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    globalData = new user_globalfunc();

    w.show();

    return a.exec();
}
