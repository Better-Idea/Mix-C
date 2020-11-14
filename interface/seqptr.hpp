#ifndef xpack_interface_seqptr
#define xpack_interface_seqptr
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_seqptr::inc
#include"interface/private/check.hpp"
#include"interface/initializer_list.hpp"
#include"interface/ranger.hpp"
#include"macro/xnew.hpp"
#include"math/index_system.hpp"
#include"meta/is_initializer_list.hpp"
#include"meta/is_origin_array.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

#define xseqptr(...)                                                                        \
::mixc::interface_seqptr::origin::seqptr<__VA_ARGS__> subseq(::mixc::iinterval i) const {   \
    using ptr_t  = __VA_ARGS__ *;                                                           \
    using ptrc_t = __VA_ARGS__ const *;                                                     \
    auto  len    = the.length();                                                            \
    auto  ptr    = (ptr_t)(ptrc_t)this[0];                                                  \
    i.normalize(len);                                                                       \
    return ::mixc::interface_seqptr::origin::seqptr<__VA_ARGS__>(                           \
        ptr + i.left(),                                                                     \
        i.right() - i.left() + 1                                                            \
    );                                                                                      \
}                                                                                           \
xranger(__VA_ARGS__)

namespace mixc::interface_seqptr::origin{
    template<class seq_t, class item_t>
    concept can_seqptrlizex = 
        (inc::check_initializer_listx<seq_t, item_t> and inc::is_initializer_list<seq_t>) or
        (inc::check_arrayx<seq_t, item_t> and bool(inc::is_origin_array<seq_t> ^ inc::check_length<seq_t>));

    template<class seq_t>
    concept can_seqptrlize = 
        (inc::check_initializer_list<seq_t> and inc::is_initializer_list<seq_t>) or
        (inc::check_array<seq_t> and bool(inc::check_length<seq_t>));

    template<class item_t> struct seqptr;
    template<class item_t>
    xstruct(
        xtmpl(seqptr, item_t)
    )
    private:
        item_t * ptr = nullptr;
        uxx      len = 0;
    public:
        seqptr(){}

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

        operator item_t *(){
            return ptr;
        }

        operator item_t const *() const{
            return ptr;
        }

        the_t backward(uxx offset) const {
            return the_t(ptr + offset, len - offset);
        }

        the_t forward(uxx offset) const {
            return the_t(ptr - offset, len + offset);
        }

        uxx length() const {
            return len;
        }

        xseqptr(item_t); 

    private:
        template<auto mode, class iterator_t>
        void foreach_template(iterator_t const & invoke, inc::iinterval itv = co{0, -1}){
            auto r      = subseq(itv);
            auto state  = loop_t::go_on;

            for(uxx i = 0; i < r.length() and state != loop_t::finish; i++){
                xitr_switch(mode,i,state,invoke, r[i]);
            }
        }
    public:
        xitr_foreach (item_t &)
        xitr_foreachx(item_t &)
        xitr_foreach_const (item_t const &)
        xitr_foreachx_const(item_t const &)
    $
}

#endif

xexport_space(mixc::interface_seqptr::origin)