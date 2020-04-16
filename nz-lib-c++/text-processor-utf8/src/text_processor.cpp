//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2020/04/17 02:03:07>
// Name           : text_processor.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2017年01月 2日 PM  2:27 Mon
// Description    : 
//============================================================================


#include "text_processor.h"

using namespace std;

CTextProcessor::CTextProcessor()
{
    setlocale(LC_ALL, "en_US.UTF-8");
}

CTextProcessor::~CTextProcessor()
{

}

int CTextProcessor::init_replace_data(char *pcDataFileName)
{
    if (pcDataFileName == NULL) return -1;
    if (strlen(pcDataFileName) >= FILE_NAME_LEN) return -1;
    ifstream iReadFile;
    iReadFile.open(pcDataFileName, ios_base::in);
    if (iReadFile.fail()) return -2;
    string strLine;
    wchar_t wcaTmp[5];
    int iRows = 0;
    m_mapReplace.clear();
    while (getline(iReadFile, strLine))
    {
        if (2 == mbstowcs(wcaTmp, strLine.c_str(), 4))
        {
            m_mapReplace[wcaTmp[0]] = wcaTmp[1];
        }
        else
        {
            // printf("warning: read line wrong [ %s ]\n", strLine.c_str());
            continue;
        }
        iRows ++;
        if (iRows > 1000000)
        {
            printf("waring: file too large not read all data!\n");
            return -3;
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
    int iLen = mbstowcs(wcaInput, pcInput, (BUFFER_LEN - 1));
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

int CTextProcessor::init_common_chinese_character_data(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setCommonChineseCharacter.clear();
    while (getline(iReadFile, strLine))
    {
        int iLen = mbstowcs(wcaTmp, strLine.c_str(), (BUFFER_LEN - 1));
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
    if (strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setSubCommonChineseCharacter.clear();
    while (getline(iReadFile, strLine))
    {
        int iLen = mbstowcs(wcaTmp, strLine.c_str(), (BUFFER_LEN - 1));
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
    if (strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setNumber.clear();
    while (getline(iReadFile, strLine))
    {
        int iLen = mbstowcs(wcaTmp, strLine.c_str(), (BUFFER_LEN - 1));
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
    if (strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setAlphabet.clear();
    while (getline(iReadFile, strLine))
    {
        int iLen = mbstowcs(wcaTmp, strLine.c_str(), (BUFFER_LEN - 1));
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
    if (strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setEmoji.clear();
    while (getline(iReadFile, strLine))
    {
        int iLen = mbstowcs(wcaTmp, strLine.c_str(), (BUFFER_LEN - 1));
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
    if (strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if (iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setSymbol.clear();
    while (getline(iReadFile, strLine))
    {
        int iLen = mbstowcs(wcaTmp, strLine.c_str(), (BUFFER_LEN - 1));
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
    int iLen = mbstowcs(wcaTmp, pcInput, (BUFFER_LEN - 1));
    int iStatCnt = 0;
    for (int i=0; i<iLen; i++)
    {
        if (m_set.find(wcaTmp[i]) != m_set.end())
        {
            iStatCnt ++;
        }
    }
    return iStatCnt;
}

int CTextProcessor::stat_common_chinese_character_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= FILE_NAME_LEN) return -2;
    if (m_setCommonChineseCharacter.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setCommonChineseCharacter);
    return iStatCnt;
}


int CTextProcessor::stat_sub_common_chinese_character_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= FILE_NAME_LEN) return -2;
    if (m_setSubCommonChineseCharacter.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setSubCommonChineseCharacter);
    return iStatCnt;
}

int CTextProcessor::stat_number_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= FILE_NAME_LEN) return -2;
    if (m_setNumber.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setNumber);
    return iStatCnt;
}

int CTextProcessor::stat_alphabet_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= FILE_NAME_LEN) return -2;
    if (m_setAlphabet.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setAlphabet);
    return iStatCnt;
}

int CTextProcessor::stat_emoji_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= FILE_NAME_LEN) return -2;
    if (m_setEmoji.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setEmoji);
    return iStatCnt;
}

int CTextProcessor::stat_symbol_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= FILE_NAME_LEN) return -2;
    if (m_setSymbol.empty()) return -3;
    int iStatCnt = stat_word_cnt(pcInput, m_setSymbol);
    return iStatCnt;
}

int CTextProcessor::get_msg_word_cnt(char *pcInput)
{
    if (pcInput == NULL) return -1;
    if (strlen(pcInput) >= FILE_NAME_LEN) return -2;
    wchar_t wcaTmp[BUFFER_LEN];
    return (int)mbstowcs(wcaTmp, pcInput, (BUFFER_LEN - 1));
}

int CTextProcessor::init_corpus(char *pcFilePath)
{
    if (pcFilePath == NULL) return -1;
    if (strlen(pcFilePath) >= FILE_NAME_LEN) return -2;
    string strFilePath(pcFilePath);

    string strReplaceDataFile = strFilePath + "/replace_data_utf8.txt";
    string strCommonChineseCharacterDataFile = strFilePath + "/common_chinese_character_utf8.txt";
    string strSubCommonChineseCharacterDataFile = strFilePath + "/sub_common_chinese_character_utf8.txt";
    string strNumberDataFile = strFilePath + "/number_utf8.txt";
    string strAlphabetDataFile = strFilePath + "/alphabet_utf8.txt";
    string strEmojiDataFile = strFilePath + "/emoji_utf8.txt";
    string strSymbolDataFile = strFilePath + "/symbol_utf8.txt";
    string strIgnoreDataFile = strFilePath + "/ignore_utf8.txt";
    string strKeyWordDataFile = strFilePath + "/key_word_utf8.txt";
    string strFocusWordDataFile = strFilePath + "/focus_utf8.txt";
    string strPinYinDataFile = strFilePath + "/pinyin_utf8.txt";
    string strRegularExpressionDataFile = strFilePath + "/regular_expression_utf8.txt";
    
    // 打印配置文件路径
    cout << "STR_REPLACE_DATA_FILE:" << strReplaceDataFile << endl;
    cout << "STR_COMMON_CHINESE_CHARACTER_DATA_FILE:" << strCommonChineseCharacterDataFile << endl;
    cout << "STR_SUB_COMMON_CHINESE_CHARACTER_DATA_FILE:" << strSubCommonChineseCharacterDataFile << endl;
    cout << "STR_NUMBER_DATA_FILE:" << strNumberDataFile << endl;
    cout << "STR_ALPHABET_DATA_FILE:" << strAlphabetDataFile << endl;
    cout << "STR_EMOJI_DATA_FILE:" << strEmojiDataFile << endl;
    cout << "STR_SYMBOL_DATA_FILE:" << strSymbolDataFile << endl;
    cout << "STR_IGNORE_DATA_FILE:" << strIgnoreDataFile << endl;
    cout << "STR_KEY_WORD_DATA_FILE:" << strKeyWordDataFile << endl;
    cout << "STR_FOCUS_WORD_DATA_FILE:" << strFocusWordDataFile << endl;
    cout << "STR_PINYIN_DATA_FILE:" << strPinYinDataFile << endl;
    cout << "STR_REGULAR_EXPRESSION_DATA_FILE:" << strRegularExpressionDataFile << endl;

    // 初始化
    if(0 > init_replace_data(const_cast<char*>(strReplaceDataFile.c_str()))) return -1;
    if(0 > init_common_chinese_character_data(const_cast<char*>(strCommonChineseCharacterDataFile.c_str()))) return -2;
    if(0 > init_sub_common_chinese_character_data(const_cast<char*>(strSubCommonChineseCharacterDataFile.c_str()))) return -3;
    if(0 > init_number_data(const_cast<char*>(strNumberDataFile.c_str()))) return -4;
    if(0 > init_alphabet_data(const_cast<char*>(strAlphabetDataFile.c_str()))) return -5;
    if(0 > init_emoji_data(const_cast<char*>(strEmojiDataFile.c_str()))) return -6;
    if(0 > init_symbol_data(const_cast<char*>(strSymbolDataFile.c_str()))) return -7;
    if(0 > init_ignore_word(const_cast<char*>(strIgnoreDataFile.c_str()))) return -8;
    if(0 > init_key_word(const_cast<char*>(strKeyWordDataFile.c_str()))) return -9;
    if(0 > init_focus_word(const_cast<char*>(strFocusWordDataFile.c_str()))) return -10;
    if(0 > init_ac_trie()) return -11;
    if(0 > init_pinyin_word(const_cast<char*>(strPinYinDataFile.c_str()))) return -12;
    if(0 >  init_regular_expression(const_cast<char*>(strRegularExpressionDataFile.c_str()))) return -13;


    return 0;
       
}

int CTextProcessor::init_key_word(char *pcFileName)
{
    if (pcFileName == NULL) return -1;
    if (strlen(pcFileName) >= FILE_NAME_LEN) return -2;
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
        mbstowcs(wcaTmp, strKeyWord.c_str(), (BUFFER_LEN - 1));
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
    int iLen = mbstowcs(wcaTmp, pcInput, (MSG_MAX_LEN - 1));
    int iRootTreePoint = 0;
    int iCurPoint = iRootTreePoint;
    map<wchar_t, int>::iterator iter;
    for (int i=0; i<iLen; i++)
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
    int iLen = mbstowcs(wcaTmp, pcInput, (MSG_MAX_LEN - 1));
    int iRootTreePoint = 0;
    int iCurPoint = iRootTreePoint;
    map<wchar_t, int>::iterator iter;
    for (int i=0; i<iLen; i++)
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
    if(strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if(iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setIgnore.clear();
    while (getline(iReadFile, strLine))
    {
        int iLen = mbstowcs(wcaTmp, strLine.c_str(), (BUFFER_LEN - 1));
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
    int iLen = mbstowcs(wcaInput, pcInput, (BUFFER_LEN - 1));
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
    if(strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if(iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_setFocus.clear();
    while (getline(iReadFile, strLine))
    {
        int iLen = mbstowcs(wcaTmp, strLine.c_str(), (BUFFER_LEN - 1));
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
    int iLen = mbstowcs(wcaInput, pcInput, (BUFFER_LEN - 1));
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
    if(strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if(iReadFile.fail()) return -3;
    string strLine;
    wchar_t wcaTmp[BUFFER_LEN];
    m_mapPinYin.clear();
    while (getline(iReadFile, strLine))
    {
        mbstowcs(wcaTmp, strLine.c_str(), (BUFFER_LEN - 1));
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
    memset(wcaInput, 0, sizeof(wcaInput));
    memset(wcaOutput, 0, sizeof(wcaOutput));
    int iLen = mbstowcs(wcaInput, pcInput, (BUFFER_LEN - 1));
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
    if(strlen(pcFileName) >= FILE_NAME_LEN) return -2;
    ifstream iReadFile;
    iReadFile.open(pcFileName, ios_base::in);
    if(iReadFile.fail()) return -3;
    string strLine;
    m_vecRegularExpression.clear();
    while (getline(iReadFile, strLine))
    {
        if(strLine.empty())
        {
            continue;
        }
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
