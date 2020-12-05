#ifndef xpack_interface_private_seqptr_h
#define xpack_interface_private_seqptr_h
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_private_seqptr::inc
#include"interface/private/check.hpp"
#include"interface/private/ranger.hpp"
#include"interface/initializer_list.hpp"
#include"macro/xnew.hpp"
#include"math/index_system.hpp"
#include"meta/is_initializer_list.hpp"
#include"meta/is_origin_array.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_adapter_array_access {
    template<class base_t> struct adapter_array_access;
}

namespace mixc::interface_private_seqptr{
    template<class seq_t, class item_t>
    concept can_seqptrlizex = 
        (inc::check_initializer_listx<seq_t, item_t> and inc::is_initializer_list<seq_t>) or
        (inc::check_arrayx<seq_t, item_t> and bool(inc::is_origin_array<seq_t> ^ inc::check_length<seq_t>));

    template<class seq_t>
    concept can_seqptrlize = 
        (inc::check_initializer_list<seq_t> and inc::is_initializer_list<seq_t>) or
        (inc::check_array<seq_t> and bool(inc::is_origin_array<seq_t> ^ inc::check_length<seq_t>));

    template<class item_type>
    xstruct(
        xtmpl(seqptr, item_type)
    )
    public:
        using item_t            = item_type;
        using final             = the_t;
    private:
        mutable item_t *    ptr = nullptr;
        mutable uxx         len = 0;
    public:
        seqptr(the_t const &) = default;
        seqptr(item_t const * ptr, uxx len) : 
            ptr((item_t *)ptr), len(len){}

        template<class seq_t>
        requires(can_seqptrlizex<seq_t, item_t>)
        seqptr(seq_t const & list){
            if constexpr (inc::check_initializer_list<seq_t>){
                xnew(this) seqptr(list.begin(), list.size());
            }
            else if constexpr (inc::check_length<seq_t>){
                xnew(this) seqptr(list, list.length());
            }
            else{
                xnew(this) seqptr(list, sizeof(seq_t) / sizeof(list[0]));
            }
        }

        seqptr(inc::initializer_list<item_t> const & list){ // 使用显式的 initializer_list
            xnew(this) seqptr(list.begin(), list.size());
        }

        operator item_t *(){
            return ptr;
        }

        operator item_t const *() const{
            return ptr;
        }

        item_t & operator[](uxx index){
            return ptr[index];
        }

        item_t const & operator [](uxx index) const {
            return ptr[index];
        }

        final backward(uxx offset) const {
            return the_t(ptr + offset, len - offset);
        }

        final forward(uxx offset) const {
            return the_t(ptr - offset, len + offset);
        }

        xpubgetx(length, uxx){
            return len;
        }
    $
}

namespace mixc::interface_private_seqptr::origin{
    using mixc::interface_private_seqptr::can_seqptrlize;
    using mixc::interface_private_seqptr::can_seqptrlizex;

    template<class item_t>
    using seqptr = mixc::docker_adapter_array_access::adapter_array_access<
        mixc::interface_private_seqptr::seqptr<item_t>
    >;
}

#define xseqptr(...)                                                                        \
template<can_interval interval_t>                                                           \
::mixc::interface_private_seqptr::origin                                                    \
::seqptr<__VA_ARGS__> subseq(interval_t const & i) const {                                  \
    using ptr_t  = __VA_ARGS__ *;                                                           \
    using ptrc_t = __VA_ARGS__ const *;                                                     \
    auto  len    = the.length();                                                            \
    auto  ptr    = (ptr_t)(ptrc_t)this[0];                                                  \
    i.normalize(len);                                                                       \
    return ::mixc::interface_private_seqptr::origin::seqptr<__VA_ARGS__>(                   \
        ptr + i.left(), i.right() - i.left() + 1                                            \
    );                                                                                      \
}                                                                                           \
xranger(__VA_ARGS__)

#endif

xexport_space(mixc::interface_private_seqptr::origin)