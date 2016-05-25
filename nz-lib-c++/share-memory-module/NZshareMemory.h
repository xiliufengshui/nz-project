//============================================================================
// LastChangeTime : Time-stamp: <Administrator 2016/05/25 20:16:36>
// Name           : NZshareMemory.h
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : naturezhang
// Email          : naturezhang@tencent.com
// CreatDate      : 2016年05月 4日 下午  7:51 周三
// Description    : 共享内存模块
//============================================================================


#ifndef NZSHAREMEMORY_H
#define NZSHAREMEMORY_H

#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>

#define MAXBUCKETCNT 1000

template <class T>
class NZshareMemory
{
public:
    NZshareMemory();
    virtual ~NZshareMemory();
    inline int show_page_size(); /* 显示系统分页大小 */
    int create_hash_table(uint32_t dwShmKey, uint32_t dwBucketSize, uint32_t dwBucketCnt);
    int get_hash_table(uint32_t dwShmKey, uint32_t dwBucketSize, uint32_t dwBucketCnt);
    
    uint32_t m_dwShmKey;              /* 共享内存key （用户输入）*/
    uint32_t m_dwBucketSize;           /* 共享内存单个桶的元素个数（用户输入） */
    uint32_t m_dwBucketCnt;            /* 共享内存桶的个数（用户输入）*/
    uint32_t m_dwRealBucketCnt;        /* 计算素数后得到真实的桶数 */
    uint64_t m_ddwShmElementCnt;       /* 共享内存元素的个数（桶数 * 每桶的大小（素数）） */
    T *m_pShm;
    int m_iShmId;               /* 共享内存shmid */

    uint32_t m_adwBucketInfo[MAXBUCKETCNT]; /* 共享内存最多支持的桶数 */
};



#endif /* NZSHAREMEMORY_H */
