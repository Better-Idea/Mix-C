#ifndef xpack_mixc_extern_simulation_logic
#define xpack_mixc_extern_simulation_logic
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::elogic
#include"define/base_type.hpp"
#include"docker/queue.hpp"
#include"macro/private/xlist.hpp"
#include"memop/signature.hpp"
#include"memop/swap.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::elogic::origin{
    enum estate_t{
        L, H, Z, X,
    };

    // 静态内存分配，将所有门阵列的输入状态汇集到该数组中
    // 通过 estate_array::alloc 分配的句柄访问
    xstruct(
        xname(estate_array)
    )
    public:
        enum{
            width    = 2,
            capacity = 1024 * 1024,
        };

        u08 bits[capacity * width / 8] = { 0 };
        uxx length = 0;

    public:
        uxx alloc(uxx bits){
            uxx offset = length;
            length    += bits;
            return offset;
        }

        void set(uxx offset, estate_t value){
            u08 i    = (offset & 0x3) * width;
            u08 mask = (0x3 << i);
            u08 v    = (u08(value) << i);
            bits[offset >> 2] &= ~mask;
            bits[offset >> 2] |= v;
        }

        estate_t get(uxx offset){
            u08 i    = (offset & 0x3) * width;
            u08 v    = (bits[offset >> 2] >> i) & 0x3;
            return estate_t(v);
        }
    $

    using einvoke_get_t  = estate_t(*)(voidp);
    using einvoke_set_t  = void(*)(voidp, estate_t);
    using einvoke_bdc_t  = void(*)(voidp);
    using invoke_index_t = u16;

    xstruct(
        xname(einvoke_group)
    )
        einvoke_get_t get;
        einvoke_set_t set;
        einvoke_bdc_t bdc;
    $

    xstruct(
        xname(einvoke_list)
    )
    private:
        enum{ capacity = 4096 };
        einvoke_group   items[capacity] = {0};
        invoke_index_t  length = 0;
    public:
        invoke_index_t registration(voidp get, voidp set, voidp broadcast){
            invoke_index_t i = length++;
            items[i].get     = einvoke_get_t(get);
            items[i].set     = einvoke_set_t(set);
            items[i].bdc     = einvoke_bdc_t(broadcast);
            return i;
        }

        einvoke_group & operator[](uxx index){
            return items[index];
        }
    $

    static inline estate_array img;
    static inline einvoke_list ivk;

    xstruct(
        xname(enext_branch)
    )
        voidp           object          = nullptr;
        invoke_index_t  index_invoke    = 0;

        void broadcast();
        void set(estate_t value){
            ivk[index_invoke].set(object, value);
        }

        estate_t get(){
            return ivk[index_invoke].get(object);
        }
    $

    template<uxx ocnt>
    xstruct(
        xtmpl(template_dat, ocnt)
    )
        u32             ibase    : 30   = 0;
        u32             in_queue : 1    = 0;
        enext_branch    obase[ocnt];
    $

    using typical_dat   = template_dat<1>;
    using typical_datp  = typical_dat *;
    using enext_branchp = enext_branch *;

    inline void enext_branch::broadcast(){
        typical_datp(object)->in_queue = false;
        ivk[index_invoke].bdc(object);
    }

    static inline inc::queue<enext_branch> que_active;
    static inline inc::queue<enext_branch> que_pile;

    inline void append(enext_branch node){
        if (typical_datp(node.object)->in_queue == false){
            typical_datp(node.object)->in_queue = true;
            que_pile.push(node);
        }
    }

    inline void tick_run(){
        inc::swap(xref que_active, xref que_pile);
        while(not que_active.is_empty()){
            que_active.pop().broadcast();
        }
    }

    template<class the_t, uxx index_signal>
    struct template_ipt{
        using im = template_ipt<the_t, index_signal>;
        friend the_t;

        template<class source_t>
        void operator= (source_t & source){
            source.bind(this);
        }

        void operator= (estate_t value){
            set(value);
        }

        operator estate_t(){
            return get();
        }

    private:
        estate_t get(){
            return img.get(typical_datp(this)->ibase + index_signal);
        }

        void set(estate_t value){
            if (auto i = typical_datp(this)->ibase + index_signal; img.get(i) != value){
                img.set(i, value);
                append(enext_branch{this, index_invoke});
            }
        }

        void broadcast(){
            the.broadcast(index_signal);
        }
    public:
        static inline auto index_invoke = ivk.registration(
            inc::signature<estate_t()>::check(& im::get),
            inc::signature<void(estate_t)>::check(& im::set),
            inc::signature<void()>::check(& im::broadcast)
        );
    };

    template<class the_t, uxx index_signal>
    struct template_opt{
        operator estate_t(){
            auto & opt       = typical_datp(this)->obase[index_signal];
            return opt.get();
        }
    private:
        template<class, uxx> 
        friend struct template_ipt;
        friend the_t;

        template<class port_t>
        void bind(port_t * port){
            auto & opt       = typical_datp(this)->obase[index_signal];
            opt.index_invoke = port_t::index_invoke;
            opt.object       = port;
        }

        void operator= (estate_t value){
            auto & opt       = typical_datp(this)->obase[index_signal];
            opt.set(value);
        }
    };

    #define __ipt__
    #define __ipt_start__           private: enum{ __ipt_start = __COUNTER__ + 1 }; public:
    #define __ipt_cnt__             private: enum{ __ipt_cnt = __COUNTER__ - __ipt_start };
    #define __ipt_opt__(...)
    #define __ipt_ipt__(name)       template_ipt<the_t, __COUNTER__ - __ipt_start> name;

    #define __opt__
    #define __opt_start__           private: enum{ __opt_start = __COUNTER__ + 1 }; public:
    #define __opt_cnt__             private: enum{ __opt_cnt = __COUNTER__ - __opt_start };
    #define __opt_ipt__(...)
    #define __opt_opt__(name)       template_opt<the_t, __COUNTER__ - __opt_start> name;

    #define xipt(...)               ipt__(__VA_ARGS__)
    #define xopt(...)               opt__(__VA_ARGS__)
    #define xgate(name,...)                                 \
    union name{                                             \
        using the_t = name;                                 \
        __xlist__(__ipt_, start__, __VA_ARGS__, cnt__)      \
        __xlist__(__opt_, start__, __VA_ARGS__, cnt__)      \
    public:                                                 \
        name(){                                             \
            dat.ibase = img.alloc(__ipt_cnt);               \
        }                                                   \
        void broadcast(uxx index_signal);                   \
    private:                                                \
        template_dat<__opt_cnt> dat;                        \
    }; inline void name::broadcast(uxx index_signal)

    /* 常用的逻辑门预定义
     * - enand  与非门
     * - eand   与门
     * - enor   或非门
     * - eor    或门
     * - enot   非门
     */

    xgate(enand, xipt(a), xipt(b), xopt(o)){
        if (a == L or b == L){
            o = H;
        }
        else if (a == H and b == H){
            o = L;
        }
        else if (a == Z and b == Z){
            o = Z;
        }
        else{
            o = X;
        }
    }

    xgate(eand, xipt(a), xipt(b), xopt(o)){
        if (a == L or b == L){
            o = L;
        }
        else if (a == H and b == H){
            o = H;
        }
        else if (a == Z and b == Z){
            o = Z;
        }
        else{
            o = X;
        }
    }

    xgate(enor, xipt(a), xipt(b), xopt(o)){
        if (a == L and b == L){
            o = H;
        }
        else if (a == H or b == H){
            o = L;
        }
        else if (a == Z and b == Z){
            o = Z;
        }
        else{
            o = X;
        }
    }

    xgate(eor, xipt(a), xipt(b), xopt(o)){
        if (a == L and b == L){
            o = L;
        }
        else if (a == H or b == H){
            o = H;
        }
        else if (a == Z and b == Z){
            o = Z;
        }
        else{
            o = X;
        }
    }

    xgate(enot, xipt(a), xopt(o)){
        if (a == L){
            o = H;
        }
        else if (a == H){
            o = L;
        }
        else{
            o = a;
        }
    }

    /*
    nand n0, n1;
    n0.a = n1.o;
    n1.a = n0.o;

    n1.o    -> output_type
    map opt -> {ipt...}
    */


}

#endif

namespace xuser::inc{
    using namespace ::mixc::elogic::origin;
}
