//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/12/27 22:04:21>
// Name           : test.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2016年12月27日 PM 10:00 Tue
// Description    : 
//============================================================================


#include "NZgetPrime.h"

int main(int argc, char *argv[])
{
    long iInputUpper = 10000;
    long ddwArray[10];
    get_Prime(iInputUpper, ddwArray, 3);
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        cout << ddwArray[i] << endl;
    }
    return 0;
}
