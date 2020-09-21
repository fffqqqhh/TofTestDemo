#ifndef USER_GLOBALFUNC_H
#define USER_GLOBALFUNC_H

#include <QWidget>
#include <QSerialPort>

class user_globalfunc
{
public:
    user_globalfunc();
    ~user_globalfunc();

    void SetPortName(QString portName);
    QString GetPortName();

private:
    QString _portName;
};

extern user_globalfunc *globalData;

#endif // USER_GLOBALFUNC_H
