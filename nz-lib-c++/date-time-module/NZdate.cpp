//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/05/04 00:25:05>
// Name           : NZdate.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2016年05月 4日 AM 12:14 Wed
// Description    : 日期 常用函数
//============================================================================


#include "NZdate.h"

int get_current_date(char *pStrDate, int iSize)
{
    if(pStrDate == NULL || iSize < 0)
    {
        return -1;
    }
    time_t dwTimer = time(NULL);
    struct tm * stTmTime;
    stTmTime = localtime(&dwTimer);
    strftime(pStrDate, iSize, "[%Y-%m-%d %H:%M:%S] ", stTmTime);
    return 0;
}
