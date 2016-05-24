//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/05/24 01:13:52>
// Name           : NZlog.h
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2016年05月 2日 AM  2:30 Mon
// Description    : 打印日志模块
//============================================================================


#ifndef NZLOG_H
#define NZLOG_H

#include <iostream>
#include <ofstream>

using namespace std;

class NZlog
{
public:
    NZlog();
    virtual ~NZlog();

    int write_debug();

    int write_error();

    int write_info();
    
    
};

#endif /* NZLOG_H */
