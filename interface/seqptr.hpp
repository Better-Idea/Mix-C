#ifndef xpack_interface_private_seqptr_h
#define xpack_interface_private_seqptr_h
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_private_seqptr::inc
#include"docker/private/adapter.array_access.hpp"
#include"interface/initializer_list.hpp"
#include"math/index_system.hpp"
#include"meta/is_initializer_listx.hpp"
#include"meta/is_origin_arrayx.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_private_seqptr{
    template<class seq_t, class item_t>
    concept has_ptr_length_pairx = requires(seq_t seq, item_t * ptr, uxx length){
        ptr     = (static_cast<item_t *>(seq)); // 适应一些使用 explict operator 的类
        length  = (seq.length());
    };

    template<class seq_t, class item_t>
    concept can_seqptrlizex = 
        inc::is_initializer_listx<seq_t, item_t> or
        inc::is_origin_arrayx<seq_t, item_t> or 
        has_ptr_length_pairx<seq_t, item_t>;

    template<class seq_t>
    concept can_seqptrlize = 
        can_seqptrlizex<seq_t, inc::item_origin_of<seq_t>> == true;

    template<class item_type>
    xstruct(
        xtmpl(seqptr, item_type),
        xprif(ptr,  item_type *),
        xprif(len,  uxx)
    )
    public:
        using item_t            = item_type;
        using final_t           = inc::adapter_array_access<the_t>;
    public:
        constexpr seqptr(the_t const & self) : 
            ptr(self.ptr), len(self.len){}

        constexpr seqptr(item_t const * ptr, uxx len) : 
            ptr((item_t *)ptr), len(len){}

        constexpr seqptr(inc::initializer_list<item_t> const & seq) :
            seqptr(seq.begin(), seq.size()){
        }

        template<class seq_t>
        requires(inc::is_origin_arrayx<seq_t, item_t>)
        constexpr seqptr(seq_t const & list) : 
            seqptr(list, sizeof(seq_t) / sizeof(list[0])){
        }

        template<class seq_t>
        requires(has_ptr_length_pairx<seq_t, item_t>)
        constexpr seqptr(seq_t const & list) : 
            seqptr((item_t const *)list, list.length()){
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

        final_t backward(uxx offset) const {
            return the_t(ptr + offset, len - offset);
        }

        final_t forward(uxx offset) const {
            return the_t(ptr - offset, len + offset);
        }

        xpubgetx(length, uxx){
            return len;
        }

        xpubgetx(is_support_logic_inverse, bool){
            return false;
        }

    public:
        template<can_interval interval_t>
        final_t subseq(interval_t const & i) const {
            the_t r = the;
            i.normalize(the.length());

            return the_t(ptr + i.left(), i.right() - i.left() + 1);
        }
    $
}

namespace mixc::interface_private_seqptr::origin{
    using mixc::interface_private_seqptr::can_seqptrlize;
    using mixc::interface_private_seqptr::can_seqptrlizex;

    template<class item_t>
    using seqptr = inc::adapter_array_access<
        mixc::interface_private_seqptr::seqptr<item_t>
    >;
}

#define xseqptr(...)                                                                        \
template<can_interval interval_t>                                                           \
::mixc::interface_private_seqptr::origin                                                    \
::seqptr<__VA_ARGS__> subseq(interval_t const & i) const {                                  \
    return ::mixc::interface_private_seqptr::origin::seqptr<__VA_ARGS__>(*this).subseq(i);  \
}

#endif

xexport_space(mixc::interface_private_seqptr::origin)