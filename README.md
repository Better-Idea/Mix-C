# Mix-C
**创立该项目的初衷是想加深对 C++ 的理解，并思考标准库是否可以实现的更好。**
**一个人的力量是有限和渺小的，我需要能一起并肩作战的伙伴**

该项目有诸多亮点，包括但不限于以下：
-	内存管理算法，让 1KB 以下的小内存的分配与回收效率有超过三倍的提升，性能超越 google 的 tcmalloc，并有效的减少了内存碎片（该部分遗留在旧代码部分，等待迁移）。旧版的还是用中文写的哦！等待改邪归正。  
https://github.com/Better-Idea/Mix-C/blob/master/old/%E5%9F%BA%E6%9C%AC%E5%AE%9A%E4%B9%89/%E8%BF%B7%E4%BD%A0.cpp

-	半伪随机数生成算法：一款高质量的伪随机数生成算法，速度比线性同余更快，周期比 mt19937更长（理论上没有周期）  
https://github.com/Better-Idea/Mix-C/blob/master/math/random.hpp

-	包含范式：给你一个干净的命名空间，一言难尽，看代码吧 。  
https://github.com/Better-Idea/Mix-C/tree/master/lang

-	SIMD基础字符串算法库：C 接口，x86-AVX2 指令集，在回家的火车上闲着无聊写的，目前只有几个小函数。  
https://github.com/Better-Idea/Mix-C/tree/master/simd/x86

-	内存管理单元映射算法：该算法提供两种映射模式，  
addressing::var 模式适合构建动态增长型的内存。对于类似 512B 512B 1KB 2KB ... 这样堆叠的内存，该算法可以通过简单的位运算，能在 O(1) 时间复杂度下访问堆叠内存中的任意字节，用这个算法实现 vector 动态增长时无需释放原有的内存块，只需在页表增加一块和当前容量一样大的内存就可以了，只是这种非硬件组织的结构随机访问性能要差一点。  
addressing::fixed 模式适合构建固定大小的内存。相比原始的直接分配一块连续的内存，该算法显得更灵活，因为它只需要少许的页表就能构成一个较大的内存块。例如我们需要234KB 的内存，那只需要 128KB 64KB 32KB 8KB 2KB的页各一块就够了，该算法也能在 O(1) 时间内帮我们完成逻辑地址到物理地址的转换。  
https://github.com/Better-Idea/Mix-C/blob/master/algo/mmu.hpp

-	现代 GC：该算法是图论和引用计数器结合的产物， 能有效的解决环形引用并代价低廉，其原理简单但结构复杂， 完整的部分涉及内存分配器 + 线程 + 智能指针 + 模板元函数式编程。  
https://github.com/Better-Idea/Mix-C/tree/master/gc

