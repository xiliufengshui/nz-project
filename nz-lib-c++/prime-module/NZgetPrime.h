//============================================================================
// LastChangeTime : Time-stamp: <Administrator 2016/05/10 13:17:34>
// Name           : NZgetPrime.h
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : naturezhang
// Email          : naturezhang@tencent.com
// CreatDate      : 2016年05月10日 下午 12:55 周二
// Description    : 获取素数 用于共享内存 hash
//============================================================================



#ifndef NZGETPRIME_H
#define NZGETPRIME_H

#define MAX_CALCULATE_UPPER 100000000000 /* 1000亿 */

#include <iostream>
#include <math.h>

using namespace std;


/* 获取比 iInputUpper 小的 ILength个素数 从大到小
   参数：
   iInputUpper 给出一个正整数 不超过1000亿 >10 且 < 1000亿
   aiPrime  获取素数结果放置在数组中
   iLength  获取素数的个数  aiPrime 数据的长度 
   return >0 正常  <0 异常 return >0 时为实际获取到素数的个数 <= iLength
*/
int get_Prime(long iInputUpper, long aiPrime[] , int iLength);


#endif /* NZGETPRIME_H */
