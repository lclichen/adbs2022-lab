#pragma once

#include <list>
#include <unordered_map>
#include <mutex>

#include "DataStorageManager.h"
#include "LRUReplacer.h"
#include "LRU2Replacer.h"

#define FRAME_NUM 1024

//enum ReplacePolicy { Invalid = 0, Lru, Clock };

class BMgr
{
public:
    BMgr(std::string filename, int choice, int input_frame_num = FRAME_NUM);
    ~BMgr();
    // Interface functions接口功能
    //int FixPage(int page_id);
    int FixPage(int page_id, int prot);//函数原型是FixPage(Page_id, protection)，返回一个 frame_id。文件和访问管理器将使用记录的record_id中的page_id调用此页面。
    //该函数查看页面是否已经在缓冲区中，如果是，则返回相应的frame_id。如果该页面尚未驻留在缓冲区中，它会选择一个受害页面（如果需要）并加载请求的页面。
    void FixNewPage();
    int UnfixPage(int page_id);
    int NumFreeFrames();//查看缓冲区并返回空闲且可以使用的缓冲区页数。这对于查询处理器的N路排序特别有用。返回一个从 0 到 BUFFERSIZE-1(1023) 的整数。
    // Internal Functions内部功能
    int SelectVictim();
    BCB* Hash(int page_id);
    void RemoveBCB(BCB* ptr, int page_id);
    void RemoveLRUEle(int frid);
    void SetDirty(int frame_id);
    void UnsetDirty(int frame_id);
    void WriteDirtys();
    void PrintFrame(int frame_id);
    bFrame* buf;//缓冲区
    int InBufferCount;
    int OutBufferCount;
    int GetIONum(int choice);
private:
    DSMgr dsmgr;//存储管理器实例
    int frame_num;
    // Hash Table

    int* ftop;//frame_id 到 page_id 的表：int hTable[BufferSize]
    BCB** ptof;//page_id 到 BCB 的哈希表：BCB hTable[BufferSize]
    Replacer *lru;
};