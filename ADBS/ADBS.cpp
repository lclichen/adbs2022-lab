// ADBS.cpp: 定义应用程序的入口点。
//

#include "ADBS.h"
#include "include/BufferManager.h"
using namespace std;

int main()
{
    //char* filename = const_cast<char*>(cmd.rest()[0].c_str());
    string filename = "data-5w-50w-zipf.txt";
    FILE* db_file = fopen("data.dbf", "r");
    /* create db with certain size */
    if (db_file == NULL) {
        db_file = fopen("data.dbf", "w");
        void* buffer = malloc(50000 * FRAMESIZE);
        fwrite(buffer, FRAMESIZE, 50000, db_file);
        free(buffer);
    }
    fclose(db_file);
    //printf("dbfile exist.");
    int choice;
    printf("输入1选择LRU算法，输入2选择LRU-2算法：");
    scanf("%d", &choice);
    if (choice == 2) {
        printf("LRU-2算法设定缓存链表长度为800\n");
    }
    clock_t start_time = clock();
    /* create buffer pool manager */
    BMgr* bmgr = new BMgr("data.dbf",choice);
    /* read data file */
    FILE* data_file = fopen(filename.c_str(), "r");
    if (data_file == NULL) {
        fprintf(stderr, "Error: file %s doesn't exist\n", filename);
        exit(1);
    }
    printf("成功打开文件...\n");
    int is_dirty, page_id;
    while (fscanf(data_file, "%d,%d", &is_dirty, &page_id) != EOF) {
        bmgr->FixPage(page_id, is_dirty);
        //printf("\r执行行数%d", line++);
        //bmgr->UnfixPage(page_id);
    }
    printf("执行完毕！\n命中计数: %d, 未命中计数: %d, 命中率%.2f%%\n", bmgr->InBufferCount, bmgr->OutBufferCount, (float)bmgr->InBufferCount*100.0/(bmgr->InBufferCount + bmgr->OutBufferCount));
    printf("读页次数: %d, 写页次数: %d, IO总次数: %d\n", bmgr->GetIONum(0), bmgr->GetIONum(1), bmgr->GetIONum(0) + bmgr->GetIONum(1));
    printf("运行时间: %.2fs\n", (double)(clock() - start_time) / CLOCKS_PER_SEC);
    fclose(data_file);
    system("pause");
    return 0;
}
