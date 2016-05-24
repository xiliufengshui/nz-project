//============================================================================
// LastChangeTime : Time-stamp: <Administrator 2016/05/10 13:29:32>
// Name           : NZgetPrime.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : naturezhang
// Email          : naturezhang@tencent.com
// CreatDate      : 2016年05月10日 下午 12:55 周二
// Description    : 获取素数 用于共享内存 hash
//============================================================================


#include "NZgetPrime.h"


int get_Prime(long iInputUpper, long aiPrime[] , int iLength)
{
    if(iInputUpper > MAX_CALCULATE_UPPER || iInputUpper < 10 || iLength <= 0 || aiPrime == NULL)
    {
        return -1;
    }
    int iTag = 0;               // 标识
    long iTmpThrod = 0;
    int iPrimeCnt = 0;
    for(long i=iInputUpper; i>=10 ;i--)
    {
        iTmpThrod = (long)sqrt(i);
        iTag = 0;
        for(long j=2; j<=iTmpThrod; j++)
        {
            if(i%j==0)
            {
                iTag = 1;
                break;
            }
        }
        if(iTag == 0)
        {
            aiPrime[iPrimeCnt] = i;
            iPrimeCnt ++;
            if(iPrimeCnt == iLength)
            {
                break;
            }
        }
    }
    return iPrimeCnt;
}



// int main(int argc, char *argv[])
// {
//     long aiPrime[1000];
//     int iRet = get_Prime(100, aiPrime, 30);
//     cout << "prime cnt: " << iRet << endl;
//     for (int i=0; i < 30; i++)
//     {
//         cout << aiPrime[i] << endl;
//     }

//     return 0;
// }
