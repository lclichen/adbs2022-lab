#include "include/BufferManager.h"

#include <cassert>
#include <cstdio>

BMgr::BMgr(std::string filename, int choice, int input_frame_num) {
    printf("缓冲区容量%d\n", input_frame_num);
    frame_num = input_frame_num;
    ftop = new int[frame_num];//frame_id 到 page_id 的表：int hTable[BufferSize]
    ptof = new BCB*[frame_num];//page_id 到 BCB 的哈希表：BCB hTable[BufferSize]
    for (int i = 0; i < frame_num; i++) {
        ftop[i] = -1;//初始化frame到page映射的哈希表，全部置为-1，表示为空
        ptof[i] = NULL;//初始化page到frame映射的哈希表，全部置为NULL，表示为空指针
    }
    dsmgr.OpenFile(filename);
    buf = (bFrame*)malloc(frame_num * sizeof(bFrame));
    switch (choice)
    {
    case(1):
        lru = new LRUReplacer(frame_num);
        break;
    case(2):
        lru = new LRU2Replacer(frame_num);
        break;
    default:
        lru = new LRUReplacer(frame_num);
        break;
    }
    InBufferCount = 0;
    OutBufferCount = 0;
}

BMgr::~BMgr() {
    WriteDirtys();
}

int BMgr::FixPage(int page_id, int prot) {
    //这个函数的原型是FixPage(Page_id, protection)，它返回一个 frame_id。文件和访问管理器将使用记录的 record_id 中的 page_id 调用此页面。该函数查看页面是否已经在缓冲区中，如果是，则返回相应的 frame_id。如果该页面尚未驻留在缓冲区中，它会选择一个受害页面（如果需要）并加载到请求的页面中。
    BCB* bcb = Hash(page_id);
    //printf("FixPage id: %d, dirty: %d.\n", page_id, prot);
    if (bcb != NULL) {//在缓冲区
        InBufferCount++;
        lru->Fix(bcb->frame_id);//LRU中将对应节点提到最前
        if (prot) {
            SetDirty(bcb->frame_id);
        }
        return bcb->frame_id;
    } else {//不在缓冲区
        OutBufferCount++;
        int frame_id;
        if (NumFreeFrames()) {//1-1024
            frame_id = frame_num - NumFreeFrames();//0-1023
        }
        else//0
        {
            frame_id = SelectVictim();
        }
        //一系列加载动作
        buf[frame_id] = dsmgr.ReadPage(page_id);
        lru->Insert(frame_id);
        bcb = new BCB(page_id,frame_id);
        BCB* prebcb = ptof[page_id % frame_num];
        if (prebcb != NULL) {
            while (prebcb->next!=NULL) {
                prebcb=prebcb->next;
            }
            prebcb->next = bcb;
        }
        else
        {
            ptof[page_id % frame_num] = bcb;
        }
        ftop[frame_id] = page_id;
        if (prot) {
            SetDirty(bcb->frame_id);
        }
        return frame_id;
    }
}
void BMgr::FixNewPage() {
    //此函数的原型是 FixNewPage()，它返回一个 page_id 和一个 frame_id。当插入、索引拆分或对象创建时需要新页面时使用此函数。返回 page_id 以便分配给 record_id 和元数据。此函数将找到一个空页面，文件和访问管理器可以使用该页面来存储一些数据。

}
int BMgr::UnfixPage(int page_id) {
    return 0;
    //这个函数的原型是 UnfixPage(page_id)，它返回一个 frame_id。此函数是对 FixPage 或 FixNewPage 调用的补充。此函数减少帧上的fix(固定)计数。如果计数减少到零，则页面上的锁被移除，并且如果选中，则帧可以被移除。page_id 被转换为 frame_id 并且它可能被解锁，以便在页面上的计数减少到零时，可以将它选为受害者页面。
}
int BMgr::NumFreeFrames() {
    return frame_num - lru->Size();
    //NumFreeFrames 函数查看缓冲区并返回空闲且可以使用的缓冲区页数。这对于查询处理器的N路排序特别有用。该函数的原型类似于 NumFreeFrames() 并返回一个从 0 到 BUFFERSIZE-1(1023) 的整数。
}
// Internal Functions
int BMgr::SelectVictim() {//使用LRU算法选取最旧的帧
    int frame_id = lru->Victim();
    int victim_page_id = ftop[frame_id];
    BCB* bcb = Hash(victim_page_id);
    RemoveBCB(bcb, victim_page_id);
    //printf("SelectVictim frameid: %d", frame_id);
    return frame_id;
}
BCB* BMgr::Hash(int page_id) {
    int bid = page_id % frame_num;
    BCB* bcb = ptof[bid];
    while (bcb != NULL) {
        if (bcb->page_id == page_id) {
            return bcb;
        }
        bcb = bcb->next;
    }
    return NULL;
}

void BMgr::RemoveBCB(BCB* ptr, int page_id) {
    int bid = page_id % frame_num;
    //printf("RemoveBCB: %d\n",page_id);
    BCB* bcb = ptof[bid];
    if (bcb == ptr) {
        ptof[bid] = ptr->next;
    }
    else
    {
        while (bcb->next != ptr)
        {
            bcb = bcb->next;
        }
        bcb->next = ptr->next;
    }
    if (ptr->dirty) {
        dsmgr.WritePage(ptr->page_id, buf[ptr->frame_id]);
    }
    free(ptr);
}
void BMgr::RemoveLRUEle(int frid) {

}
void BMgr::SetDirty(int frame_id) {
    int page_id = ftop[frame_id];
    BCB* bcb = Hash(page_id);
    if (bcb) {
        bcb->dirty = 1;
    }
}
void BMgr::UnsetDirty(int frame_id) {
    int page_id = ftop[frame_id];
    BCB* bcb = Hash(page_id);
    if (bcb) {
        bcb->dirty = 0;
    }
}
void BMgr::WriteDirtys() {
    BCB* bcb;
    for (int i = 0; i < frame_num; i++) {
        bcb = Hash(ftop[i]);
        if (bcb->dirty) {
            dsmgr.WritePage(bcb->page_id, buf[bcb->frame_id]);
        }
    }
}
int BMgr::GetIONum(int choice) {
    switch (choice)
    {
    case 0:
        return dsmgr.ReadCount;
        break;
    case 1:
        return dsmgr.WriteCount;
        break;
    default:
        return dsmgr.ReadCount;
        break;
    }
    
}
void BMgr::PrintFrame(int frame_id) {

}