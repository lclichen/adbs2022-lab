#include <cstdio>

#include "include/LRU2Replacer.h"

LRU2Replacer::LRU2Replacer(int max_size) {
    Lsize = 0;
    L2size = 0;
    Lmax = max_size;
    L2max = 800;
    Lhead = new LRUNode(-1);
    Ltail = new LRUNode(-1);
    L2head = new LRUNode(-1);
    L2tail = new LRUNode(-1);
    set_pointer(Lhead, Ltail);
    set_pointer(L2head, L2tail);
}

LRU2Replacer::~LRU2Replacer() {
    /*LRUNode* p = head_, * tmp;
    for (; p != nullptr; p = tmp) {
        tmp = p->next;
        delete p;
    }*/
}

int LRU2Replacer::Victim() {//选取链表尾部的节点，返回frame_id
    if (Lsize == 0)
        return -1;
    if (L2size >= L2max || Ltail->prev->frame_id == -1) {
        return L2tail->prev->frame_id;
    }
    else
    {
        return Ltail->prev->frame_id;
    }
}

void LRU2Replacer::Insert(int frame_id) {
    if (Lsize == Lmax) {
        Remove();
    }
    LRUNode* node = new LRUNode(frame_id);
    node->next = Lhead->next;
    node->next->prev = node;
    set_pointer(Lhead, node);
    Lsize++;
}

void LRU2Replacer::Remove() {//移除历史表尾部节点
    LRUNode* node = Ltail->prev;
    if (L2size >= L2max || node->frame_id == -1) {
        return Remove2();
    }
    set_pointer(node->prev, Ltail);
    /*node->prev->next = Ltail;
    Ltail->prev = node->prev;*/
    free(node);
    Lsize--;
}
void LRU2Replacer::Remove2() {//移除缓存表尾部节点
    LRUNode* node = L2tail->prev;
    set_pointer(node->prev, L2tail);
    /*node->prev->next = L2tail;
    L2tail->prev = node->prev;*/
    free(node);
    Lsize--;
    L2size--;
}

void LRU2Replacer::Fix(int frame_id) {
    //LRU2的第一个链纯顺序即可。
    //LRU2的第二个链才需要LRU操作。
    //目测是先找缓存链，然后再找历史记录链。
    LRUNode* node = L2head->next;
    if (node->frame_id == frame_id) {
        return;
    }
    while (node->frame_id != frame_id)
    {
        if (node->frame_id == -1) {
            return Fix2(frame_id);
        }
        node = node->next;
    }
    LRUNode* nextnode = node->next;
    LRUNode* prevnode = node->prev;
    set_pointer(prevnode, nextnode);
    node->next = L2head->next;
    node->next->prev = node;
    L2head->next = node;
    node->pincount++;
}

void LRU2Replacer::Fix2(int frame_id) {
    LRUNode* node = Lhead->next;
    if (node->frame_id == frame_id) {
        set_pointer(Lhead, node->next);
    }
    else {
        while (node->frame_id != frame_id)
        {
            node = node->next;
            /*if (node->frame_id == -1) {
                printf("can`t find %d", frame_id);
                return;
            }*/
        }
        LRUNode* nextnode = node->next;
        LRUNode* prevnode = node->prev;
        set_pointer(prevnode, nextnode);
    }
    node->next = L2head->next;
    node->next->prev = node;
    set_pointer(L2head, node);
    node->pincount++;
    L2size++;
}

int LRU2Replacer::Size() {
    return Lsize;
}

void LRU2Replacer::Print() {
    printf("lru replacer:\n");
    for (LRUNode* node = Lhead->next; node != Ltail; node = node->next) {
        printf("%d ", node->frame_id);
    }
    printf("\n");
}