//============================================================================
// LastChangeTime : Time-stamp: <Administrator 2016/05/25 19:34:13>
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
NZshareMemory<T>::NZshareMemory()
{
    m_ddwShmKey = 0;
    m_dwBucketSize = 0; 
    m_dwBucketCnt = 0;
    m_dwRealBucketCnt = 0;
    m_ddwShmElementCnt = 0;
    m_iShmId = 0;
    m_pShm = NULL;
}

template<class T>
NZshareMemory<T>::~NZshareMemory()
{
}

template<class T>
inline int NZshareMemory<T>::show_page_size()
{
    std::cout << getpagesize() << std::endl;
    return 0;
}

template<class T>
int NZshareMemory<T>::create_hash_table(uint32_t dwShmKey, uint32_t dwBucketSize, uint32_t dwBucketCnt)
{
    if(dwBucketSize == 0 || dwBucketCnt == 0 || ddwShmKey == 0 || dwBucketCnt > MAXBUCKETCNT) return -1;
    int iRst = get_Prime(dwBucketSize, m_adwBucketInfo, dwBucketCnt);
    if(iRst < 0) return -2;
    m_ddwShmKey = ddwShmKey;
    m_dwBucketSize = dwBucketSize;
    m_dwBucketCnt = dwBucketCnt;
    m_dwRealBucketCnt = iRst;
    for(int i=0; i<=m_dwRealBucketCnt; i++)
    {
        m_ddwShmElementCnt += m_adwBucketInfo[i];
    }
    int iShmId = shmget((key_t)dwShmKey, sizeof(T)*m_ddwShmElementCnt, 0666|IPC_CREAT|IPC_EXCL);
    if(iShmId == -1) return -1
    m_iShmId = iShmId;
    void * tmpMem;
    tmpMem = shmat(m_iShmId, (const void*)0, 0);
    if(*(int*)tmpMem == -1) return -1;
    m_pShm = (T *)tmpMem;
    memset(m_pShm, 0 , sizeof(T)*m_ddwShmElementCnt);
    return 0;
}

template<class T>
int NZshareMemory<T>::get_hash_table(uint32_t dwShmKey, uint32_t dwBucketSize, uint32_t dwBucketCnt)
{
    if(dwBucketSize == 0 || dwBucketCnt == 0 || ddwShmKey == 0 || dwBucketCnt > MAXBUCKETCNT) return -1;
    int iRst = get_Prime(dwBucketSize, m_adwBucketInfo, dwBucketCnt);
    if(iRst < 0) return -2;
    m_ddwShmKey = ddwShmKey;
    m_dwBucketSize = dwBucketSize;
    m_dwBucketCnt = dwBucketCnt;
    m_dwRealBucketCnt = iRst;
    for(int i=0; i<=m_dwRealBucketCnt; i++)
    {
        m_ddwShmElementCnt += m_adwBucketInfo[i];
    }
    int iShmId = shmget((key_t)dwShmKey, 0, 0);
    if(iShmId == -1) return -1;
    void *tmpMem = shmat(iShmId, (const void*)0, 0);
    if(*(int*)tmpMem == -1) return -1;
    m_iShmId = iShmId;
    m_pShm = (T *)tmpMem;
    return 0;
}

template<class T>
int NZshareMemory<T>::insert_hash_table(uint64_t dwPhone)
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

int NZshareMemory::clear_old_data(uint64_t dwPhone)
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


int NZshareMemory::get_phone_info(uint64_t dwPhone)
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
//     NZshareMemory NZshareMemoryIns;
//     if(NZshareMemoryIns.get_hash_table() < 0)
//     {
//         if(NZshareMemoryIns.create_hash_table() < 0)
//         {
//             std::cout << "error: create hash table failed!" << std::endl;
//             return -1;
//         }
//     }
//     NZshareMemoryIns.insert_hash_table(18033442257);
//     std::cout << NZshareMemoryIns.is_black_phone(18033442257) << std::endl;
//     std::cout << NZshareMemoryIns.is_black_phone(15160026760) << std::endl;
//     return 0;
// }
