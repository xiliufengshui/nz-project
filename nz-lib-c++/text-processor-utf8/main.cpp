//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2017/01/09 14:37:35>
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

map<wchar_t, wchar_t> g_mapReplace;
int init_replace_data(char *pcDataFile)
{
    setlocale(LC_ALL, "");
    ifstream iReadFile;
    iReadFile.open(pcDataFile, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[100];
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 100);
        if (wcslen(wcaTmp) == 2)
        {
            g_mapReplace[wcaTmp[0]] = wcaTmp[1];
        }
        else
        {
            printf("warning: read line wrong [ %s ]", strLine.c_str());
        }
    }
    return 0;
}

int replace_word(wchar_t *pwcOutput, wchar_t *pwcInput)
{
    char *pcDataFile = "test_replace";
    int iRst = init_replace_data(pcDataFile);
    if (iRst != 0) printf("error: init replace wrong\n");
    
    int iLen = (int)wcslen(pwcInput);
    for (int i=0; i<iLen; i++)
    {
        if (0 == g_mapReplace[pwcInput[i]])
        {
            pwcOutput[i] = pwcInput[i];
        }
        else
        {
            pwcOutput[i] = g_mapReplace[pwcInput[i]];
        }
    }
    return 0;
}

int single_word_stat_other(char *pcFileName)
{
    setlocale(LC_ALL, "");
    map<wchar_t, long> mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) 
    {
        return -1;
    }
    string strLine;
    wchar_t wcaTmp[1000] = {0};
    long iTotalCnt = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 1000);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            iTotalCnt ++;
            mapStat[wcaTmp[i]] ++;
        }
    }
    iReadFile.close();
    char caTmp[10];
    char caOut[1000];
    printf("total word cnt: %ld\n", iTotalCnt);
    ofstream iWriterFile("rst_single.txt", ios_base::out);
    map<wchar_t, long>::iterator iter;
    for (iter = mapStat.begin(); iter != mapStat.end(); iter++)
    {
        
        wcstombs(caTmp, &(iter->first), 9);
        snprintf(caOut, 1000, "%s\t%ld\t%f\n", caTmp, iter->second, ((float)(iter->second)/iTotalCnt));
        iWriterFile << caOut;
    }
    iWriterFile.close();
    return 0;
}

int stat_before(char *pcFileName)
{
    setlocale(LC_ALL, "");
    map<wchar_t, map<wchar_t, long> > mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail())
    {
        return -1;
    }
    string strLine;
    wchar_t wcaTmp[1000] = {0};
    long iTotalWord = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 1000);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=1; i<iLen; i++)
        {
            iTotalWord ++;
            mapStat[wcaTmp[i]][wcaTmp[i-1]] ++;
        }
    }
    iReadFile.close();

    char caTmp[10];
    char caTmpSub[10];
    char caOut[1000];
    printf("total word cnt: %ld\n", iTotalWord);
    ofstream iWriterFile("rst_before.txt", ios_base::out);
    map<wchar_t, map<wchar_t, long> >::iterator iterStat;
    map<wchar_t, long>::iterator iterNext;
    for (iterStat = mapStat.begin(); iterStat != mapStat.end(); iterStat++)
    {
        for (iterNext = iterStat->second.begin(); iterNext != iterStat->second.end(); iterNext++)
        {
            wcstombs(caTmp, &(iterStat->first), 9);
            wcstombs(caTmpSub, &(iterNext->first), 9);
            snprintf(caOut, 1000, "%s\t%s\t%ld\n", caTmp, caTmpSub, iterNext->second);
            iWriterFile << caOut;
        }
    }
    iWriterFile.close();
    return 0;
}


int stat_next(char *pcFileName)
{
    setlocale(LC_ALL, "");
    map<wchar_t, map<wchar_t, long> > mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail())
    {
        return -1;
    }
    string strLine;
    wchar_t wcaTmp[1000] = {0};
    long iTotalWord = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 1000);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<(iLen-1); i++)
        {
            iTotalWord ++;
            mapStat[wcaTmp[i]][wcaTmp[i+1]] ++;
        }
    }
    iReadFile.close();

    char caTmp[10];
    char caTmpSub[10];
    char caOut[1000];
    printf("total word cnt: %ld\n", iTotalWord);
    ofstream iWriterFile("rst_next.txt", ios_base::out);
    map<wchar_t, map<wchar_t, long> >::iterator iterStat;
    map<wchar_t, long>::iterator iterNext;
    for (iterStat = mapStat.begin(); iterStat != mapStat.end(); iterStat++)
    {
        for (iterNext = iterStat->second.begin(); iterNext != iterStat->second.end(); iterNext++)
        {
            wcstombs(caTmp, &(iterStat->first), 9);
            wcstombs(caTmpSub, &(iterNext->first), 9);
            snprintf(caOut, 1000, "%s\t%s\t%ld\n", caTmp, caTmpSub, iterNext->second);
            iWriterFile << caOut;
        }
    }
    iWriterFile.close();
    return 0;
}

int get_all(char *pcFileName)
{
    setlocale(LC_ALL, "");
    map<wchar_t, long> mapStatSingle;
    map<wchar_t, map<wchar_t, long> > mapStatBefore;
    map<wchar_t, map<wchar_t, long> > mapStatNext;
    mapStatSingle.clear();
    mapStatBefore.clear();
    mapStatNext.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail())
    {
        return -1;
    }
    string strLine;
    wchar_t wcaTmp[1000] = {0};
    long iTotalWord = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 1000);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            iTotalWord ++;
            mapStatSingle[wcaTmp[i]] ++;
        }
        for (int i=1; i<iLen; i++)
        {
            mapStatBefore[wcaTmp[i]][wcaTmp[i-1]] ++;
        }
        for (int i=0; i<(iLen-1); i++)
        {
            mapStatNext[wcaTmp[i]][wcaTmp[i+1]] ++;
        }
    }
    iReadFile.close();

    char caTmp[10];
    char caTmpSub[10];
    char caOut[1000];
    printf("total word cnt: %ld\n", iTotalWord);
    ofstream iWriterFileSingle("rst_single.txt", ios_base::out);
    ofstream iWriterFileBefore("rst_before.txt", ios_base::out);
    ofstream iWriterFileNext("rst_next.txt", ios_base::out);

    map<wchar_t, long>::iterator iterSingle;
    for (iterSingle=mapStatSingle.begin(); iterSingle!=mapStatSingle.end(); iterSingle++)
    {
        wcstombs(caTmp, &(iterSingle->first), 9);
        snprintf(caOut, 1000, "%s\t%ld\n", caTmp, iterSingle->second);
        iWriterFileSingle << caOut;
    }

    map<wchar_t, map<wchar_t, long> >::iterator iterStatBefore;
    map<wchar_t,long>::iterator iterBefore;
    for (iterStatBefore=mapStatBefore.begin(); iterStatBefore!=mapStatBefore.end(); iterStatBefore++)
    {
        for (iterBefore=iterStatBefore->second.begin(); iterBefore!=iterStatBefore->second.end(); iterBefore++)
        {
            wcstombs(caTmp, &(iterStatBefore->first), 9);
            wcstombs(caTmpSub, &(iterBefore->first), 9);
            snprintf(caOut, 1000, "%s\t%s\t%ld\n", caTmp, caTmpSub, iterBefore->second);
            iWriterFileBefore << caOut;
        }
    }
    
    map<wchar_t, map<wchar_t, long> >::iterator iterStatNext;
    map<wchar_t, long>::iterator iterNext;
    for (iterStatNext = mapStatNext.begin(); iterStatNext != mapStatNext.end(); iterStatNext++)
    {
        for (iterNext = iterStatNext->second.begin(); iterNext != iterStatNext->second.end(); iterNext++)
        {
            wcstombs(caTmp, &(iterStatNext->first), 9);
            wcstombs(caTmpSub, &(iterNext->first), 9);
            snprintf(caOut, 1000, "%s\t%s\t%ld\n", caTmp, caTmpSub, iterNext->second);
            iWriterFileNext << caOut;
        }
    }

    iWriterFileSingle.close();
    iWriterFileBefore.close();
    iWriterFileNext.close();
    return 0;
}

int test(char *pcFileName)
{
    setlocale(LC_ALL, "");
    map<wchar_t, long> mapStatSingle;
    map<wchar_t, map<wchar_t, long> > mapStatBefore;
    map<wchar_t, map<wchar_t, long> > mapStatNext;
    map<wchar_t, long> mapStatBeforeAll;
    map<wchar_t, long> mapStatNextAll;
    mapStatSingle.clear();
    mapStatBefore.clear();
    mapStatNext.clear();
    mapStatBeforeAll.clear();
    mapStatNextAll.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[1000] = {0};
    long iTotalWord = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 1000);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            iTotalWord ++;
            mapStatSingle[wcaTmp[i]] ++;
        }
        for (int i=1; i<iLen; i++)
        {
            mapStatBefore[wcaTmp[i]][wcaTmp[i-1]] ++;
        }
        for (int i=0; i<(iLen-1); i++)
        {
            mapStatNext[wcaTmp[i]][wcaTmp[i+1]] ++;
        }
    }
    iReadFile.close();

    map<wchar_t, map<wchar_t, long> >::iterator iterBefore;
    map<wchar_t, map<wchar_t, long> >::iterator iterNext;
    map<wchar_t, long>::iterator iterTmp;
    for (iterBefore=mapStatBefore.begin(); iterBefore!=mapStatBefore.end(); iterBefore++)
    {
        int iCnt = 0;
        for (iterTmp=iterBefore->second.begin(); iterTmp!=iterBefore->second.end(); iterTmp++)
        {
            iCnt += iterTmp->second;
        }
        mapStatBeforeAll[iterBefore->first] = iCnt;
    }

    for (iterNext=mapStatNext.begin(); iterNext!=mapStatNext.end(); iterNext++)
    {
        int iCnt = 0;
        for (iterTmp=iterNext->second.begin(); iterTmp!=iterNext->second.end(); iterTmp++)
        {
            iCnt += iterTmp->second;
        }
        mapStatNextAll[iterNext->first] = iCnt;
    }
    
    char caOut[1024];
    ofstream iWriterFile("rst_tmp.txt", ios_base::out);
    ofstream iWriterFile2("rst_tmp2.txt", ios_base::out);
    map<wchar_t, long>::iterator iter2;

    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 1000);
        // int iLowCnt = 0;
        int iLen = (int)wcslen(wcaTmp);
        int i = 0;
        // for (i=0; i<iLen; i++)
        // {
        //     if (mapStatSingle[wcaTmp[i]] < 1000)
        //     {
        //         iLowCnt ++;
        //     }
        // }
        // snprintf(caOut, 1024, "%d,%f,%s\n", iLowCnt, ((float)iLowCnt/iLen), strLine.c_str());
        // iWriterFile << caOut;

        // 跳变
        int iJumpCnt = 0;
        for (i=0; i<(iLen-1); i++)
        {
            if ((mapStatNext[wcaTmp[i]][wcaTmp[i+1]] / (float)mapStatNextAll[wcaTmp[i]]) < 0.05)
            {
                iJumpCnt ++;
            }
        }
        snprintf(caOut, 1024, "%d,%d,%s\n", iJumpCnt, iLen, strLine.c_str());
        iWriterFile2 << caOut;
    }
    iReadFile.close();
    iWriterFile.close();
    iWriterFile2.close();

    // ifstream iReadFileYuCe;
    // iReadFileYuCe.open("yu_ce.txt", ios_base::in);
    // if (iReadFileYuCe.fail())
    // {
    //     return -1;
    // }
    // while (getline(iReadFileYuCe, strLine))
    // {
    //     mbstowcs(wcaTmp, strLine.c_str(), 1000);
    //     int iLen = (int)wcslen(wcaTmp);
    //     for (int i=1; i<(iLen-1); i++)
    //     {
            
    //     }
    // }

    
    
    // sleep(10000);
    return 0;
}

int filter_msg(char *pcFileName)
{
    setlocale(LC_ALL, "");
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= 1024) return -1;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[1024];
    int iLen = 0;
    int iElement = 0;
    char acOut[1024];
    ofstream iWriterFile("rst_filter_msg.txt", ios_base::out);
    map<wchar_t, int> mapTmp;
    while (getline(iReadFile, strLine))
    {
        // printf("%s\n", strLine.c_str());
        // memset(wcaTmp, 0, sizeof(wchar_t)*1024);
        mbstowcs(wcaTmp, strLine.c_str(), 1024);
        iLen = (int)wcslen(wcaTmp);
        // printf("length : %d\n", iLen);
        if (iLen < 5) 
        {
            continue;
        }
        mapTmp.clear();
        for (int i=0; i<iLen; i++)
        {
            mapTmp[wcaTmp[i]] = 1;
        }
        iElement = mapTmp.size();
        // printf("size : %d\n", iElement);
        if (iElement/(float)iLen < 0.5)
        {
            continue;
        }
        snprintf(acOut, 1024, "%s\n", strLine.c_str());
        iWriterFile << acOut;
    }
    iReadFile.close();
    iWriterFile.close();

    return 0;
}

int main(int argc, char *argv[])
{
    // filter_msg(argv[1]);
    CTextProcessor oCTextProcessor;
    // oCTextProcessor.reload_init_data();
    // oCTextProcessor.get_sample_feature(argv[1]);
    double dRst = oCTextProcessor.get_svm_predict("100元-3万金币要的加QQ14757928先货");
    printf("predict rst: %g\n", dRst);
    // // oCTextProcessor.stat_msg_length(argv[1]);
    // oCTextProcessor.tag_msg_length(argv[1]);
    // oCTextProcessor.get_sample_feature(argv[1]);
    
    // if (argc != 3)
    // {
    //     cout << "example: " << argv[0] << " filename option[single|before|next|all|test]" << endl;
    //     return 0;
    // }
    
    // if (strcmp(argv[2], "single") == 0)
    // {
    //     printf("I am single\n");
    //     oCTextProcessor.single_word_stat(argv[1]);
    // }
    // if (strcmp(argv[2], "before") == 0)
    // {
    //     printf("I am before\n");
    //     oCTextProcessor.before_word_stat(argv[1]);
    // }
    // if (strcmp(argv[2], "next") == 0)
    // {
    //     printf("I am next\n");
    //     oCTextProcessor.next_word_stat(argv[1]);
    // }
    // if (strcmp(argv[2], "all") == 0)
    // {
    //     printf("I am all\n");
    //     oCTextProcessor.get_all_stat(argv[1]);
    // }
    // if (strcmp(argv[2], "test") == 0)
    // {
    //     printf("I am test\n");
    //     test(argv[1]);
    // }
    cout << "finish" << endl;
    return 0;
}

