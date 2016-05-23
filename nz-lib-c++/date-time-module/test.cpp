//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/05/04 00:25:46>
// Name           : test.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2016年05月 3日 PM 11:11 Tue
// Description    : just for test
//============================================================================

#include <iostream>
#include <time.h>
#include "NZdate.h"

using namespace std;


int main(int argc, char *argv[])
{
    char acTmpBuf[100];
    get_current_date(acTmpBuf, 100);
    cout << acTmpBuf << endl;
    return 0;
}
