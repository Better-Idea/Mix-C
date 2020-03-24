/*
在 GC 组件中，routing_result 结构体作为 GC 递归 routing 时的栈上变量
我们压缩了该结构的体积以减少栈内存的消耗
Q : 用 u32 定义该结构能在 64 位系统上节省栈空间吗？
A : 一般是不能的，大多数 cpu 体系结构会按通用寄存器位宽压栈，在 64bit 系统上仍是按 64bit 压栈
*/

#ifndef xpack_gc_routing_result
#define xpack_gc_routing_result
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::gc_routing_result
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::gc_routing_result{
        struct routing_result{
            // 该字段指示在递归 routing 的过程中，经过该节点的路径是否可以再次回到根节点（构成环）
            uxx     can_arrive_root : 1;

            // 重点：============================================================================
            // 该字段指示在一个有向图中，出度和入度的差值
            // 概念：在闭合有向图中，出度和入度的差值总是等于 0
            //       我们定义一个节点的引用数为入度，能抵达根节点的子成员数为该节点出度
            // 由以上概念可知，出度总和是不会大于入度总和的，当一个节点存在外部引用时，
            // 会使得出度总和小于入度总和，此时我们可以通过计算该闭合有向图的出入度差是否为 0
            // 来决定是否可以释放根节点
            ixx     degree_dvalue   : sizeof(ixx) * 8 - 1;

            routing_result() : routing_result(0){}
            routing_result(uxx can_arrive_root, ixx degree_dvalue = 0) : 
                can_arrive_root(can_arrive_root), 
                degree_dvalue(degree_dvalue) {}
            
            void operator += (routing_result value){
                // can_arrive_root 是具有传递性的。如果一个节点的任意一个子成员能抵达根节点，
                // 那么该节点和所有指向该节点的节点都能抵达根节点
                can_arrive_root |= value.can_arrive_root;

                // 计算出入度差的总和
                degree_dvalue += value.degree_dvalue;
            }
        };
    }

#endif

namespace xuser::inc{
    using mixc::gc_routing_result::routing_result;
}
