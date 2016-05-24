//============================================================================
// LastChangeTime : Time-stamp: <Administrator 2016/05/20 16:24:22>
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

CBlackPhoneShm::CBlackPhoneShm()
{
    m_iShmId = 0;
    m_aHashPailSize[0] = 14999981;
    m_aHashPailSize[1] = 14999977;
    m_aHashPailSize[2] = 14999969;
    m_aHashPailSize[3] = 14999953;
    m_aHashPailSize[4] = 14999947;
    m_aHashPailSize[5] = 14999921;
    m_aHashPailSize[6] = 14999917;
    m_aHashPailSize[7] = 14999903;
    m_aHashPailSize[8] = 14999867;
    m_aHashPailSize[9] = 14999861;

    m_iSize = 0;
    for (int i=0; i<10; i++)
    {
        m_iSize += m_aHashPailSize[i];
    }

    m_pShm = NULL;
}

CBlackPhoneShm::~CBlackPhoneShm()
{
}

inline int CBlackPhoneShm::show_page_size()
{
    std::cout << getpagesize() << std::endl;
    return 0;
}

int CBlackPhoneShm::create_hash_table()
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

int CBlackPhoneShm::is_black_phone(uint64_t dwPhone)
{
    if(dwPhone < 10000 || m_pShm == NULL)
    {
        return -1;
    }
    int iIndexOffset = 0;
    for (int i=0; i<10; i++)
    {
        uint64_t iElementIdx = dwPhone % m_aHashPailSize[i] + iIndexOffset;
        if(m_pShm[iElementIdx].iTimes > 0 && m_pShm[iElementIdx].dwPhone == dwPhone)
        {
            return iElementIdx;
        }
        iIndexOffset += m_aHashPailSize[i];
    }
    return -1;
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
