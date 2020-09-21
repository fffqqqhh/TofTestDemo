#ifndef TOFDEMO_H
#define TOFDEMO_H

#include <QWidget>
#include <QFont>
#include <QMessageBox>
#include <QTimer>
#include <QIcon>

#include "user_globalfunc.h"
#include "user_procotol.h"
#include "user_serialport.h"

#define windowWidth 400

#define BTN_BORDED_LESS "QPushButton{border-style:flat}" //按键无边框
#define BTN_BACK_COLOR_GREEN "background:rgb(100,160,100)" //背景色为绿色
#define BTN_BACK_COLOR_RED "background:rgb(210,65,45)" //背景色为红色

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void ScanSerialPort();
    void SendComFunc(enum command cmd);
    void ReadDataFunc(char* dataBuf,int dataLen);

    bool readFlagStatus;
    int timeoutValue;

private slots:
    void SerialPortReadData();

    void on_pushButton_switch_clicked();

    void on_pushButton_scan_clicked();

    void on_pushButton_read_clicked();

    void handlerTimeout();

private:
    Ui::Widget *ui;

    user_serialport *tofSerial;

    QTimer *tofSendCmdTimer;
};

#endif // TOFDEMO_H
