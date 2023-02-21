#pragma once

#include <iostream>
//定义帧存储结构
#define FRAMESIZE 4096//定义帧大小=页面大小=4096
struct bFrame
{
    char field[FRAMESIZE];
};

//定义缓冲区数组，数组将存储一系列帧
#define DEFBUFSIZE 1024
//bFrame buf[DEFBUFSIZE]; // or the size that the user defined by the input parameter 或用户通过输入参数定义的尺寸

#define MAXPAGES 50000

//缓冲区控制块(BCB)
//page_ids 用作将 page_id 映射到 BCB 的散列函数的键
//必须保留两个哈希表：一个将 page_ids 映射到 frame_ids 和 BCB，另一个将 frame_ids 映射到 page_ids
struct BCB
{
    BCB() {};
    int page_id;//页id
    int frame_id;//帧id
    int latch;//内存锁
    int count;//用户占用计数
    int dirty;//脏位
    BCB* next;//指向同hash值的下一项
    BCB(int page_id, int frame_id) : page_id(page_id), frame_id(frame_id), count(0), latch(0), dirty(0),next(NULL) {}    
};

struct LRUNode//LRU节点
{
    int frame_id;
    int pincount;
    LRUNode* prev;//指向链表前一项
    LRUNode* next;//指向链表下一项
    LRUNode(int frame_id) :frame_id(frame_id),pincount(1) {};
};

//哈希表的静态哈希技术。哈希函数将如下所示：

//H(k) = (page_id) % buffer_size

//缓冲区控制块将包含 page_id、frame_id、latch、count、dirty_bit
//page_id 到 BCB 的哈希表：BCB hTable[BufferSize]
//frame_id 到 page_id 的表：int hTable[BufferSize]