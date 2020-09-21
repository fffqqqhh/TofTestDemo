#include "user_serialport.h"

user_serialport::user_serialport(QObject *parent) : QObject(parent)
{
    _mySerial = new QSerialPort();

    _mySerial->setPortName(globalData->GetPortName());
    _mySerial->setBaudRate(QSerialPort::Baud9600);
    _mySerial->setDataBits(QSerialPort::Data8);
    _mySerial->setStopBits(QSerialPort::OneStop);
    _mySerial->setParity(QSerialPort::NoParity);
    _mySerial->setFlowControl(QSerialPort::NoFlowControl);

    if(_mySerial->open(QIODevice::ReadWrite)){
        qDebug()<<"port is open";
    }
    else{
        QMessageBox::warning(NULL,"WARNING","Port Error",QMessageBox::Yes);
    }
}

user_serialport::~user_serialport(){
    _mySerial->clear();
    _mySerial->close();
}

QSerialPort *user_serialport::getSerialPort(){
    return _mySerial;
}

/**
 * @brief 通过串口发送数据
 * @param dataBuf:要发送的数据指针
 * @param dataLen:要发生的数据长度
 */
void user_serialport::SendData(char *dataBuf, int dataLen){
    _mySerial->write(dataBuf,dataLen);
}

/**
 * @brief 串口读取数据，这个函数暂时没有用到
 */
void user_serialport::ReadSerialData(){
    _myReadBuf = _mySerial->readAll();
}
