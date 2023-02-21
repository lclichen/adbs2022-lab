#pragma once

#include <iostream>
#include "Common.h"

class DSMgr
{
public:
    DSMgr();
    ~DSMgr();
    DSMgr(std::string filename);
    int OpenFile(std::string filename);
    int CloseFile();
    bFrame ReadPage(int page_id);
    int WritePage(int page_id, bFrame frm);
    int Seek(int offset, int pos);
    FILE* GetFile();
    void IncNumPages();
    int GetNumPages();
    void SetUse(int index, int use_bit);
    int GetUse(int index);
    int ReadCount;
    int WriteCount;
private:
    FILE* currFile;
    int numPages;
    int pages[MAXPAGES];
};