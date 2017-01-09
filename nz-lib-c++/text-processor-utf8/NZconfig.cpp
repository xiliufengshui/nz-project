//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/05/24 01:03:44>
// Name           : NZconfig.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2015年11月18日 PM  4:04 Wed
// Description    : 配置文件
//                  配置文件格式a＝b a b 为字符串 其中a不能为空 扫描时找到的第一个＝为
//                  分割符 逐行扫描 目标给予最大的灵活性
//============================================================================


#include "NZconfig.h"
#define MAX_ROW_NUM 1000		// 配置文件最大行
#define MAX_LINE_LENGTH 3000            // 配置文件每行最大长度

NZconfig::NZconfig()
{
    
}

NZconfig::~NZconfig()
{

}

int NZconfig::read_config_file(char * pcFileName)
{
    ifstream ifFILE;
    ifFILE.open(pcFileName, ios_base::in);
    if (ifFILE.fail()) return -1;
    if (!m_mapConfig.empty()) m_mapConfig.clear();	// 预先清空	  
    string strLine;
    string strKey;
    string strValue;
    int iRowNum = 0;
    while(getline(ifFILE, strLine))
    {
        iRowNum ++;
        strKey.clear();	// 清空
        strValue.clear();
        if (iRowNum > MAX_ROW_NUM) // 防止恶意破坏 
        {
            ifFILE.close();
            return -2;
        }
        // 删除前后空格
        int iPosTmp = strLine.find_first_not_of(" ");
        if (iPosTmp != string::npos) strLine.erase(0, strLine.find_first_not_of(" "));
        iPosTmp = strLine.find_last_not_of(" ");
        if (iPosTmp != string::npos) strLine.erase(strLine.find_last_not_of(" ")+1);
        // 如果是＃  注释 跳过
        iPosTmp = strLine.find_first_of("#");
        if (iPosTmp != string::npos) continue;
        // 删除\r
        iPosTmp = strLine.rfind('\r');
        if (iPosTmp != string::npos) strLine.erase(iPosTmp);
        // 格式 A＝B 至少需要3个字符 并且 少于最大长度
        if (strLine.size()<3 || strLine.size()>MAX_LINE_LENGTH) continue;
        iPosTmp = strLine.find('=', 0);
        // 没找到 =
        if (iPosTmp == string::npos) continue;
        strKey = strLine.substr(0, iPosTmp);
        if (strKey.empty())
        {
            continue;
        }
        strValue = strLine.substr(iPosTmp+1);
        m_mapConfig.insert(pair<string, string>(strKey, strValue));
    }
    ifFILE.close();
    return 0;
}


int NZconfig::print_config()
{
    if (m_mapConfig.empty()) return -1;
    int iRowNum = 0;
    map<string, string>::iterator it;
    for (it = m_mapConfig.begin(); it != m_mapConfig.end(); it++) 
    {
        iRowNum ++;
        cout << iRowNum << ":" << it->first << "=" << it->second << endl;
    }
    return 0;
}


int NZconfig::get_config_value(string strKey, string &strValue)
/* 根据配置名 返回 配置项 s_content */
{
    if (m_mapConfig.empty()) return -1;
    if (strKey.empty()) return -2;
    map<string, string>::iterator it;
    it = m_mapConfig.find(strKey);
    if (it != m_mapConfig.end())
    {
        strValue = it->second;
    }
    else
    {
        return -3;
    }
    return 0;
}


string NZconfig::get_config_value(string strKey)
{
    string strRst = "";
    if(m_mapConfig.empty() || strKey.empty()) return strRst;
    map<string, string>::iterator it;
    it = m_mapConfig.find(strKey);
    if(it != m_mapConfig.end())
    {
        strRst = it->second;
    }
    return strRst;
}
