//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2020/04/17 02:04:37>
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
#include <set>
#include <queue>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>             /* 使用c的正则，原因速度最快 对比 c++ 、boost  */
#include <vector>



#define BUFFER_LEN 60240
#define MSG_MAX_LEN 60000
#define FILE_NAME_LEN 1024

struct TrieNode
{
    std::map<wchar_t, int> mapNext;
    int iFailPoint;
    int iKeyWordIndex;
    TrieNode(){
        mapNext.clear();
        iFailPoint = 0;
        iKeyWordIndex = 0;
    }
};

class CTextProcessor
{
public:
    CTextProcessor();
    virtual ~CTextProcessor();

    int init_replace_data(char *pcDataFileName); /* 初始化替换字符数据 */
    int replace_word(wchar_t *pwcOutput, wchar_t *pwcInput);
    int replace_word(char *pcOutput, char *pcInput);
    
    int init_common_chinese_character_data(char *pcFileName); /* 初始化常用汉字数据 */
    int init_sub_common_chinese_character_data(char *pcFileName); /* 初始化次常用汉字数据 */
    int init_number_data(char *pcFileName);
    int init_alphabet_data(char *pcFileName);
    int init_emoji_data(char *pcFileName);
    int init_symbol_data(char *pcFileName);

    int stat_common_chinese_character_cnt(char *pcInput);
    int stat_sub_common_chinese_character_cnt(char *pcInput);
    int stat_number_cnt(char *pcInput);
    int stat_alphabet_cnt(char *pcInput);
    int stat_emoji_cnt(char *pcInput);
    int stat_symbol_cnt(char *pcInput);

    int get_msg_word_cnt(char *pcInput);

    int init_corpus(char *pcFilePath);
    
    int init_key_word(char *pcFileName);
    int init_ac_trie();

    /* mspRst 返回命中关键词和命中该关键词的次数 */
    int get_all_find_key_word(std::map<std::string, int> &mapRst , char *pcInput);
    /* 命中关键词即返回 1:命中 0:未命中 */
    int find_key_word(char *pcInput);

    int init_ignore_word(char *pcFileName);
    int filter_ignore_word(char *pcOutput, char *pcInput);

    int init_focus_word(char *pcFileName); /* 加载需要保留的字 */
    int filter_not_focus_word(char *pcOutput, char *pcInput); /* 过滤不关心的字 */

    int init_pinyin_word(char *pcFileName); /* 加载汉字拼音对照表 */
    int translate_word_to_pinyin(char *pcOutput, char *pcInput); /* 翻译为拼音 */

    int init_regular_expression(char *pcFileName); /* 加载正则表达式 */
    int match_regular_expression(char *pcInput);   /* 匹配正则表达式 0：匹配 非0:不匹配 */

private:
    int stat_word_cnt(char *pcInput, std::set<wchar_t> &m_set);
    
    std::map<wchar_t, wchar_t> m_mapReplace;
    std::set<wchar_t> m_setCommonChineseCharacter;
    std::set<wchar_t> m_setSubCommonChineseCharacter;
    std::set<wchar_t> m_setNumber;
    std::set<wchar_t> m_setAlphabet;
    std::set<wchar_t> m_setEmoji;
    std::set<wchar_t> m_setSymbol;
    std::set<wchar_t> m_setIgnore;
    std::set<wchar_t> m_setFocus;
    std::map<wchar_t, std::wstring> m_mapPinYin;
    std::vector<std::string> m_vecRegularExpression;
    
    std::map<int, std::string> m_mapKeyWord; /* 关键词 */
    std::map<int, struct TrieNode> m_mapAcTrie; /* AC 自动机 trie树 */

};


#endif /* TEXT_PROCESSOR_H */
