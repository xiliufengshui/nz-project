//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/05/25 00:54:17>
// Name           : NZshareMemory.cpp
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : naturezhang
// Email          : naturezhang@tencent.com
// CreatDate      : 2016年05月 4日 下午  7:53 周三
// Description    : 共享内存模块
//============================================================================


#include "NZshareMemory.h"
#include <string.h>
#include <stdlib.h>
#include "NZgetPrime.h"

template<class T>
CBlackPhoneShm<T>::CBlackPhoneShm()
{
    m_ddwShmKey = 0;
    m_dwBucketSize = 0; 
    m_dwBucketCnt = 0;
    m_pShm = NULL;
}

template<class T>
CBlackPhoneShm<T>::~CBlackPhoneShm()
{
}

template<class T>
inline int CBlackPhoneShm<T>::show_page_size()
{
    std::cout << getpagesize() << std::endl;
    return 0;
}

template<class T>
int CBlackPhoneShm<T>::create_hash_table(uint64_t ddwShmKey, uint32_t dwBucketSize, uint32_t dwBucketCnt)
{
    if(m_iSize == 0)
    {
        return -1;
    }
    int iShmId = shmget((key_t)SHM_ID, sizeof(stElement)*m_iSize, 0666|IPC_CREAT|IPC_EXCL);
    if(iShmId == -1)
    {
        return -1;
    }
    m_iShmId = iShmId;
    void * tmpMem;
    tmpMem = shmat(m_iShmId, (const void*)0, 0);
    if(*(int*)tmpMem == -1)
    {
        return -1;
    }
    m_pShm = (stElement *)tmpMem;
    memset(m_pShm, 0 , sizeof(stElement)*m_iSize);
    return 0;
}



int CBlackPhoneShm::insert_hash_table(uint64_t dwPhone)
{
    if(dwPhone < 10000 || m_pShm == NULL)
    {
        return -1;
    }
    int iExistElementIdx = is_black_phone(dwPhone);
    if(iExistElementIdx != -1)
    {
        m_pShm[iExistElementIdx].iTimes++;
        return 0;
    }
    int iIndexOffset = 0;       // 偏移量
    for (int i=0; i<10; i++)
    {
        uint32_t iElementIdx = dwPhone % m_aHashPailSize[i] + iIndexOffset;
        if(m_pShm[iElementIdx].iTimes == 0)
        {
            m_pShm[iElementIdx].dwPhone = dwPhone;
            m_pShm[iElementIdx].iTimes = 1;
            return 0;
        }
        iIndexOffset += m_aHashPailSize[i];
    }
    // hash 冲突
    return -2;
}

int CBlackPhoneShm::clear_old_data(uint64_t dwPhone)
{
    if(dwPhone < 10000 || m_pShm == NULL)
    {
        return -1;
    }
    int iExistElementIdx = is_black_phone(dwPhone);
    if(iExistElementIdx != -1)
    {
        m_pShm[iExistElementIdx].iTimes--;
        return 0;
    }
    return -2;
}

int CBlackPhoneShm::get_hash_table()
{
    int iShmId = shmget((key_t)SHM_ID, 0, 0);
    if(iShmId == -1)
    {
        return -1;
    }
    void *tmpMem = shmat(iShmId, (const void*)0, 0);
    if(*(int*)tmpMem == -1)
    {
        return -1;
    }
    m_iShmId = iShmId;
    m_pShm = (stElement *)tmpMem;
    return 0;
}

int CBlackPhoneShm::get_phone_info(uint64_t dwPhone)
{
    if(dwPhone < 10000 || m_pShm == NULL)
    {
        return -1;
    }
    int iExistElementIdx = is_black_phone(dwPhone);
    if(iExistElementIdx != -1)
    {
        return m_pShm[iExistElementIdx].iTimes;
    }
    return -2;
}

// just for test
// int main(int argc, char *argv[])
// {
//     std::cout << "nihaoma" << std::endl;
//     CBlackPhoneShm CBlackPhoneShmIns;
//     if(CBlackPhoneShmIns.get_hash_table() < 0)
//     {
//         if(CBlackPhoneShmIns.create_hash_table() < 0)
//         {
//             std::cout << "error: create hash table failed!" << std::endl;
//             return -1;
//         }
//     }
//     CBlackPhoneShmIns.insert_hash_table(18033442257);
//     std::cout << CBlackPhoneShmIns.is_black_phone(18033442257) << std::endl;
//     std::cout << CBlackPhoneShmIns.is_black_phone(15160026760) << std::endl;
//     return 0;
// }
