//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/05/24 00:52:59>
// Name           : NZconfig.h
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2015年11月18日 PM  2:26 Wed
// Description    : 配置文件
//                  配置文件格式a＝b a b 为字符串 其中a不能为空 扫描时找到的第一个＝为
//                  分割符 逐行扫描 目标给予最大的灵活性 注意：b可以是空格
//============================================================================


#ifndef NZCONFIG_H
#define NZCONFIG_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace std;


class NZconfig
{
public:
    NZconfig();		/* 构造函数  */
    virtual ~NZconfig();
    /* function: read_config_file
       返回值 ：
       0   正常
       -1  打开配置文件失败
       -2  配置文件行数太多
       -3  
     */
    int read_config_file(char * pcFileName); /* 读取配置文件 */
    /* function: config 
       返回值 ：
       0    正常
       -1   没有配置内容
     */
    int print_config();
    /* function: get_config_value 
       返回值 ：
       0    正常
       -1   没有配置内容
       -2   传入查询key为空
       -3   没有找到对应key值
     */
    int get_config_value(string strKey, string &strValue); /* 根据配置名 返回 配置项 */

    string get_config_value(string strKey);

    
    map<string, string> m_mapConfig;
};


#endif /* NZCONFIG_H */
