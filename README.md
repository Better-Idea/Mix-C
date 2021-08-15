# Mix-C
**创立该项目的初衷是想加深对 C++ 的理解，并思考标准库是否可以实现的更好。**
**一个人的力量是有限和渺小的，我需要能一起并肩作战的伙伴**

## 推荐点
该项目有诸多亮点，包括但不限于以下：
- 内存管理算法，让 4KB 以下的小内存的分配与回收效率有超过三倍的提升，性能超越 google 的 tcmalloc，并有效的减少了内存碎片。该算法已从 old/ 旧代码中迁移完毕并增强了原有功能。  
https://github.com/Better-Idea/Mix-C/blob/master/utils/private/memory_flow.hpp

- 策略锁：功能比读写锁更完整、更通用化。同步开销和自旋锁相当，可配置且能有效的管理线程并发和同步，同时避免了传统读写锁可能出现的线程饥饿问题。  
https://github.com/Better-Idea/Mix-C/blob/master/concurrency/lock/policy_barrier.hpp  
https://github.com/Better-Idea/Mix-C/blob/master/discuss/talk_about_policy_barrier.cpp

- 半伪随机数生成算法：一款高质量的伪随机数生成算法，速度比线性同余更快，周期比 mt19937更长（理论上没有周期）  
https://github.com/Better-Idea/Mix-C/blob/master/math/random.hpp  
https://github.com/Better-Idea/Mix-C/blob/master/discuss/talk_about_random.cpp

- 包含范式：给你一个干净的命名空间，一言难尽，看代码吧 。  
https://github.com/Better-Idea/Mix-C/tree/master/lang

- SIMD基础字符串算法库：C 接口，x86-AVX2 指令集，在回家的火车上闲着无聊写的，目前只有几个小函数。  
https://github.com/Better-Idea/Mix-C/tree/master/simd/x86

- 内存管理单元映射算法：该算法提供两种映射模式。  
addressing::var 模式适合构建动态增长型的内存。对于类似 512B 512B 1KB 2KB ... 这样堆叠的内存，该算法可以通过简单的位运算，能在 O(1) 时间复杂度下访问堆叠内存中的任意字节，用这个算法实现 vector 动态增长时无需释放原有的内存块，只需在页表增加一块和当前容量一样大的内存就可以了，只是这种非硬件组织的结构随机访问性能要差一点。  
addressing::fixed 模式适合构建固定大小的内存。相比原始的直接分配一块连续的内存，该算法显得更灵活，因为它只需要少许的页表就能构成一个较大的内存块。例如我们需要 234KB 的内存，那只需要 128KB 64KB 32KB 8KB 2KB的页各一块就够了，该算法也能在 O(1) 时间内帮我们完成逻辑地址到物理地址的转换。  
https://github.com/Better-Idea/Mix-C/blob/master/algo/mmu.hpp

- 现代 GC：该算法是图论和引用计数器结合的产物， 能有效的解决环形引用并代价低廉，其原理简单但结构复杂， 完整的部分涉及内存分配器 + 线程 + 智能指针 + 模板元函数式编程。  
https://github.com/Better-Idea/Mix-C/tree/master/gc

- 数学库：运用微分思想提供高效、独特的初等函数库  
https://github.com/Better-Idea/Mix-C/blob/master/math

## 目录说明
| Dir                   | Description                                                            |
|-----------------------|------------------------------------------------------------------------|
| algo                  | 通用算法类，多以函数单体形式提供                                       |
| concurrency           | 为并发提供支持库，包含原子操作、线程接口                               |
| chrono                | 与计时相关的库，以对象形式提供                                         |
| define                | 提供基础类型的定义，以及类似 nan 非数与 nullref 空引用字面量语法糖     |
| discuss               | 一个专门讨论模块可用性的目录，提供示例以供参考                         |
| docker                | 容器类专属目录，包含静态数组，动态数组，哈希表等常用数据结构           |
| draft                 | 草稿与点子                                                             |
| dumb                  | 本身无实际作用，仅用于修饰、包装使用者的类集                           |
| extern                | Mix-C 库的具体应用实例                                                 |
| gc                    | 基于图论 + 模板元的现代 GC 库                                          |
| instruction           | 与 cpu 相关的基础指令                                                  |
| interface             | 通用接口类，目前只限于作为函数参数                                     |
| io                    | 对控制台、文件系统等输入输出类的包装                                   |
| lang                  | 底层的原始字符处理模块，包含字符以及字符串的基本处理                   |
| macro                 | 为构成的模块提供简便写法，用于简化代码、实现奇特的效果                 |
| math                  | 提供初等数学以及概率统计相关的函数                                     |
| memop                 | 直接操作内存的库                                                       |
| meta                  | 为元编程提供元信息                                                     |
| meta_seq              | 为编译期的类型数组与常量数组提供基础操作                               |
| simd                  | 与 cpu 相关的矢量运算库，用于发掘 cpu 的性能                           |
| test                  | 测试单元                                                               |
| ttyctrl               | 为控制台打造的控件库                                                   |
| utils                 | 初始化列表、底层内存分配模块、位图指示器、等等小工具                   |

## 框架指引
- [运作观](https://github.com/Better-Idea/Mix-C/tree/master/doc_guide/point_of_view.md)
- [包含范式](https://github.com/Better-Idea/Mix-C/tree/master/doc_guide/inclusion_paradigm.md)

TO BE CONTINUE
