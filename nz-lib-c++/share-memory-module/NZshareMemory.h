//============================================================================
// LastChangeTime : Time-stamp: <Administrator 2016/05/19 20:33:21>
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

class CBlackPhoneShm
{
public:
    CBlackPhoneShm();
    virtual ~CBlackPhoneShm();
    inline int show_page_size(); /* 显示系统分页大小 */
    int create_hash_table();
    /* return 输入值有误：-1 正常：0 hash冲突：-2 */
    int insert_hash_table(uint64_t dwPhone);
    /* 判断是否是black phone  return no:-1 yes:>=0 other:-1*/
    int is_black_phone(uint64_t dwPhone);
    /* 清理数据 引用计减一 返回正常：0*/
    int clear_old_data(uint64_t dwPhone);
    int get_hash_table();

    /* 获取输入手机号的共享内存信息(引用次数) */
    int get_phone_info(uint64_t dwPhone);

    int m_iShmKey;              /* 共享内存key */
    int m_iShmId;               /* 共享内存shmid */
    int m_iSize;                /* 共享内存大小，元素个数*/
    stElement *m_pShm;
    int m_aHashPailSize[10];     /* 定义hash 10个桶 元素值为每个桶对应的大小 */

};



#endif /* NZSHAREMEMORY_H */
