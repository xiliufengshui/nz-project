//============================================================================
// LastChangeTime : Time-stamp: <naturezhang 2016/05/25 00:32:44>
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

template <class T>
class CBlackPhoneShm
{
public:
    CBlackPhoneShm();
    virtual ~CBlackPhoneShm();
    inline int show_page_size(); /* 显示系统分页大小 */
    int create_hash_table(uint64_t ddwShmKey, uint32_t dwBucketSize, uint32_t dwBucketCnt);
    /* return 输入值有误：-1 正常：0 hash冲突：-2 */
    int insert_hash_table(uint64_t dwPhone);
    int get_hash_table();
    
    uint64_t m_ddwShmKey;              /* 共享内存key */
    uint32_t m_dwBucketSize;           /* 共享内存单个桶的元素个数 */
    uint32_t m_dwBucketCnt;            /* 共享内存桶的个数*/
    T *m_pShm;
    int m_iShmId;               /* 共享内存shmid */
    
};



#endif /* NZSHAREMEMORY_H */
