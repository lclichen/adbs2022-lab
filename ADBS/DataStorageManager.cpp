#include "include/DataStorageManager.h"

DSMgr::DSMgr() {
    numPages = 0;
    ReadCount = 0;
    WriteCount = 0;
    for (int i = 0; i < MAXPAGES; i++) {
        pages[i] = 0;
    }
}

DSMgr::DSMgr(std::string filename) {
    OpenFile(filename);
}

DSMgr::~DSMgr() {
    CloseFile();
}

int DSMgr::OpenFile(std::string filename){
    currFile = fopen(filename.c_str(), "r+");
    if (currFile == nullptr) {
        currFile = fopen(filename.c_str(), "w");
        currFile = freopen(filename.c_str(), "r+", currFile);
    }
    else {
        fseek(currFile, 0L, SEEK_END);
        long sz = ftell(currFile);//函数 ftell 用于得到文件位置指针当前位置相对于文件首的偏移字节数。
        numPages = sz / FRAMESIZE;
    }
    return currFile != nullptr;
}

int DSMgr::CloseFile() {
    return fclose(currFile);
}

bFrame DSMgr::ReadPage(int page_id) {
    bFrame data;
    if (Seek(page_id * FRAMESIZE,0)) {
        fprintf(stderr, "Error: cannot find page: %d\n", page_id);
        exit(1);
    }
    fread(data.field, FRAMESIZE, 1, currFile);
    SetUse(page_id,1);
    ReadCount++;
    return data;
}

int DSMgr::WritePage(int page_id, bFrame frm) {//对帧id到page_id的转换应在BMgr中进行。
    if (Seek(page_id * FRAMESIZE, SEEK_SET) != 0) {
        fprintf(stderr, "Error: cannot find page: %d\n", page_id);
        exit(1);
    }
    SetUse(page_id, 0);
    WriteCount++;
    return (int)fwrite(frm.field, FRAMESIZE, 1, currFile);
}

int DSMgr::Seek(int offset, int pos) {
    return fseek(currFile, (long)offset, pos);
}

FILE* DSMgr::GetFile() {
    return currFile;
}

void DSMgr::IncNumPages() {
    numPages++;
}

int DSMgr::GetNumPages() {
    return numPages;
}

void DSMgr::SetUse(int index, int use_bit) {
    //SetUse 函数用于设置页面数组中的位。该数组跟踪正在使用的页面。如果页面中的所有记录都被删除，那么该页面就不再真正使用，可以在数据库中再次使用。
    //为了知道页面是否可重用，检查数组中是否有任何设置为零的 use_bits。
    //fixNewPage 函数首先检查此数组的 use_bit 是否为零。如果找到，则重新使用该页面。如果不是，则分配一个新页面。
    pages[index] = use_bit;
}

int DSMgr::GetUse(int index) {
    return pages[index];
}
