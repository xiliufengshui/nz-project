//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2020/04/17 14:15:55>
// Name           : main.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2016年12月27日 PM 10:32 Tue
// Description    : 统计utf8文本的字频
//============================================================================


#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>

#include "text_processor.h"

using namespace std;


int main(int argc, char *argv[])
{
    CTextProcessor oCTextProcessor;
    int iRst = oCTextProcessor.init_corpus("../conf/corpus");
    if(iRst == 0)
    {
        cout << "成功：语料库初始化" << endl;
    }
    else
    {
        cout << "失败：语料库初始化" << endl;
    }

    char pcOutput[BUFFER_LEN];
    char acStr[] = "一二三mmnfammsf你裸聊好吗13";
    int iCnt = 0;
    cout << "src string: " << acStr << endl;
    // 统计个数
    iCnt = oCTextProcessor.stat_number_cnt(acStr);
    cout << "字符串中包含数字个数: " << iCnt << endl;

    // 字符替换
    oCTextProcessor.replace_word(pcOutput, acStr);
    cout << "替换后的字符串: " << pcOutput << endl;
    
    // 关键词匹配
    map<string, int> mapRst;
    iCnt = oCTextProcessor.get_all_find_key_word(mapRst, acStr);
    std::map<string, int>::iterator iter;
    for(iter = mapRst.begin(); iter!=mapRst.end(); iter++)
    {
        cout << "keyword:" << iter->first << " 命中次数:" << iter->second << endl;
    }
    return 0;
}
