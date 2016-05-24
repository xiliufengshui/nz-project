//============================================================================
// LastChangeTime : Time-stamp: <Administrator 2016/04/08 13:58:38>
// Name           : black_phone_shm.h
// Version        : 1.0
// Copyright      : 裸奔的鸡蛋
// Author         : naturezhang
// Email          : naturezhang@tencent.com
// CreatDate      : 2016年03月22日 下午  2:58 周二
// Description    : QQ注册黑手机号共享内存 采用滑动时间窗口机制洗白
//============================================================================


#ifndef BLACK_PHONE_SHM_H
#define BLACK_PHONE_SHM_H

#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>

#define SHM_ID 0x88810001
/* #define ELEMENT_SIZE 19999999    /\* 素数 *\/ */
/* #define ELEMENT_SIZE 100000037  /\* 素数 *\/ */

#define HASH_PAIL_NUM 10        /* hash 桶数量 */

struct stElement
{
    int iTimes;
    uint64_t dwPhone;
};

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
    
    int m_iShmId;               /* 共享内存ID */
    int m_iSize;                /* 共享内存大小，元素个数*/
    stElement *m_pShm;
    int m_aHashPailSize[10];     /* 定义hash 10个桶 元素值为每个桶对应的大小 */

};

#endif /* BLACK_PHONE_SHM_H */
