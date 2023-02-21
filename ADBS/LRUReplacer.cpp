#include <cstdio>

#include "include/LRUReplacer.h"

LRUReplacer::LRUReplacer(int max_size) {
    Lsize = 0;
    Lmax = max_size;
    Lhead = new LRUNode(-1);
    Ltail = new LRUNode(-1);
    set_pointer(Lhead, Ltail);
}

LRUReplacer::~LRUReplacer() {
    /*LRUNode* p = head_, * tmp;
    for (; p != nullptr; p = tmp) {
        tmp = p->next;
        delete p;
    }*/
}

int LRUReplacer::Victim() {//选取链表尾部的节点，返回frame_id
    if (Lsize == 0)
        return -1;
    return Ltail->prev->frame_id;
}

void LRUReplacer::Insert(int frame_id) {
    LRUNode* node = new LRUNode(frame_id);
    node->next = Lhead->next;
    node->next->prev = node;
    set_pointer(Lhead, node);
    Lsize++;
    if (Lsize > Lmax) {
        Remove();
    }
}

void LRUReplacer::Remove() {//移除尾部节点
    LRUNode* node = Ltail->prev;
    set_pointer(node->prev, Ltail);
    free(node);
    Lsize--;
}

void LRUReplacer::Fix(int frame_id) {
    LRUNode* node = Lhead->next;
    if (node->frame_id == frame_id) {
        return;
    }
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

    node->next = Lhead->next;
    node->next->prev = node;
    node->prev = Lhead;
    Lhead->next = node;
}

int LRUReplacer::Size() {
    return Lsize;
}

void LRUReplacer::Print() {
    printf("lru replacer:\n");
    for (LRUNode* node = Lhead->next; node != Ltail; node = node->next) {
        printf("%d ", node->frame_id);
    }
    printf("\n");
}