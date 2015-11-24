//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2015/11/19 20:33:01>
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
    classConfig.print_config();
    return 0;
}
