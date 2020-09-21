#ifndef USER_SERIALPORT_H
#define USER_SERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include "user_globalfunc.h"

#define SEND_DATA_LEN   5

class user_serialport : public QObject
{
    Q_OBJECT
public:
    explicit user_serialport(QObject *parent = nullptr);
    ~user_serialport();

    void SendData(char *dataBuf,int dataLen);
    void ReadSerialData();

    QSerialPort *getSerialPort();

    char _sendDataBuf[SEND_DATA_LEN];

private:
    QByteArray _myReadBuf;
    QSerialPort *_mySerial;

signals:

public slots:
};

#endif // USER_SERIALPORT_H
