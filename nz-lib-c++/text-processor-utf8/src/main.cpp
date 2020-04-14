//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2020/04/15 01:16:25>
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
    // if(argc != 3) return -2;
    // int iRst = 0;
    CTextProcessor oCTextProcessor;
    int iRst = oCTextProcessor.init_corpus("../conf/corpus");
    cout << "iRst: " << iRst << endl;
    
    // oCTextProcessor.init_pinyin_word("./corpus/pinyin_utf8.txt");
    char pcOutput[BUFFER_LEN];
    oCTextProcessor.translate_word_to_pinyin(pcOutput, "你好吗");
    cout<< "pcOutPut: " << pcOutput << endl;
    
    // oCTextProcessor.test_print_pinyin();
    // cout << oCTextProcessor.init_regular_expression("./corpus/regular_expression_utf8.txt") << endl;
    // cout << "test: " << oCTextProcessor.match_regular_expression("23423123") << endl;
    // cout << "test: " << oCTextProcessor.match_regular_expression("343345345") << endl;
    
    // iRst = oCTextProcessor.init_from_configuration_file(argv[1]);
    // if(iRst < 0)
    // {
    //     cout << "error: init config file failed! error code: " << iRst << endl;
    //     return -1;
    // }
    // cout << "paramter 2 is: " << argv[2] << endl;
    // char acOut[1024];
    // oCTextProcessor.filter_not_focus_word(acOut, argv[2]);
    // cout << "output: " << acOut << endl;
    // cout << "finish" << endl;
    return 0;
}
