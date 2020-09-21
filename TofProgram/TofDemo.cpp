#include "TofDemo.h"
#include "ui_TofDemo.h"

static user_procotol ReadData;
static user_procotol SendData;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setWindowTitle("Unipal Tof Test");
    this->setMaximumSize(windowWidth,windowWidth*0.618);
    this->setMinimumSize(windowWidth,windowWidth*0.618);

    //this->setStyleSheet("background-color: gray");

    tofSendCmdTimer = new QTimer(this);
    tofSendCmdTimer->stop();
    connect(tofSendCmdTimer,&QTimer::timeout,this,&Widget::handlerTimeout);

    QFont font("Simsun",10);
    ui->comboBox_portName->setMinimumSize(90,40*0.618);
    ui->comboBox_portName->setFont(font);

    ui->pushButton_scan->setMinimumSize(90,40*0.618);
    ui->pushButton_scan->setText("ScanPort");
    ui->pushButton_scan->setFont(font);

    ui->pushButton_switch->setMinimumSize(90,40*0.618);
    ui->pushButton_switch->setText("OpenPort");
    ui->pushButton_switch->setFont(font);
    ui->pushButton_switch->setStyleSheet(BTN_BORDED_LESS);
    ui->pushButton_switch->setStyleSheet(BTN_BACK_COLOR_GREEN);

    ui->label_distance->setMinimumSize(90,40*0.618);
    //ui->label_distance->setStyleSheet("QLabel{border:1px solid black}");
    ui->label_distance->setAlignment(Qt::AlignCenter);
    ui->label_distance->setText("距离/状态");
    ui->label_distance->setFont(QFont("Simsun",8));

    ui->label_distanceData->setMinimumSize(40,40*0.618);
    //ui->label_distanceData->setStyleSheet("QLabel{border:1px solid black}");
    ui->label_distanceData->setAlignment(Qt::AlignCenter);
    ui->label_distanceData->setText("0");
    ui->label_distanceData->setFont(QFont("Simsun",12));
    ui->label_distanceData->setStyleSheet("color:blue");
    //ui->label_distanceData->setStyleSheet("QLabel{border:1px solid black}""QLabel{color:blue}");

    ui->label_unit->setMinimumSize(30,40*0.618);
    ui->label_unit->setAlignment(Qt::AlignCenter);
    ui->label_unit->setText("mm");
    ui->label_unit->setFont(QFont("Simsun",10));
    //ui->label_unit->setStyleSheet("QLabel{border:1px solid black}");

    ui->label_status->setMinimumSize(80,40*0.618);
    ui->label_status->setAlignment(Qt::AlignCenter);
    ui->label_status->setText("未知状态");
    ui->label_status->setFont(QFont("Simsun",8));
    ui->label_status->setStyleSheet("color:blue");

    ui->pushButton_read->setMinimumSize(90,40*0.618);
    ui->pushButton_read->setText("开始读取");
    ui->pushButton_read->setFont(font);
    ui->pushButton_read->setStyleSheet(BTN_BORDED_LESS);
    ui->pushButton_read->setStyleSheet(BTN_BACK_COLOR_GREEN);
    ui->pushButton_read->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

/**
 * @brief Widget::SerialPortReadData
 */
void Widget::SerialPortReadData(){
    int readDataNum = 0;
    char readBuf[10] = {0};

    readDataNum = tofSerial->getSerialPort()->read(readBuf,10);
    ReadDataFunc(readBuf,readDataNum);
}

/**
 * @brief 对接收到的数据的处理
 * @param dataBuf
 * @param dataLen
 */
void Widget::ReadDataFunc(char *dataBuf, int dataLen){
    int unPackFlag = 0;

    enum command tempCmd;
    unsigned char* tempDataBuf = (unsigned char*)dataBuf;
    int data = 0;
    unsigned char dataStatus = 0;

    unPackFlag = ReadData.UnPackage(tempDataBuf,dataLen);
    if(unPackFlag != 0){
        QMessageBox::warning(this,"Warning","解包失败",QMessageBox::Yes);
        return;
    }

    if(ReadData.getReadStatus()){
        ReadData.SetReadStatus(false);

        tempCmd = ReadData.GetCommand();
        switch (tempCmd) {
        case MeasurDistance:
            if(0x03 == tempDataBuf[3]){
                data = tempDataBuf[4]<<8|tempDataBuf[5];
                ui->label_distanceData->setText(QString::number(data));
                //qDebug()<<data;

                dataStatus = tempDataBuf[6];
                switch (dataStatus) { //这个后面也要改成enum
                case 0x00:
                    ui->label_status->setText("正常");
                    break;
                case 0x01:
                    ui->label_status->setText("VCSEL短路");
                    break;
                case 0x02:
                    ui->label_status->setText("物体反射光小");
                    break;
                case 0x04:
                    ui->label_status->setText("反射/干扰之比小");
                    break;
                case 0x08:
                    ui->label_status->setText("干扰光很大");
                    break;
                case 0x10:
                    ui->label_status->setText("环绕错误");
                    break;
                case 0x20:
                    ui->label_status->setText("内部计算错误");
                    break;
                case 0x80:
                    ui->label_status->setText("面板串扰大");
                    break;
                default:
                    ui->label_status->setText("未知状态");
                    break;
                }
            }

            memset(dataBuf,0,dataLen);
            break;
        default:
            break;
        }
    }
}

/**
 * @brief 扫描串口，在combobox里添加串口号
 */
void Widget::ScanSerialPort(){
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        QSerialPort tempSerial;
        tempSerial.setPort(info);

        if(tempSerial.open(QIODevice::ReadWrite)){
            ui->comboBox_portName->addItem(tempSerial.portName());
            tempSerial.close();
        }
    }
}

void Widget::SendComFunc(enum command cmd){
    int sendDataLen = 0;

    SendData.SetCommand(cmd);

    switch (cmd) {
    case MeasurDistance: //读取距离
        SendData.SetDataLen(0);

        break;
    default:
        break;
    }

    sendDataLen = SendData.Package(tofSerial->_sendDataBuf);
    //qDebug()<<sendDataLen;

    tofSerial->SendData(tofSerial->_sendDataBuf,sendDataLen);
}

void Widget::on_pushButton_switch_clicked()
{
    if(ui->pushButton_switch->text() == "OpenPort"){
        if(ui->comboBox_portName->currentText() == ""){
            QMessageBox::warning(this,"WARNING","Port Is Null!!!",QMessageBox::Yes);
        }
        else{
            globalData->SetPortName(ui->comboBox_portName->currentText());

            tofSerial = new user_serialport();
            if(tofSerial->getSerialPort()->isOpen()){
                ui->pushButton_switch->setText("ClosePort");
                ui->pushButton_switch->setStyleSheet(BTN_BACK_COLOR_RED);

                ui->pushButton_scan->setEnabled(false);
                ui->comboBox_portName->setEnabled(false);

                ui->pushButton_read->setEnabled(true);

                connect(tofSerial->getSerialPort(),QSerialPort::readyRead,this,&Widget::SerialPortReadData);
            }
        }
    }
    else{
        tofSerial->getSerialPort()->clear();
        tofSerial->getSerialPort()->close();

        tofSendCmdTimer->stop();

        ui->pushButton_switch->setText("OpenPort");
        ui->pushButton_switch->setStyleSheet(BTN_BACK_COLOR_GREEN);

        ui->pushButton_scan->setEnabled(true);
        ui->comboBox_portName->setEnabled(true);

        ui->pushButton_read->setText("开始读取");
        ui->pushButton_read->setStyleSheet(BTN_BACK_COLOR_GREEN);
        ui->pushButton_read->setEnabled(false);
    }
}

void Widget::on_pushButton_scan_clicked()
{
    ui->comboBox_portName->clear();
    ScanSerialPort();
}

void Widget::on_pushButton_read_clicked()
{
    if(ui->pushButton_read->text() == "开始读取"){
        tofSendCmdTimer->start(100);

        ui->pushButton_read->setText("结束读取");
        ui->pushButton_read->setStyleSheet(BTN_BACK_COLOR_RED);
    }
    else if(ui->pushButton_read->text() == "结束读取"){
        tofSendCmdTimer->stop();

        ui->pushButton_read->setText("开始读取");
        ui->pushButton_read->setStyleSheet(BTN_BACK_COLOR_GREEN);
    }
}

/**
 * @brief 定时器到时处理
 */
void Widget::handlerTimeout(){
    if(tofSendCmdTimer->isActive()){
        if(timeoutValue < 60000){
            timeoutValue++;

            if(timeoutValue >= 1){
                timeoutValue = 0;

                if(tofSerial->getSerialPort()->isOpen()){
                    SendComFunc(MeasurDistance);
                }
            }
        }
    }
}
