//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2018/11/06 19:08:12>
// Name           : text_processor.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2017年01月 2日 PM  2:27 Mon
// Description    : 
//============================================================================


#include "text_processor.h"
#include "NZconfig.h"


CTextProcessor::CTextProcessor()
{
    m_pSvmModel = NULL;
    setlocale(LC_ALL, "en_US.UTF-8");
}

CTextProcessor::~CTextProcessor()
{
    if (m_pSvmModel != NULL)
    {
        svm_free_and_destroy_model(&m_pSvmModel);
    }
}

int CTextProcessor::init_replace_data(char *pcDataFileName)
{
    if (pcDataFileName == NULL) return -1;
    if (strlen(pcDataFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcDataFileName, ios_base::in);
    if (iReadFile.fail()) return -2;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    int iRows = 0;
    m_mapReplace.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        if (wcslen(wcaTmp) == 2)
        {
            m_mapReplace[wcaTmp[0]] = wcaTmp[1];
        }
        else
        {
            printf("warning: read line wrong [ %s ]\n", strLine.c_str());
        }
        iRows ++;
        if (iRows > 1000000)
        {
            printf("waring: file too large not read all data!\n");
            break;
        }
    }
    iReadFile.close();
    return 0;
}


int CTextProcessor::replace_word(wchar_t *pwcOutput, wchar_t *pwcInput)
{
    if (pwcInput == NULL || pwcOutput == NULL) return -1;
    if (m_mapReplace.empty()) return -2;
    int iLen = (int)wcslen(pwcInput);
    int i=0;
    for (i=0; i<iLen; i++)
    {
        if (m_mapReplace.find(pwcInput[i]) == m_mapReplace.end())
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

int CTextProcessor::replace_word(char *pcOutput, char *pcInput)
{
    if (pcOutput == NULL || pcInput == NULL) return -1;
    if (m_mapReplace.empty()) return -2;
    if (strlen(pcInput) >= BUFFER_LEN) return -3;
    wchar_t wcaInput[BUFFER_LEN];
    wchar_t wcaOutput[BUFFER_LEN];
    mbstowcs(wcaInput, pcInput, BUFFER_LEN);
    int iLen = (int)wcslen(wcaInput);
    int i = 0;
    for (i=0; i<iLen; i++)
    {
        if (m_mapReplace.find(wcaInput[i]) == m_mapReplace.end())
        {
            wcaOutput[i] = wcaInput[i];
        }
        else
        {
            wcaOutput[i] = m_mapReplace[wcaInput[i]];
        }
    }
    wcaOutput[i] = 0;
    wcstombs(pcOutput, wcaOutput, BUFFER_LEN);
    return 0;
}

int CTextProcessor::single_word_stat(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    map<wchar_t, long> mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
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
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    map<wchar_t, map<wchar_t, long> > mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
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
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    map<wchar_t, map<wchar_t, long> > mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
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
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    map<wchar_t, long> mapStatSingle;
    map<wchar_t, map<wchar_t, long> > mapStatBefore;
    map<wchar_t, map<wchar_t, long> > mapStatNext;
    mapStatSingle.clear();
    mapStatBefore.clear();
    mapStatNext.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
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

int CTextProcessor::stat_msg_length(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    int aiStat[BUFFER_LEN];
    memset(aiStat, 0, sizeof(int)*BUFFER_LEN);
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    int iLen = 0;
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        iLen = (int)wcslen(wcaTmp);
        if (iLen >= BUFFER_LEN)
        {
            printf("warning: msg is too long ,lost info\n");
            continue;
        }
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
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
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


int CTextProcessor::init_common_chinese_character_data(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setCommonChineseCharacter.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_setCommonChineseCharacter.insert(wcaTmp[i]);
        }
    }
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_sub_common_chinese_character_data(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setSubCommonChineseCharacter.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_setSubCommonChineseCharacter.insert(wcaTmp[i]);
        }
    }
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_number_data(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setNumber.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_setNumber.insert(wcaTmp[i]);
        }
    }
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_alphabet_data(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setAlphabet.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_setAlphabet.insert(wcaTmp[i]);
        }
    }
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_emoji_data(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setEmoji.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_setEmoji.insert(wcaTmp[i]);
        }
    }
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_symbol_data(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setSymbol.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            m_setSymbol.insert(wcaTmp[i]);
        }
    }
    iReadFile.close();
    return 0;
}


int CTextProcessor::stat_word_cnt(char *pcInput, set<wchar_t> &m_set)
{
    wchar_t wcaTmp[BUFFER_LEN];
    mbstowcs(wcaTmp, pcInput, BUFFER_LEN);
    int iLen = (int)wcslen(wcaTmp);
    int i = 0;
    set<wchar_t>::iterator iter;
    int iStatCnt = 0;
    for (i=0; i<iLen; i++)
    {
        iter = m_set.find(wcaTmp[i]);
        if (iter != m_set.end())
        {
            iStatCnt ++;
        }
    }
    return iStatCnt;
}

int CTextProcessor::stat_common_chinese_character_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -2;
    if (m_setCommonChineseCharacter.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setCommonChineseCharacter);
    return iStatCnt;
}


int CTextProcessor::stat_sub_common_chinese_character_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -2;
    if (m_setSubCommonChineseCharacter.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setSubCommonChineseCharacter);
    return iStatCnt;
}

int CTextProcessor::stat_number_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -2;
    if (m_setNumber.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setNumber);
    return iStatCnt;
}

int CTextProcessor::stat_alphabet_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -2;
    if (m_setAlphabet.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setAlphabet);
    return iStatCnt;
}

int CTextProcessor::stat_emoji_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -2;
    if (m_setEmoji.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setEmoji);
    return iStatCnt;
}

int CTextProcessor::stat_symbol_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -2;
    if (m_setSymbol.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setSymbol);
    return iStatCnt;
}

int CTextProcessor::get_msg_word_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -2;
    wchar_t wcaTmp[BUFFER_LEN];
    mbstowcs(wcaTmp, pcInput, BUFFER_LEN);
    return (int)wcslen(wcaTmp);
}

int CTextProcessor::init_svm_model(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    if ((m_pSvmModel = svm_load_model(pcFileName)) == 0)
    {
        return -3;
    }
    return 0;
}

double CTextProcessor::get_svm_predict(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -2;
    if (m_pSvmModel == NULL) return -3;
    double adFeatureVector[BUFFER_LEN];
    int iFeatureCnt = 0;
    iFeatureCnt = get_feature(adFeatureVector, pcInput);
    struct svm_node stSvmNode[BUFFER_LEN];
    int i = 0;
    for (i = 0; i < iFeatureCnt; i++)
    {
        stSvmNode[i].index = i + 1;
        stSvmNode[i].value = adFeatureVector[i];
    }
    stSvmNode[i].index = -1;
    double dRstPredict = svm_predict(m_pSvmModel, stSvmNode);
    return dRstPredict;
}

int CTextProcessor::get_feature(double adFeatureVector[], char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -2;

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

    set<wchar_t>::iterator iter;
    mbstowcs(wcaTmp, pcInput, BUFFER_LEN);
    iMsgLen = (int)wcslen(wcaTmp);
    int i = 0;
    for (i=0; i<iMsgLen; i++)
    {
        mapMsg[wcaTmp[i]] = 1;
        iter = m_setCommonChineseCharacter.find(wcaTmp[i]);
        if (iter != m_setCommonChineseCharacter.end())
        {
            iCommonCnt ++;
            mapCommon[wcaTmp[i]] = 1;
        }
        iter = m_setSubCommonChineseCharacter.find(wcaTmp[i]);
        if (iter != m_setSubCommonChineseCharacter.end())
        {
            iSubCommonCnt ++;
            mapSubCommon[wcaTmp[i]] = 1;
        }
        iter = m_setNumber.find(wcaTmp[i]);
        if (iter != m_setNumber.end())
        {
            iNumberCnt ++;
            mapNumber[wcaTmp[i]] = 1;
        }
        iter = m_setAlphabet.find(wcaTmp[i]);
        if (iter != m_setAlphabet.end())
        {
            iAlphabetCnt ++;
            mapAlphabet[wcaTmp[i]] = 1;
        }
        iter = m_setEmoji.find(wcaTmp[i]);
        if (iter != m_setEmoji.end())
        {
            iEmojiCnt ++;
            mapEmoji[wcaTmp[i]] = 1;
        }
        iter = m_setSymbol.find(wcaTmp[i]);
        if (iter != m_setSymbol.end())
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

    adFeatureVector[0] = (double)iMsgLen;
    adFeatureVector[1] = (double)iMsgUniqCnt;
    adFeatureVector[2] = (double)iCommonCnt;
    adFeatureVector[3] = (double)iCommonUniqCnt;
    adFeatureVector[4] = (double)iSubCommonCnt;
    adFeatureVector[5] = (double)iSubCommonUniqCnt;
    adFeatureVector[6] = (double)iNumberCnt;
    adFeatureVector[7] = (double)iNumberUniqCnt;
    adFeatureVector[8] = (double)iAlphabetCnt;
    adFeatureVector[9] = (double)iAlphabetUniqCnt;
    adFeatureVector[10] = (double)iEmojiCnt;
    adFeatureVector[11] = (double)iEmojiUniqCnt;
    adFeatureVector[12] = (double)iSymbolCnt;
    adFeatureVector[13] = (double)iSymbolUniqCnt;
    adFeatureVector[14] = -1;
    return 14;                  // feature number
}

int CTextProcessor::get_sample_feature(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    if (m_setCommonChineseCharacter.empty()) return -3;
    if (m_setSubCommonChineseCharacter.empty()) return -4;
    if (m_setNumber.empty()) return -5;
    if (m_setAlphabet.empty()) return -6;
    if (m_setEmoji.empty()) return -7;
    if (m_setSymbol.empty()) return -8;
    
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
    string strLine;
    double adFeatureVector[BUFFER_LEN];
    int iFeatureCnt = 0;
    ofstream iWriterFile("rst_feature.txt", ios_base::out);
    char acOut[BUFFER_LEN];
    while (getline(iReadFile, strLine))
    {
        iFeatureCnt = 0;
        memset(adFeatureVector, 0 , sizeof(double)*BUFFER_LEN);
        get_feature(adFeatureVector, const_cast<char*>(strLine.c_str()));

        snprintf(acOut, BUFFER_LEN, "1:%d 2:%d 3:%d 4:%d 5:%d 6:%d 7:%d 8:%d 9:%d 10:%d 11:%d 12:%d 13:%d 14:%d\n"
                 ,(int)adFeatureVector[0]
                 ,(int)adFeatureVector[1]
                 ,(int)adFeatureVector[2]
                 ,(int)adFeatureVector[3]
                 ,(int)adFeatureVector[4]
                 ,(int)adFeatureVector[5]
                 ,(int)adFeatureVector[6]
                 ,(int)adFeatureVector[7]
                 ,(int)adFeatureVector[8]
                 ,(int)adFeatureVector[9]
                 ,(int)adFeatureVector[10]
                 ,(int)adFeatureVector[11]
                 ,(int)adFeatureVector[12]
                 ,(int)adFeatureVector[13]

            );
        iWriterFile << acOut;
    }
    iReadFile.close();
    iWriterFile.close();
    return 0;
}

int CTextProcessor::init_from_configuration_file(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    string strReplaceDataFile;
    string strCommonChineseCharacterDataFile;
    string strSubCommonChineseCharacterDataFile;
    string strNumberDataFile;
    string strAlphabetDataFile;
    string strEmojiDataFile;
    string strSymbolDataFile;
    string strSvmModelDataFile;
    string strIgnoreDataFile;
    string strKeyWordDataFile;
    string strFocusWordDataFile;
    string strPinYinDataFile;
    string strRegularExpressionDataFile;
    
    NZconfig oNZconfig;
    oNZconfig.read_config_file(pcFileName);
    oNZconfig.get_config_value("STR_REPLACE_DATA_FILE", strReplaceDataFile);
    oNZconfig.get_config_value("STR_COMMON_CHINESE_CHARACTER_DATA_FILE", strCommonChineseCharacterDataFile);
    oNZconfig.get_config_value("STR_SUB_COMMON_CHINESE_CHARACTER_DATA_FILE", strSubCommonChineseCharacterDataFile);
    oNZconfig.get_config_value("STR_NUMBER_DATA_FILE", strNumberDataFile);
    oNZconfig.get_config_value("STR_ALPHABET_DATA_FILE", strAlphabetDataFile);
    oNZconfig.get_config_value("STR_EMOJI_DATA_FILE", strEmojiDataFile);
    oNZconfig.get_config_value("STR_SYMBOL_DATA_FILE", strSymbolDataFile);
    oNZconfig.get_config_value("STR_SVMMODEL_DATA_FILE", strSvmModelDataFile);
    oNZconfig.get_config_value("STR_IGNORE_DATA_FILE", strIgnoreDataFile);
    oNZconfig.get_config_value("STR_KEY_WORD_DATA_FILE", strKeyWordDataFile);
    oNZconfig.get_config_value("STR_FOCUS_WORD_DATA_FILE", strFocusWordDataFile);
    oNZconfig.get_config_value("STR_PINYIN_DATA_FILE", strPinYinDataFile);
    oNZconfig.get_config_value("STR_REGULAR_EXPRESSION_DATA_FILE", strRegularExpressionDataFile);

    // 打印配置文件路径
    cout << "STR_REPLACE_DATA_FILE:" << strReplaceDataFile << endl;
    cout << "STR_COMMON_CHINESE_CHARACTER_DATA_FILE:" << strCommonChineseCharacterDataFile << endl;
    cout << "STR_SUB_COMMON_CHINESE_CHARACTER_DATA_FILE:" << strSubCommonChineseCharacterDataFile << endl;
    cout << "STR_NUMBER_DATA_FILE:" << strNumberDataFile << endl;
    cout << "STR_ALPHABET_DATA_FILE:" << strAlphabetDataFile << endl;
    cout << "STR_EMOJI_DATA_FILE:" << strEmojiDataFile << endl;
    cout << "STR_SYMBOL_DATA_FILE:" << strSymbolDataFile << endl;
    cout << "STR_SVMMODEL_DATA_FILE:" << strSvmModelDataFile << endl;
    cout << "STR_IGNORE_DATA_FILE:" << strIgnoreDataFile << endl;
    cout << "STR_KEY_WORD_DATA_FILE:" << strKeyWordDataFile << endl;
    cout << "STR_FOCUS_WORD_DATA_FILE:" << strFocusWordDataFile << endl;
    cout << "STR_PINYIN_DATA_FILE:" << strPinYinDataFile << endl;
    cout << "STR_REGULAR_EXPRESSION_DATA_FILE:" << strRegularExpressionDataFile << endl;

    int iRst = 0;
    iRst = init_replace_data(const_cast<char*>(strReplaceDataFile.c_str()));
    if(iRst < 0) return -1;
    iRst = init_common_chinese_character_data(const_cast<char*>(strCommonChineseCharacterDataFile.c_str()));
    if(iRst < 0) return -2;
    iRst = init_sub_common_chinese_character_data(const_cast<char*>(strSubCommonChineseCharacterDataFile.c_str()));
    if(iRst < 0) return -3;
    iRst = init_number_data(const_cast<char*>(strNumberDataFile.c_str()));
    if(iRst < 0) return -4;
    iRst = init_alphabet_data(const_cast<char*>(strAlphabetDataFile.c_str()));
    if(iRst < 0) return -5;
    iRst = init_emoji_data(const_cast<char*>(strEmojiDataFile.c_str()));
    if(iRst < 0) return -6;
    iRst = init_symbol_data(const_cast<char*>(strSymbolDataFile.c_str()));
    if(iRst < 0) return -7;
    iRst = init_svm_model(const_cast<char*>(strSvmModelDataFile.c_str()));
    if(iRst < 0) return -8;
    iRst = init_ignore_word(const_cast<char*>(strIgnoreDataFile.c_str()));
    if(iRst < 0) return -9;
    iRst = init_key_word(const_cast<char*>(strKeyWordDataFile.c_str()));
    if(iRst < 0) return -10;
    iRst = init_focus_word(const_cast<char*>(strFocusWordDataFile.c_str()));
    if(iRst < 0) return -11;

    iRst = init_ac_trie();
    if(iRst < 0) return -12;

    iRst = init_pinyin_word(const_cast<char*>(strPinYinDataFile.c_str()));
    if(iRst < 0) return -13;

    iRst = init_regular_expression(const_cast<char*>(strRegularExpressionDataFile.c_str()));
    if(iRst < 0) return -14;

    return 0;
}

int CTextProcessor::init_key_word(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    int iRows = 0;
    m_mapKeyWord.clear();
    while (getline(iReadFile, strLine))
    {
        if (strLine.empty())
        {
            continue;
        }
        iRows ++;
        m_mapKeyWord[iRows] = strLine;
        if (iRows >= 1000000)
        {
            break;
        }
    }
    iReadFile.close();
    return 0;
}

int CTextProcessor::init_ac_trie()
{
    struct TrieNode stRootTrieNode;
    int iRootTreePoint = 0;
    int iTreePoint = 0;
    m_mapAcTrie[iRootTreePoint] = stRootTrieNode;
    wchar_t wcaTmp[BUFFER_LEN];
    // 创建ac树
    map<int, string>::iterator iter;
    for (iter=m_mapKeyWord.begin(); iter!=m_mapKeyWord.end(); iter++)
    {
        int iKeyWordIndex = iter->first;
        string strKeyWord = iter->second;
        int iPreTreePoint = iRootTreePoint;
        mbstowcs(wcaTmp, strKeyWord.c_str(), BUFFER_LEN);
        if (strKeyWord.size() >= BUFFER_LEN)
        {
            printf("waring: keyword too long [ %s ]", strKeyWord.c_str());
        }
        int iLen = (int)wcslen(wcaTmp);
        int i = 0;
        for (i=0; i<iLen; i++)
        {
            if (m_mapAcTrie[iPreTreePoint].mapNext.find(wcaTmp[i]) == m_mapAcTrie[iPreTreePoint].mapNext.end())
            {
                iTreePoint ++;
                struct TrieNode stTmpTrieNode;
                m_mapAcTrie[iTreePoint] = stTmpTrieNode;
                m_mapAcTrie[iPreTreePoint].mapNext[wcaTmp[i]] = iTreePoint;
                iPreTreePoint = iTreePoint;
            }
            else
            {
                iPreTreePoint = m_mapAcTrie[iPreTreePoint].mapNext[wcaTmp[i]];
            }
        }
        m_mapAcTrie[iPreTreePoint].iKeyWordIndex = iKeyWordIndex;
    }

    // 构建失败指针 默认初始化时失败指针都指向了根结点 (广度遍历)
    map<wchar_t, int>::iterator iterTmp;
    map<wchar_t, int> mapNodeTmp;
    queue<int> qTmp;
    qTmp.push(iRootTreePoint);
    while (!qTmp.empty())
    {
        int iTmpNode = qTmp.front();
        qTmp.pop();
        int iFailNode = 0;
        int iNextNode = 0;
        if(m_mapAcTrie[iTmpNode].mapNext.size() > 0)
        {
            if(iTmpNode == iRootTreePoint)
            {
                for(iterTmp=m_mapAcTrie[iTmpNode].mapNext.begin(); iterTmp!=m_mapAcTrie[iTmpNode].mapNext.end(); iterTmp++)
                {
                    m_mapAcTrie[iterTmp->second].iFailPoint = iRootTreePoint;
                    qTmp.push(iterTmp->second);
                }
            }
            else
            {
                iFailNode = m_mapAcTrie[iTmpNode].iFailPoint;
                for(iterTmp=m_mapAcTrie[iTmpNode].mapNext.begin(); iterTmp!=m_mapAcTrie[iTmpNode].mapNext.end(); iterTmp++)
                {
                    while(1)
                    {
                        if(m_mapAcTrie[iFailNode].mapNext.find(iterTmp->first) != m_mapAcTrie[iFailNode].mapNext.end())
                        {
                            iNextNode = iterTmp->second;
                            m_mapAcTrie[iNextNode].iFailPoint = m_mapAcTrie[iFailNode].mapNext[iterTmp->first];
                            qTmp.push(iNextNode);
                            break;
                        }
                        else
                        {
                            if(iFailNode == iRootTreePoint)
                            {
                                iNextNode = iterTmp->second;
                                m_mapAcTrie[iNextNode].iFailPoint = iRootTreePoint;
                                qTmp.push(iNextNode);
                                break;
                            }
                            iFailNode = m_mapAcTrie[iFailNode].iFailPoint;
                        }
                    }
                    
                }
            }
        }
    }
    // 打印树结构
    // for (int i = 0; i <= iTreePoint; i++)
    // {
    //     map<wchar_t, int>::iterator iterTmp;
    //     printf("%d %d %d", i, m_mapAcTrie[i].iKeyWordIndex, m_mapAcTrie[i].iFailPoint);
    //     for (iterTmp=m_mapAcTrie[i].mapNext.begin(); iterTmp!=m_mapAcTrie[i].mapNext.end(); iterTmp ++)
    //     {
    //         printf("(%lc, %d) ", iterTmp->first, iterTmp->second);
    //     }
    //     printf("\n");
    // }
    return 0;
}

int CTextProcessor::get_all_find_key_word(map<string, int> &mapRst , char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= MSG_MAX_LEN) return -2;
    if (m_mapAcTrie.empty()) return -3;
    if (m_mapKeyWord.empty()) return -4;
    wchar_t wcaTmp[MSG_MAX_LEN];
    mbstowcs(wcaTmp, pcInput, MSG_MAX_LEN);
    int iLen = (int)wcslen(wcaTmp);
    int i = 0;
    int iRootTreePoint = 0;
    int iCurPoint = iRootTreePoint;
    map<wchar_t, int>::iterator iter;
    for (i=0; i<iLen; i++)
    {
        while (1)
        {
            iter = m_mapAcTrie[iCurPoint].mapNext.find(wcaTmp[i]);
            if (iter != m_mapAcTrie[iCurPoint].mapNext.end())
            {
                iCurPoint = m_mapAcTrie[iCurPoint].mapNext[wcaTmp[i]];
                if (m_mapAcTrie[iCurPoint].iKeyWordIndex > 0)
                {
                    mapRst[m_mapKeyWord[m_mapAcTrie[iCurPoint].iKeyWordIndex]] ++;
                }
                break;
            }
            iCurPoint = m_mapAcTrie[iCurPoint].iFailPoint;
            if (m_mapAcTrie[iCurPoint].iKeyWordIndex > 0)
            {
                mapRst[m_mapKeyWord[m_mapAcTrie[iCurPoint].iKeyWordIndex]] ++;
            }
            if (iCurPoint == iRootTreePoint)
            {
                break;
            }
        }
    }
    return 0;
}

int CTextProcessor::find_key_word(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= MSG_MAX_LEN) return -2;
    if (m_mapAcTrie.empty()) return -3;
    wchar_t wcaTmp[MSG_MAX_LEN];
    mbstowcs(wcaTmp, pcInput, MSG_MAX_LEN);
    int iLen = (int)wcslen(wcaTmp);
    int i = 0;
    int iRootTreePoint = 0;
    int iCurPoint = iRootTreePoint;
    map<wchar_t, int>::iterator iter;
    for (i=0; i<iLen; i++)
    {
        while (1)
        {
            iter = m_mapAcTrie[iCurPoint].mapNext.find(wcaTmp[i]);
            if (iter != m_mapAcTrie[iCurPoint].mapNext.end())
            {
                iCurPoint = m_mapAcTrie[iCurPoint].mapNext[wcaTmp[i]];
                if (m_mapAcTrie[iCurPoint].iKeyWordIndex > 0)
                {
                    return 1;
                }
                break;
            }
            iCurPoint = m_mapAcTrie[iCurPoint].iFailPoint;
            if (m_mapAcTrie[iCurPoint].iKeyWordIndex > 0)
            {
                return 1;
            }
            if (iCurPoint == iRootTreePoint)
            {
                break;
            }
        }
    }
    return 0;
}


int CTextProcessor::init_ignore_word(char *pcFileName)
{
    if(pcFileName == NULL) return -1;
    if(strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if(iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    int iLen = 0;
    m_setIgnore.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        iLen = (int)wcslen(wcaTmp);
        for(int i=0; i<iLen; i++)
        {
            m_setIgnore.insert(wcaTmp[i]);
        }
    }
    iReadFile.close();
    return 0;
}

int CTextProcessor::filter_ignore_word(char *pcOutput, char *pcInput)
{
    if(pcOutput == NULL || pcInput == NULL) return -1;
    if(strlen(pcInput) >= BUFFER_LEN) return -2;
    if(m_setIgnore.empty()) return -3;
    wchar_t wcaInput[BUFFER_LEN];
    wchar_t wcaOutput[BUFFER_LEN];
    mbstowcs(wcaInput, pcInput, BUFFER_LEN);
    int iLen = (int)wcslen(wcaInput);
    int j = 0;
    for(int i=0; i<iLen; i++)
    {
        if(m_setIgnore.find(wcaInput[i]) != m_setIgnore.end())
        {
            continue;
        }
        wcaOutput[j] = wcaInput[i];
        j++;
    }
    wcaOutput[j] = 0;
    wcstombs(pcOutput, wcaOutput, BUFFER_LEN);
    return 0;
}

int CTextProcessor::init_focus_word(char *pcFileName)
{
    if(pcFileName == NULL) return -1;
    if(strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if(iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    int iLen = 0;
    m_setFocus.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        iLen = (int)wcslen(wcaTmp);
        for(int i=0; i<iLen; i++)
        {
            m_setFocus.insert(wcaTmp[i]);
        }
    }
    iReadFile.close();
    return 0;
}

int CTextProcessor::filter_not_focus_word(char *pcOutput, char *pcInput)
{
    if(pcOutput == NULL || pcInput == NULL) return -1;
    if(strlen(pcInput) >= BUFFER_LEN) return -2;
    if(m_setFocus.empty()) return -3;
    wchar_t wcaInput[BUFFER_LEN];
    wchar_t wcaOutput[BUFFER_LEN];
    mbstowcs(wcaInput, pcInput, BUFFER_LEN);
    int iLen = (int)wcslen(wcaInput);
    int j = 0;
    for(int i=0; i<iLen; i++)
    {
        if(m_setFocus.find(wcaInput[i]) == m_setFocus.end())
        {
            continue;
        }
        wcaOutput[j] = wcaInput[i];
        j++;
    }
    wcaOutput[j] = 0;
    wcstombs(pcOutput, wcaOutput, BUFFER_LEN);
    return 0;
}

int CTextProcessor::init_pinyin_word(char *pcFileName)
{
    if(pcFileName == NULL) return -1;
    if(strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if(iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_mapPinYin.clear();
    while (getline(iReadFile, strLine))
    {
        if(strLine.length() >= BUFFER_LEN)
        {
            continue;
        }
        mbstowcs(wcaTmp, strLine.c_str(), BUFFER_LEN);
        wstring wstrTmp(wcaTmp);
        wstring::size_type iPos;
        iPos = wstrTmp.find(' ');
        if(iPos == wstrTmp.npos)
        {
            continue;
        }
        m_mapPinYin[wcaTmp[0]] = wstrTmp.substr(iPos+1);
    }
    return 0;
}

int CTextProcessor::translate_word_to_pinyin(char *pcOutput, char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= MSG_MAX_LEN) return -2;
    if (m_mapPinYin.empty()) return -3;
    wchar_t wcaInput[BUFFER_LEN];
    wchar_t wcaOutput[BUFFER_LEN];
    mbstowcs(wcaInput, pcInput, BUFFER_LEN);
    int iLen = (int)wcslen(wcaInput);
    wstring wstrTmp;
    wstrTmp.clear();
    for (int i=0; i < iLen; i++)
    {
        if(m_mapPinYin.find(wcaInput[i]) == m_mapPinYin.end())
        {
            wstrTmp += wcaInput[i];
        }
        else
        {
            wstrTmp += L" ";
            wstrTmp += m_mapPinYin[wcaInput[i]];
            wstrTmp += L" ";
        }
    }
    if(wstrTmp.length() >= BUFFER_LEN) return -4;
    wcsncpy(wcaOutput, wstrTmp.c_str(), wstrTmp.length());
    wcstombs(pcOutput, wcaOutput, BUFFER_LEN);
    return 0;
}

int CTextProcessor::init_regular_expression(char *pcFileName)
{
    if(pcFileName == NULL) return -1;
    if(strlen(pcFileName) >= BUFFER_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if(iReadFile.fail()) return -3;
    string strLine;
    m_vecRegularExpression.clear();
    while (getline(iReadFile, strLine))
    {
        m_vecRegularExpression.push_back(strLine);
    }
    return 0;
}

int CTextProcessor::match_regular_expression(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= MSG_MAX_LEN) return -2;
    bool bIsMatch = false;
    vector<string>::iterator iter;
    for(iter=m_vecRegularExpression.begin(); iter!=m_vecRegularExpression.end(); iter++)
    {
        regex_t reg;
        // 共有如下四种编译模式：
        // REG_EXTENDED：使用功能更强大的扩展正则表达式
        // REG_ICASE：忽略大小写
        // REG_NOSUB：不用存储匹配后的结果(ps:提升性能)
        // REG_NEWLINE：识别换行符，这样‘$’就可以从行尾开始匹配，‘^’就可以从行的开头开始匹配。否则忽略换行符，把整个文本串当做一个字符串处理。
        regcomp(&reg, (*iter).c_str(), REG_EXTENDED|REG_NOSUB);
        int iRst = 0;
        const size_t nmatch = 1;
        regmatch_t pmatch[1];
        iRst = regexec(&reg, pcInput, nmatch, pmatch, 0);
        regfree(&reg);
        if(iRst == 0)
        {
            bIsMatch = true;
            break;
        }
    }
    if(bIsMatch == false)
    {
        return -3;
    }
    return 0;
}
