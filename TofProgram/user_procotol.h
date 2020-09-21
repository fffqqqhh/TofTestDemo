#ifndef USER_PROCOTOL_H
#define USER_PROCOTOL_H

#define PACKAGE_DATA_LEN    10 //一帧数据的长度为8(发送为5，接收为8[正常情况下])
#define PACKET_HEADER_ONE   0x55
#define PACKET_HEADER_TWO   0xAA
#define PACKET_END_FLAG     0xFA

enum command{
    CommandUndef = 0x80,
    MeasurDistance = 0x81, //测量距离
    CrosstalkCalibrat = 0x82, //串扰校准
    OffsetCalibrat = 0x83, //失调校准
    ResetTofModule = 0x84, //重置tof模块
    ReadFactoryData = 0x85, //读取工厂数据
    ReadVersion = 0x86, //阅读版本信息
};

class user_procotol
{
public:
    user_procotol();
    ~user_procotol();

    int Package(char* data);
    int UnPackage(unsigned char* data,int dataLen);

    void SetCommand(enum command cmd);
    void SetBuf(unsigned char* data,int dataLen);
    void SetDataLen(int dataLen);
    void SetReadStatus(bool status);

    enum command GetCommand();
    unsigned char* GetBuf();//获取包体信息
    short int GetDataLen();
    bool getReadStatus();

private:
    enum command _command;
    short int _dataLen;

    unsigned char _dataBuf[PACKAGE_DATA_LEN];
    bool _readFnish;
};

#endif // USER_PROCOTOL_H
