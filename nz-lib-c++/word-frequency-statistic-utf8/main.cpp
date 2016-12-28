//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/12/28 01:29:41>
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
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "example: " << argv[0] << " filename" << endl;
        return 0;
    }
    setlocale(LC_ALL, "");
    map<wchar_t, long> mapStat;
    mapStat.clear();
    ifstream iReadFile;
    iReadFile.open(argv[1], ios_base::in);
    if (iReadFile.fail()) 
    {
        return -1;
    }
    string strLine;
    wchar_t wcaTmp[1000] = {0};
    long iTotalCnt = 0;
    while (getline(iReadFile, strLine))
    {
        // cout << strLine << endl;
        mbstowcs(wcaTmp, strLine.c_str(), 1000);
        // printf("%ld    %ls\n", wcslen(wcaTmp), wcaTmp);
        int iLen = (int)wcslen(wcaTmp);
        for (int i=0; i<iLen; i++)
        {
            iTotalCnt ++;
            mapStat[wcaTmp[i]] ++;
        }
    }
    char caTmp[10];
    char caOut[1000];
    printf("total word cnt: %ld\n", iTotalCnt);
    ofstream iWriterFile("rst.txt", ios_base::out);
    map<wchar_t, long>::iterator iter;
    for (iter = mapStat.begin(); iter != mapStat.end(); iter++)
    {
        
        wcstombs(caTmp, &(iter->first), 9);
        // cout << strlen(caTmp) << endl;
        snprintf(caOut, 1000, "%s\t%ld\t%f\n", caTmp, iter->second, ((float)(iter->second)/iTotalCnt));
        iWriterFile << caOut;
        // cout << caOut << endl;
    }
    cout << "finish" << endl;
    return 0;
}



// #include <map>
// #include <string>
// #include <iostream>
// using namespace std;
// int main()
// {
//     map<int, string> mapStudent;
//     pair<map<int, string>::iterator, bool> Insert_Pair;
//     Insert_Pair=mapStudent.insert(pair<int, string>(1, "student_one"));
//     if(Insert_Pair.second == true)
//     {
//         cout<<"Insert Successfully"<<endl;
//     }
//     else
//     {
//         cout<<"Insert Failure"<<endl;
//     }
//     Insert_Pair=mapStudent.insert(pair<int, string>(1, "student_two"));
//     if(Insert_Pair.second == true)
//     {
//         cout<<"Insert Successfully"<<endl;
//     }
//     else
//     {
//         cout<<"Insert Failure"<<endl;
//     }
//     map<int, string>::iterator  iter;
//     for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
//     {
//         cout<<iter->first<<"   "<<iter->second <<endl;
//     }
// }


// #include <map>
// #include <string>
// #include <iostream>
// using namespace std;
// int main()
// {
//     map<int, string> mapStudent;
//     mapStudent[1] =  "student_one";
//     mapStudent[1] =  "student_two";
//     mapStudent[2] =  "student_three";
//     map<int, string>::iterator  iter;
//     for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
//     {
//         cout<<iter->first<<"   "<<iter->second<<endl;
//     }
// }
