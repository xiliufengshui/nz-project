//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/05/24 01:04:25>
// Name           : test.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2015年11月19日 PM  8:28 Thu
// Description    : just for test
//============================================================================


#include "NZconfig.h"

int main(int argc, char *argv[])
{
    NZconfig classConfig;
    classConfig.read_config_file("test.ini");
    string strRst;
    classConfig.get_config_value("asd_socket_client_port", strRst);
    std::cout << strRst << std::endl;
    // classConfig.print_config();

    strRst = classConfig.get_config_value("asd_socket_client_port");
    cout << strRst << endl;
    return 0;
}
