#pragma once

#include <unordered_map>
#include "Replacer.h"

class LRU2Replacer : public Replacer {

public:
    //LRU2Replacer();
    LRU2Replacer(int max_size = DEFBUFSIZE);
    ~LRU2Replacer();
    int Victim();
    void Insert(int frame_id);
    void Remove();
    void Fix(int frame_id);
    int Size();
    void Print();

private:
    void Remove2();
    void Fix2(int frame_id);
    LRUNode* Lhead, * Ltail;
    LRUNode* L2head, * L2tail;
    int Lsize, L2size;
    int Lmax{ DEFBUFSIZE };
    int L2max;

    inline void set_pointer(LRUNode* p, LRUNode* q) {
        p->next = q;
        q->prev = p;
    }
};
