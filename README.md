# adbs2022-lab

Advanced Database Systems Lab: Storage and Buffer Manager

## 题目说明

Lab 1：根据下面的设计要求，实现一个Storage and Buffer Manager。

    设计具体要求：adbs-lab.pdf（课程网站下载）

1. 按文档要求实现一个Storage and Buffer Manager，要求至少实现LRU算法。

2. 底层文件默认采用目录式堆文件。

3. 建议先构建一个5万个page（页号从0到49999）的堆文件（使用自己实现的FixNewPage()接口），然后再运行trace文件：data-5w-50w-zipf.txt（包含了50万次满足Zipfan分布-0.8的页面请求，即80%的请求集中在前20%的页号上），根据设计文档要求统计磁盘IO、Buffer命中率、运行时间等数据。

4. *下面的实验为选做内容，不做强制要求。如果完成了选做实验（一个或者多个），最后实验成绩会根据完成的情况适当加分：
（1）实现CLOCK、LRU-2、LIRS等其它缓存置换算法（至少一种），并与LRU算法的性能进行对比；
（2）加入缓存请求时的并发控制，通过内存锁(latch)解决缓存访问时的冲突；要求通过多线程方式执行trace并给出测试结果。

## 详细介绍及报告

[高级数据库系统课程实验Storage and Buffer Manager](https://blog.4c43.work/index.php/archives/47/)