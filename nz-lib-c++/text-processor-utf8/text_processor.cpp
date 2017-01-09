//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2017/01/09 15:13:00>
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
    m_bIsInitReplaceDate = false;
    m_bIsInitCommonChineseCharacter = false;
    m_bIsInitSubCommonChineseCharacter = false;
    m_bIsInitNumber = false;
    m_bIsInitAlphabet = false;
    m_bIsInitEmoji = false;
    m_bIsInitSymbol = false;
    m_bIsInitSvmModel = false;
    
    snprintf(m_acReplaceDataFile, BUFFER_LEN, "%s", "replace_data_utf8.txt");
    snprintf(m_acCommonChineseCharacterDataFile, BUFFER_LEN, "%s", "common_chinese_character_utf8.txt");
    snprintf(m_acSubCommonChineseCharacterDataFile, BUFFER_LEN, "%s", "sub_common_chinese_character_utf8.txt");
    snprintf(m_acNumberDataFile, BUFFER_LEN, "%s", "number_utf8.txt");
    snprintf(m_acAlphabetDataFile, BUFFER_LEN, "%s", "alphabet_utf8.txt");
    snprintf(m_acEmojiDataFile, BUFFER_LEN, "%s", "emoji_utf8.txt");
    snprintf(m_acSymbolDataFile, BUFFER_LEN, "%s", "symbol_utf8.txt");
    snprintf(m_acSvmModelDataFile, BUFFER_LEN, "%s", "rst_feature_train.txt.model");

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
    if (m_bIsInitReplaceDate == true) return 0;
    if (pcDataFileName == NULL) return -1;
    if (strlen(pcDataFileName) >= BUFFER_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcDataFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
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

int CTextProcessor::replace_word(char *pcOutput, char *pcInput)
{
    if (pcOutput == NULL || pcInput == NULL) return -1;
    int iRst = init_replace_data(m_acReplaceDataFile);
    if (iRst != 0)
    {
        printf("error: init replace data failed!\n");
        return -1;
    }
    if (strlen(pcInput) > BUFFER_LEN)
    {
        printf("error: input string too long!\n");
        return -1;
    }
    wchar_t wcaInput[BUFFER_LEN];
    wchar_t wcaOutput[BUFFER_LEN];
    mbstowcs(wcaInput, pcInput, BUFFER_LEN);
    int iLen = (int)wcslen(wcaInput);
    int i = 0;
    for (i=0; i<iLen; i++)
    {
        if (0 == m_mapReplace[wcaInput[i]])
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
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    map<wchar_t, long> mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -1;
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
    if (iReadFile.fail()) return -1;
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
    if (iReadFile.fail()) return -1;
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
    m_mapCommonChineseCharacter.clear();
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
    m_mapSubCommonChineseCharacter.clear();
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
    m_mapNumber.clear();
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
    m_mapAlphabet.clear();
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
    m_mapEmoji.clear();
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
    m_mapSymbol.clear();
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


int CTextProcessor::set_common_chinese_character_file_name(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    memset(m_acCommonChineseCharacterDataFile, 0, BUFFER_LEN);
    strncpy(m_acCommonChineseCharacterDataFile, pcFileName, BUFFER_LEN);
    return 0;
}

int CTextProcessor::set_sub_common_chinese_character_file_name(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    memset(m_acSubCommonChineseCharacterDataFile, 0, BUFFER_LEN);
    strncpy(m_acSubCommonChineseCharacterDataFile, pcFileName, BUFFER_LEN);
    return 0;
}

int CTextProcessor::set_number_file_name(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    memset(m_acNumberDataFile, 0, BUFFER_LEN);
    strncpy(m_acNumberDataFile, pcFileName, BUFFER_LEN);
    return 0;    
}

int CTextProcessor::set_alphabet_file_name(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    memset(m_acAlphabetDataFile, 0, BUFFER_LEN);
    strncpy(m_acAlphabetDataFile, pcFileName, BUFFER_LEN);
    return 0;
}

int CTextProcessor:: set_emoji_file_name(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    memset(m_acEmojiDataFile, 0, BUFFER_LEN);
    strncpy(m_acEmojiDataFile, pcFileName, BUFFER_LEN);
    return 0;
}

int CTextProcessor::set_symbol_file_name(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    memset(m_acSymbolDataFile, 0, BUFFER_LEN);
    strncpy(m_acSymbolDataFile, pcFileName, BUFFER_LEN);
    return 0;
}

int CTextProcessor::init_svm_model(char *pcFileName)
{
    if (m_bIsInitSvmModel == true) return 0;
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    if ((m_pSvmModel = svm_load_model(pcFileName)) == 0)
    {
        printf("error: open model of svm failed\n");
        return -1;
    }
    m_bIsInitSvmModel = true;
    return 0;
}

double CTextProcessor::get_svm_predict(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= BUFFER_LEN) return -1;
    if (init_svm_model(m_acSvmModelDataFile) != 0) return -1;
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

int CTextProcessor::get_feature(double adFeatureVector[], char *pcMsg)
{
    if (pcMsg == NULL) return -1;
    if (strlen(pcMsg) >= BUFFER_LEN) return -1;
    if (init_common_chinese_character(m_acCommonChineseCharacterDataFile) != 0) return -1;
    if (init_sub_common_chinese_character(m_acSubCommonChineseCharacterDataFile) != 0) return -1;
    if (init_number(m_acNumberDataFile) != 0) return -1;
    if (init_alphabet(m_acAlphabetDataFile) != 0) return -1;
    if (init_emoji(m_acEmojiDataFile) != 0) return -1;
    if (init_symbol(m_acSymbolDataFile) != 0) return -1;

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

    mbstowcs(wcaTmp, pcMsg, BUFFER_LEN);
    iMsgLen = (int)wcslen(wcaTmp);
    int i = 0;
    for (i=0; i<iMsgLen; i++)
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

int CTextProcessor::set_svm_model_file_name(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= BUFFER_LEN) return -1;
    memset(m_acSvmModelDataFile, 0, BUFFER_LEN);
    strncpy(m_acSvmModelDataFile, pcFileName, BUFFER_LEN);
    return 0;
}

int CTextProcessor::reload_init_data()
{
    m_bIsInitReplaceDate = false;
    m_bIsInitCommonChineseCharacter = false;
    m_bIsInitSubCommonChineseCharacter = false;
    m_bIsInitNumber = false;
    m_bIsInitAlphabet = false;
    m_bIsInitEmoji = false;
    m_bIsInitSymbol = false;
    m_bIsInitSvmModel = false;
    if (init_replace_data(m_acReplaceDataFile) != 0) return -1;
    if (init_common_chinese_character(m_acCommonChineseCharacterDataFile) != 0) return -1;
    if (init_sub_common_chinese_character(m_acSubCommonChineseCharacterDataFile) != 0) return -1;
    if (init_number(m_acNumberDataFile) != 0) return -1;
    if (init_alphabet(m_acAlphabetDataFile) != 0) return -1;
    if (init_emoji(m_acEmojiDataFile) != 0) return -1;
    if (init_symbol(m_acSymbolDataFile) != 0) return -1;
    if (init_svm_model(m_acSvmModelDataFile) != 0) return -1;
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
    return 0;
}
