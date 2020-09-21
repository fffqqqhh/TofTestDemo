/*
 * 夏普tof协议
 */
#include "user_procotol.h"
#include <stdlib.h>
#include <string.h>
#include <QDebug>

user_procotol::user_procotol()
{
    SetCommand(CommandUndef);
    //_command = 0;
    SetDataLen(0);
    //_dataLen = 0;
    SetReadStatus(false);
    //_readFnish = false;
    memset(_dataBuf,0,PACKAGE_DATA_LEN);
}

user_procotol::~user_procotol(){

}

/**
 * @brief 获取命令值
 * @return
 */
enum command user_procotol::GetCommand(){
    return _command;
}

/**
 * @brief 获得数据包
 * @return
 */
unsigned char* user_procotol::GetBuf(){
    return _dataBuf;
}

/**
 * @brief 设置命令值
 * @param cmd
 */
void user_procotol::SetCommand(enum command cmd){
    _command = cmd;
}

/**
 * @brief 设置数据包数据
 * @param data
 * @param dataLen
 */
void user_procotol::SetBuf(unsigned char *data, int dataLen){
    memset(_dataBuf,0,PACKAGE_DATA_LEN);
    memcpy(_dataBuf,data,static_cast<size_t>(dataLen));
    _dataLen = static_cast<size_t>(dataLen);
}

/**
 * @brief 设置数据长度（数组）
 * @param dataLen
 */
void user_procotol::SetDataLen(int dataLen){
    _dataLen = dataLen;
}

/**
 * @brief 获取数据长度
 * @return
 */
short int user_procotol::GetDataLen(){
    return _dataLen;
}

/**
 * @brief 设置读状态（是否接收完完整的一帧数据）
 * @param status
 */
void user_procotol::SetReadStatus(bool status){
    _readFnish = status;
}

/**
 * @brief 获得读状态
 * @return
 */
bool user_procotol::getReadStatus(){
    return _readFnish;
}

/**
 * @brief 数据封装
 * @param data
 * @return
 */
int user_procotol::Package(char *data){
    data[0x00] = PACKET_HEADER_ONE;
    data[0x01] = PACKET_HEADER_TWO;
    data[0x02] = GetCommand();
    data[0x03] = GetDataLen();
    memcpy(data+4,GetBuf(),GetDataLen());
    data[0x04+GetDataLen()] = PACKET_END_FLAG;

    return (5+GetDataLen());
}

/**
 * @brief 数据解包
 * @param data
 * @param dataLen
 * @return 解包状态：
 *         0 --- 成功
 *        -1 --- 包头错误
 *        -2 --- 包尾错误
 */
int user_procotol::UnPackage(unsigned char *data, int dataLen){
    //先检验包头
    if((data[0] == PACKET_HEADER_ONE) && (data[1] == PACKET_HEADER_TWO)){
        //再检验包尾
        if(data[dataLen-1] == PACKET_END_FLAG){
            SetCommand((enum command)data[2]);
            //_command = data[2];
            SetDataLen(data[3]<<8|data[4]);
            //_dataLen = data[3];
            //_dataLen = (_dataLen<<8)|data[4];
            memcpy(_dataBuf,&data[4],_dataLen);

            //_readFnish = true;
            SetReadStatus(true);
        }
        else{
            return -2;
        }
    }
    else{
        return -1;
    }

    return 0;
}
