//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2017/01/09 15:01:49>
// Name           : text_processor.h
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : 张健
// CreatDate      : 2017年01月 2日 PM  2:26 Mon
// Description    : 
//============================================================================


#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include "svm.h"

using namespace std;

#define BUFFER_LEN 1024

class CTextProcessor
{
public:
    CTextProcessor();
    virtual ~CTextProcessor();
    int init_replace_data(char *pcDataFileName); /* 初始化替换字符数据 */
    int replace_word(wchar_t *pwcOutput, wchar_t *pwcInput);
    int replace_word(char *pcOutput, char *pcInput);
    int single_word_stat(char *pcFileName);
    int before_word_stat(char *pcFileName);
    int next_word_stat(char *pcFileName);
    int get_all_stat(char *pcFileName);
    int set_replace_date_file_name(char *pcFileName);
    int stat_msg_length(char *pcFileName);
    int tag_msg_length(char *pcFileName);
    int get_sample_feature(char *pcFileName);
    int init_common_chinese_character(char *pcFileName); /* 初始化常用汉字数据 */
    int init_sub_common_chinese_character(char *pcFileName); /* 初始化次常用汉字数据 */
    int init_number(char *pcFileName);
    int init_alphabet(char *pcFileName);
    int init_emoji(char *pcFileName);
    int init_symbol(char *pcFileName);
    int set_common_chinese_character_file_name(char *pcFileName);
    int set_sub_common_chinese_character_file_name(char *pcFileName);
    int set_number_file_name(char *pcFileName);
    int set_alphabet_file_name(char *pcFileName);
    int set_emoji_file_name(char *pcFileName);
    int set_symbol_file_name(char *pcFileName);
    int init_svm_model(char *pcFileName);
    double get_svm_predict(char *pcInput);
    int get_feature(double adFeatureVector[], char *pcMsg);
    int set_svm_model_file_name(char *pcFileName);
    int reload_init_data();
    int init_from_configuration_file(char *pcFileName);

    map<wchar_t, wchar_t> m_mapReplace;
    map<wchar_t, int> m_mapCommonChineseCharacter;
    map<wchar_t, int> m_mapSubCommonChineseCharacter;
    map<wchar_t, int> m_mapNumber;
    map<wchar_t, int> m_mapAlphabet;
    map<wchar_t, int> m_mapEmoji;
    map<wchar_t, int> m_mapSymbol; 
    
    bool m_bIsInitReplaceDate;
    bool m_bIsInitCommonChineseCharacter;
    bool m_bIsInitSubCommonChineseCharacter;
    bool m_bIsInitNumber;
    bool m_bIsInitAlphabet;
    bool m_bIsInitEmoji;
    bool m_bIsInitSymbol;
    bool m_bIsInitSvmModel;
        
    char m_acReplaceDataFile[BUFFER_LEN];
    char m_acCommonChineseCharacterDataFile[BUFFER_LEN];
    char m_acSubCommonChineseCharacterDataFile[BUFFER_LEN];
    char m_acNumberDataFile[BUFFER_LEN];
    char m_acAlphabetDataFile[BUFFER_LEN];
    char m_acEmojiDataFile[BUFFER_LEN];
    char m_acSymbolDataFile[BUFFER_LEN];
    char m_acSvmModelDataFile[BUFFER_LEN];
    
    struct svm_model* m_pSvmModel;
};


#endif /* TEXT_PROCESSOR_H */
