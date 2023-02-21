#pragma once

#include "Replacer.h"

class LRUReplacer : public Replacer {

public:
    //LRUReplacer();
    LRUReplacer(int max_size = DEFBUFSIZE);
    ~LRUReplacer();
    int Victim();
    void Insert(int frame_id);
    void Remove();
    void Fix(int frame_id);
    int Size();
    void Print();

private:
    LRUNode* Lhead, * Ltail;
    int Lsize;
    int Lmax{ DEFBUFSIZE };

    inline void set_pointer(LRUNode* p, LRUNode* q) {
        p->next = q;
        q->prev = p;
    }
};
