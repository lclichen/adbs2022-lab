#pragma once

#include "Common.h"

class Replacer {
public:
    Replacer() = default;
    virtual ~Replacer() = default;
    virtual int Victim() = 0;
    virtual void Insert(int frame_id) = 0;
    virtual void Remove() = 0;
    virtual void Fix(int frame_id)=0;
    virtual void Print() = 0;
    virtual int Size() = 0;
};