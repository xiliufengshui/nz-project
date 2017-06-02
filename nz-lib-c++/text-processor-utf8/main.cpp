//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2017/06/02 12:34:51>
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

// int replace_word(wchar_t *pwcOutput, wchar_t *pwcInput)
// {
//     char *pcDataFile = "test_replace";
//     int iRst = init_replace_data(pcDataFile);
//     if (iRst != 0) printf("error: init replace wrong\n");
    
//     int iLen = (int)wcslen(pwcInput);
//     for (int i=0; i<iLen; i++)
//     {
//         if (0 == g_mapReplace[pwcInput[i]])
//         {
//             pwcOutput[i] = pwcInput[i];
//         }
//         else
//         {
//             pwcOutput[i] = g_mapReplace[pwcInput[i]];
//         }
//     }
//     return 0;
// }

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

// int main(int argc, char *argv[])
// {
//     // filter_msg(argv[1]);
//     CTextProcessor oCTextProcessor;
//     oCTextProcessor.init_from_configuration_file(argv[1]);
//     // oCTextProcessor.reload_init_data();
//     oCTextProcessor.get_sample_feature(argv[2]);
//     // double dRst = oCTextProcessor.get_svm_predict("100元-3万金币要的加QQ14757928先货");
//     // printf("predict rst: %g\n", dRst);
//     // // oCTextProcessor.stat_msg_length(argv[1]);
//     // oCTextProcessor.tag_msg_length(argv[1]);
//     // oCTextProcessor.get_sample_feature(argv[1]);
    
//     // if (argc != 3)
//     // {
//     //     cout << "example: " << argv[0] << " filename option[single|before|next|all|test]" << endl;
//     //     return 0;
//     // }
    
//     // if (strcmp(argv[2], "single") == 0)
//     // {
//     //     printf("I am single\n");
//     //     oCTextProcessor.single_word_stat(argv[1]);
//     // }
//     // if (strcmp(argv[2], "before") == 0)
//     // {
//     //     printf("I am before\n");
//     //     oCTextProcessor.before_word_stat(argv[1]);
//     // }
//     // if (strcmp(argv[2], "next") == 0)
//     // {
//     //     printf("I am next\n");
//     //     oCTextProcessor.next_word_stat(argv[1]);
//     // }
//     // if (strcmp(argv[2], "all") == 0)
//     // {
//     //     printf("I am all\n");
//     //     oCTextProcessor.get_all_stat(argv[1]);
//     // }
//     // if (strcmp(argv[2], "test") == 0)
//     // {
//     //     printf("I am test\n");
//     //     test(argv[1]);
//     // }
//     cout << "finish" << endl;
//     return 0;
// }

// int main(int argc, char *argv[])
// {
//     char acInput[] = "ooxxCC%dAAAoen....END";
//     CTextProcessor oCTextProcessor;
//     // oCTextProcessor.init_from_configuration_file(argv[1]);
//     // oCTextProcessor.get_sample_feature(argv[2]);
//     // int iRst = oCTextProcessor.get_svm_predict(acInput);
//     map<string, int> mapRst;
//     map<string, int>::iterator iter; 
//     oCTextProcessor.init_key_word(argv[1]);
//     oCTextProcessor.init_ac_trie();
//     // int iRst = oCTextProcessor.find_key_word(acInput);
//     int iRst = oCTextProcessor.get_all_find_key_word(mapRst, acInput);
//     cout << "src string: " << acInput << endl;
//     cout << "iRst: "<< iRst << endl;
//     cout << "mapRst.size(): " << mapRst.size() << endl;

//     for (iter = mapRst.begin(); iter != mapRst.end(); iter ++)
//     {
//         cout << iter->first << " " << iter->second << endl;
//     }

    
//     // oCTextProcessor.init_replace_data(argv[1]);
//     // oCTextProcessor.init_common_chinese_character_data(argv[1]);

//     // char acInput[] = "四五六,フⅦ我是好人8907sdwe";
//     // char acOut[300];
//     // wchar_t wcaInput[] = L"四五六,フⅦ我是好人";
//     // wchar_t wcaOutput[300];
//     // int iCommonCnt = oCTextProcessor.stat_common_chinese_character_cnt(acInput);
//     // int iSubCommonCnt = oCTextProcessor.stat_sub_common_chinese_character_cnt(acInput);
//     // int iNumberCnt = oCTextProcessor.stat_number_cnt(acInput);
//     // int iAlphabetCnt = oCTextProcessor.stat_alphabet_cnt(acInput);
//     // int iEmojiCnt = oCTextProcessor.stat_emoji_cnt(acInput);
//     // int iSymbolCnt = oCTextProcessor.stat_symbol_cnt(acInput);
//     // int iMsgLen = oCTextProcessor.get_msg_word_cnt(acInput);
//     // cout << acInput << endl;
//     // cout << iCommonCnt << endl;
//     // cout << iSubCommonCnt << endl;
//     // cout << iNumberCnt << endl;
//     // cout << iAlphabetCnt << endl;
//     // cout << iEmojiCnt << endl;
//     // cout << iSymbolCnt << endl;
//     // cout << iMsgLen << endl;
    
//     // oCTextProcessor.replace_word(wcaOutput, wcaInput);
//     // printf("%ls\n", wcaInput);
//     // printf("%ls\n", wcaOutput);
//     // oCTextProcessor.replace_word(acOut, acInput);
//     // cout << acInput << endl;
//     // cout << acOut << endl;
//     // oCTextProcessor.init_key_word(argv[1]);
//     // map<int, string>::iterator iter;
//     // for (iter=oCTextProcessor.m_mapKeyWord.begin(); iter!=oCTextProcessor.m_mapKeyWord.end(); iter++)
//     // {
//     //     cout << iter->first << "    " << iter->second << endl;
//     // }
//     // oCTextProcessor.init_ac_trie();
//     // struct TireNode stTireNode;
//     // cout << stTireNode.wcWord << endl;
//     // cout << stTireNode.iFailPoint << endl;
//     // cout << stTireNode.iKeyWordIndex << endl;
//     // cout << stTireNode.mapNext.size() << endl;
    
//     cout << "finish!" << endl;

    
//     return 0;
// }

// int main(int argc, char *argv[])
// {
//     cout << "file name: " << argv[1] << endl;
//     ifstream ifFILE;
//     ifFILE.open(argv[1], ios_base::in);
//     if (ifFILE.fail()) return -1;
//     CTextProcessor oCTextProcessor;
//     oCTextProcessor.init_from_configuration_file(argv[2]);
//     char acReplaced[1024];
//     char acFilterIgnore[1024];
//     int iRst = 0;
//     string strLine;
//     while (getline(ifFILE, strLine))
//     {
//         cout << "*********************************" << endl;
//         cout << strLine << endl;
//         oCTextProcessor.replace_word(acReplaced,const_cast<char*>(strLine.c_str()));
//         iRst = oCTextProcessor.filter_ignore_word(acFilterIgnore, acReplaced);
//         cout << acReplaced << endl;
//         cout << acFilterIgnore << endl;
//         iRst = oCTextProcessor.find_key_word(acReplaced);
//         if(iRst == 1)
//         {
//             cout << "++++++++: "; 
//             cout << strLine << endl;
//         }
//     }
//     ifFILE.close();
//     return 0;
//

int main(int argc, char *argv[])
{
    if(argc != 3) return -2;
    int iRst = 0;
    CTextProcessor oCTextProcessor;
    iRst = oCTextProcessor.init_from_configuration_file(argv[1]);
    if(iRst < 0)
    {
        cout << "error: init config file failed! error code: " << iRst << endl;
        return -1;
    }
    cout << "paramter 2 is: " << argv[2] << endl;
    char acOut[1024];
    oCTextProcessor.filter_not_focus_word(acOut, argv[2]);
    cout << "output: " << acOut << endl;
    cout << "finish" << endl;
    return 0;
}
