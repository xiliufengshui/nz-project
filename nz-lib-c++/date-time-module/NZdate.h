//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/05/04 00:48:06>
// Name           : NZdate.h
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2016年05月 4日 AM 12:07 Wed
// Description    : 日期 常用函数
//============================================================================


#ifndef NZDATE_H
#define NZDATE_H

#include <iostream>
#include <time.h>

/* return 0 正常 <0 异常 
   参数：字符串指针 和 对应字符串的长度
   得到字符串 [%Y-%m-%d %H:%M:%S] 
*/
int get_current_date(char *pStrDate, int iSize);


#endif /* NZDATE_H */


