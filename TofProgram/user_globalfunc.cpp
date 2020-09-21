#include "user_globalfunc.h"

user_globalfunc::user_globalfunc()
{

}

/**
 * @brief 设置当前串口号
 * @param portName
 */
void user_globalfunc::SetPortName(QString portName){
    _portName = portName;
}

/**
 * @brief 获得当前串口号
 * @return
 */
QString user_globalfunc::GetPortName(){
    return _portName;
}
