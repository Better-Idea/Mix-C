#ifndef xpack_interface_seqptr
#define xpack_interface_seqptr
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_seqptr::inc
#include"interface/initializer_list.hpp"
#include"interface/ranger.hpp"
#include"math/index_system.hpp"
#include"memop/signature.hpp"
#include"meta/is_same.hpp"
#include"meta/item_origin_of.hpp"
#include"meta/is_origin_array.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

#define xseqptr(...)                                                                \
::mixc::interface_seqptr::seqptr<__VA_ARGS__> subseq(::mixc::iinterval i) const {   \
    using ptr_t  = __VA_ARGS__ *;                                                   \
    using ptrc_t = __VA_ARGS__ const *;                                             \
    auto  len    = the.length();                                                    \
    auto  ptr    = (ptr_t)(ptrc_t)this[0];                                          \
    i.normalize(len);                                                               \
    return ::mixc::interface_seqptr::seqptr<__VA_ARGS__>(                           \
        ptr + i.left(),                                                             \
        i.right() - i.left() + 1                                                    \
    );                                                                              \
}                                                                                   \
xranger(__VA_ARGS__)

namespace mixc::interface_seqptr{
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

        seqptr(item_t * ptr, uxx len) : 
            ptr(ptr), len(len){}

        template<class type>
        requires(
            inc::is_origin_array<type> and
            inc::is_same<item_t, inc::item_origin_of<type>>
        )
        seqptr(type const & list) :
            seqptr((item_t *)list, sizeof(type) / sizeof(item_t)){
        }

        template<class object> requires(
            inc::signature<uxx()>::has(& object::length) and
            (
                inc::signature<item_t const *()>::has(& object::operator item_t const *) or
                inc::signature<item_t *()>::has(& object::operator item_t *)
            )
        )
        seqptr(object const & impl){
            len = (uxx)impl.length();
            ptr = (item_t *)(item_t const *)impl;
        }

        seqptr(inc::initializer_list<item_t> list){
            len = (uxx)list.size();
            ptr = (item_t *)list.begin();
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

xexport(mixc::interface_seqptr::seqptr)