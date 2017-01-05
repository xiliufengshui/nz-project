//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2017/01/04 21:11:48>
// Name           : text_processor.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2017年01月 2日 PM  2:27 Mon
// Description    : 
//============================================================================


#include "text_processor.h"


CTextProcessor::CTextProcessor()
{
    m_mapReplace.clear();
    m_mapCommonChineseCharacter.clear();
    m_mapSubCommonChineseCharacter.clear();
    m_mapNumber.clear();
    m_mapAlphabet.clear();
    m_mapEmoji.clear();
    m_mapSymbol.clear();
    
    m_bIsInitReplaceDate = false;
    m_bIsInitCommonChineseCharacter = false;
    m_bIsInitSubCommonChineseCharacter = false;
    m_bIsInitNumber = false;
    m_bIsInitAlphabet = false;
    m_bIsInitEmoji = false;
    m_bIsInitSymbol = false;
    
    snprintf(m_acReplaceDataFile, BUFFER_LEN, "%s", "test_replace");
    snprintf(m_acCommonChineseCharacterDataFile, BUFFER_LEN, "%s", "common_chinese_character_utf8.txt");
    snprintf(m_acSubCommonChineseCharacterDataFile, BUFFER_LEN, "%s", "sub_common_chinese_character_utf8.txt");
    snprintf(m_acNumberDataFile, BUFFER_LEN, "%s", "number_utf8.txt");
    snprintf(m_acAlphabetDataFile, BUFFER_LEN, "%s", "alphabet_utf8.txt");
    snprintf(m_acEmojiDataFile, BUFFER_LEN, "%s", "emoji_utf8.txt");
    snprintf(m_acSymbolDataFile, BUFFER_LEN, "%s", "symbol_utf8.txt");
    
    setlocale(LC_ALL, "");
}

CTextProcessor::~CTextProcessor()
{
}


int CTextProcessor::init_replace_data(char *pcDataFileName)
{
    if (m_bIsInitReplaceDate == true) return 0;
    if (pcDataFileName == NULL) return -1;
    if (strlen(pcDataFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcDataFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    int iRows = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        if (wcslen(wcaTmp) == 2)
        {
            m_mapReplace[wcaTmp[0]] = wcaTmp[1];
        }
        else
        {
            printf("warning: read line wrong [ %s ]", strLine.c_str());
        }
        iRows ++;
        if (iRows > 100000)
        {
            printf("waring: file too large not read all data!\n");
            break;
        }
    }
    m_bIsInitReplaceDate = true;
    iReadFile.close();
    return 0;
}


int CTextProcessor::replace_word(wchar_t *pwcOutput, wchar_t *pwcInput)
{
    if (pwcInput == NULL || pwcOutput == NULL) return -1;
    int iRst = init_replace_data(m_acReplaceDataFile);
    if (iRst != 0)
    {
        printf("error: init replace data failed!\n");
        return -1;
    }        
    
    int iLen = (int)wcslen(pwcInput);
    int i=0;
    for (i=0; i<iLen; i++)
    {
        if (0 == m_mapReplace[pwcInput[i]])
        {
            pwcOutput[i] = pwcInput[i];
        }
        else
        {
            pwcOutput[i] = m_mapReplace[pwcInput[i]];
        }
    }
    pwcOutput[i] = 0;
    return 0;
}


int CTextProcessor::single_word_stat(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    map<wchar_t, long> mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) 
    {
        return -1;
    }
    string strLine;
    wchar_t wcaTmp[1024] = {0};
    long iTotalCnt = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 1024);
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

int CTextProcessor::before_word_stat(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    map<wchar_t, map<wchar_t, long> > mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail())
    {
        return -1;
    }
    string strLine;
    wchar_t wcaTmp[1024] = {0};
    long iTotalWord = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 1024);
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

int CTextProcessor::next_word_stat(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    map<wchar_t, map<wchar_t, long> > mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail())
    {
        return -1;
    }
    string strLine;
    wchar_t wcaTmp[1024] = {0};
    long iTotalWord = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), 1024);
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

int CTextProcessor::get_all_stat(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;    
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

int CTextProcessor::set_replace_date_file_name(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    memset(m_acReplaceDataFile, 0, BUFFER_LEN);
    strncpy(m_acReplaceDataFile, pcFileName, BUFFER_LEN);
    return 0;
}

int CTextProcessor::stat_msg_length(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    int aiStat[BUFFER_LEN];
    memset(aiStat, 0, sizeof(int)*BUFFER_LEN);
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    int iLen = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        iLen = (int)wcslen(wcaTmp);
        aiStat[iLen] ++;
    }
    iReadFile.close();
    int i = 0;
    for (i=0; i<BUFFER_LEN; i++)
    {
        if (aiStat[i] == 0) continue;
        printf("%d,%d\n", i, aiStat[i]);
    }
    printf("finish\n");
    return 0;
}

int CTextProcessor::tag_msg_length(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    int iLen = 0;
    char acOut[BUFFER_LEN];
    ofstream iWriterFile("rst_tag_length.txt", ios_base::out);
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        iLen = (int)wcslen(wcaTmp);
        snprintf(acOut, BUFFER_LEN, "%d,%s\n", iLen, strLine.c_str());
        iWriterFile << acOut;
    }
    iReadFile.close();
    iWriterFile.close();
    return 0;
}

int CTextProcessor::get_sample_feature(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    if (init_common_chinese_character(m_acCommonChineseCharacterDataFile) != 0) return -1;
    if (init_sub_common_chinese_character(m_acSubCommonChineseCharacterDataFile) != 0) return -1;
    if (init_number(m_acNumberDataFile) != 0) return -1;
    if (init_alphabet(m_acAlphabetDataFile) != 0) return -1;
    if (init_emoji(m_acEmojiDataFile) != 0) return -1;
    if (init_symbol(m_acSymbolDataFile) != 0) return -1;
    
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    
    int iMsgLen = 0;
    int iMsgUniqCnt = 0;
    int iCommonCnt = 0;
    int iCommonUniqCnt = 0;
    int iSubCommonCnt = 0;
    int iSubCommonUniqCnt = 0;
    int iNumberCnt = 0;
    int iNumberUniqCnt = 0;
    int iAlphabetCnt = 0;
    int iAlphabetUniqCnt = 0;
    int iEmojiCnt = 0;
    int iEmojiUniqCnt = 0;
    int iSymbolCnt = 0;
    int iSymbolUniqCnt = 0;

    map<wchar_t, int> mapMsg;
    map<wchar_t, int> mapCommon;
    map<wchar_t, int> mapSubCommon;
    map<wchar_t, int> mapNumber;
    map<wchar_t, int> mapAlphabet;
    map<wchar_t, int> mapEmoji;
    map<wchar_t, int> mapSymbol;


    
    ofstream iWriterFile("rst_feature.txt", ios_base::out);
    char acOut[BUFFER_LEN];
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);

        iMsgLen = (int)wcslen(wcaTmp);
        iMsgUniqCnt = 0;
        iCommonCnt = 0;
        iCommonUniqCnt = 0;
        iSubCommonCnt = 0;
        iSubCommonUniqCnt = 0;
        iNumberCnt = 0;
        iNumberUniqCnt = 0;
        iAlphabetCnt = 0;
        iAlphabetUniqCnt = 0;
        iEmojiCnt = 0;
        iEmojiUniqCnt = 0;
        iSymbolCnt = 0;
        iSymbolUniqCnt = 0;
        
        mapMsg.clear();
        mapCommon.clear();
        mapSubCommon.clear();
        mapNumber.clear();
        mapAlphabet.clear();
        mapEmoji.clear();
        mapSymbol.clear();
        
        for (int i=0; i<iMsgLen; i++)
        {
            mapMsg[wcaTmp[i]] = 1;
            if (m_mapCommonChineseCharacter[wcaTmp[i]] == 1)
            {
                iCommonCnt ++;
                mapCommon[wcaTmp[i]] = 1;
            }
            if (m_mapSubCommonChineseCharacter[wcaTmp[i]] == 1)
            {
                iSubCommonCnt ++;
                mapSubCommon[wcaTmp[i]] = 1;
            }
            if (m_mapNumber[wcaTmp[i]] == 1)
            {
                iNumberCnt ++;
                mapNumber[wcaTmp[i]] = 1;
            }
            if (m_mapAlphabet[wcaTmp[i]] == 1)
            {
                iAlphabetCnt ++;
                mapAlphabet[wcaTmp[i]] = 1;
            }
            if (m_mapEmoji[wcaTmp[i]] == 1)
            {
                iEmojiCnt ++;
                mapEmoji[wcaTmp[i]] = 1;
            }
            if (m_mapSymbol[wcaTmp[i]] == 1)
            {
                iSymbolCnt ++;
                mapSymbol[wcaTmp[i]] = 1;
            }
            
        }
        iMsgUniqCnt = mapMsg.size();
        iCommonUniqCnt = mapCommon.size();
        iSubCommonUniqCnt = mapSubCommon.size();
        iNumberUniqCnt = mapNumber.size();
        iAlphabetUniqCnt = mapAlphabet.size();
        iEmojiUniqCnt = mapEmoji.size();
        iSymbolUniqCnt = mapSymbol.size();

        float fTmp1 = (float)iMsgUniqCnt/iMsgLen;
        
        float fTmp2 = (float)iCommonCnt/iMsgLen;
        
        float fTmp3 = 0;
        if (iCommonCnt != 0) fTmp3 = (float)iCommonUniqCnt/iCommonCnt;
        
        float fTmp4 = (float)iSubCommonCnt/iMsgLen;
        
        float fTmp5 = 0;
        if (iSubCommonCnt != 0) fTmp5 = (float)iSubCommonUniqCnt/iSubCommonCnt;
        
        float fTmp6 = (float)iNumberCnt/iMsgLen;
        
        float fTmp7 = 0;
        if (iNumberCnt != 0) fTmp7 = (float)iNumberUniqCnt/iNumberCnt;
        
        float fTmp8 = (float)iAlphabetCnt/iMsgLen;
        
        float fTmp9 = 0;
        if (iAlphabetCnt != 0) fTmp9 = (float)iAlphabetUniqCnt/iAlphabetCnt;
        
        float fTmp10 = (float)iEmojiCnt/iMsgLen;
        
        float fTmp11 = 0;
        if (iEmojiCnt != 0) fTmp11 = (float)iEmojiUniqCnt/iEmojiCnt;

        float fTmp12 = (float)iCommonUniqCnt/iMsgUniqCnt;
        float fTmp13 = (float)iSubCommonUniqCnt/iMsgUniqCnt;
        float fTmp14 = (float)iNumberUniqCnt/iMsgUniqCnt;
        float fTmp15 = (float)iAlphabetUniqCnt/iMsgUniqCnt;
        float fTmp16 = (float)iEmojiUniqCnt/iMsgUniqCnt;
        

        snprintf(acOut, BUFFER_LEN, "1:%d 2:%d 3:%d 4:%d 5:%d 6:%d 7:%d 8:%d 9:%d 10:%d 11:%d 12:%d 13:%d 14:%d\n"
                 ,iMsgLen       
                 ,iMsgUniqCnt
                 ,iCommonCnt    
                 ,iCommonUniqCnt
                 ,iSubCommonCnt
                 ,iSubCommonUniqCnt
                 ,iNumberCnt    
                 ,iNumberUniqCnt
                 ,iAlphabetCnt  
                 ,iAlphabetUniqCnt
                 ,iEmojiCnt     
                 ,iEmojiUniqCnt
                 ,iSymbolCnt
                 ,iSymbolUniqCnt

                 // ,fTmp1
                 // ,fTmp2
                 // ,fTmp3
            );
        // snprintf(acOut, BUFFER_LEN, "1:%d 2:%f 3:%f 4:%f 5:%f 6:%f 7:%f 8:%f 9:%f 10:%f 11:%f 12:%f 13:%f 14:%f 15:%f 16:%f 17:%f\n"
        //          ,iMsgLen
        //          ,fTmp1
        //          ,fTmp2
        //          ,fTmp3
        //          ,fTmp4
        //          ,fTmp5
        //          ,fTmp6
        //          ,fTmp7
        //          ,fTmp8
        //          ,fTmp9
        //          ,fTmp10
        //          ,fTmp11
        //          ,fTmp12
        //          ,fTmp13
        //          ,fTmp14
        //          ,fTmp15
        //          ,fTmp16
        //     );
        iWriterFile << acOut;
    }
    iReadFile.close();
    iWriterFile.close();
    return 0;
}

int CTextProcessor::init_common_chinese_character(char *pcFileName)
{
    if (m_bIsInitCommonChineseCharacter == true) return 0;
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_mapCommonChineseCharacter[wcaTmp[i]] = 1;
        }
    }
    m_bIsInitCommonChineseCharacter = true;
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_sub_common_chinese_character(char *pcFileName)
{
    if (m_bIsInitSubCommonChineseCharacter == true) return 0;
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_mapSubCommonChineseCharacter[wcaTmp[i]] = 1;
        }
    }
    m_bIsInitSubCommonChineseCharacter = true;
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_number(char *pcFileName)
{
    if (m_bIsInitNumber == true) return 0;
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_mapNumber[wcaTmp[i]] = 1;
        }
    }
    m_bIsInitNumber = true;
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_alphabet(char *pcFileName)
{
    if (m_bIsInitAlphabet == true) return 0;
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_mapAlphabet[wcaTmp[i]] = 1;
        }
    }
    m_bIsInitAlphabet = true;
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_emoji(char *pcFileName)
{
    if (m_bIsInitEmoji == true) return 0;
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_mapEmoji[wcaTmp[i]] = 1;
        }
    }
    m_bIsInitEmoji = true;
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_symbol(char *pcFileName)
{
    if (m_bIsInitSymbol == true) return 0;
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_mapSymbol[wcaTmp[i]] = 1;
        }
    }
    m_bIsInitSymbol = true;
    iReadFile.close();
    return 0;
}
