#ifndef xpack_interface_private_ranger
#define xpack_interface_private_ranger
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_private_ranger::inc
#include"define/base_type.hpp"
#include"docker/private/adapter_array_access.hpp"
#include"interface/initializer_list.hpp"
#include"interface/seqptr.hpp"
#include"macro/xalign.hpp"
#include"macro/xexport.hpp"
#include"macro/xref.hpp"
#include"macro/xstruct.hpp"
#include"math/index_system.hpp"
#include"meta/is_origin_array.hpp"
#include"meta/is_origin_arrayx.hpp"
#include"meta/is_origin_string.hpp"
#include"meta/is_origin_stringx.hpp"
#include"meta/is_same.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser") 

namespace mixc::interface_private_ranger{
    using namespace inc;

    template<class seq_t, class item_t>
    concept has_indexer_length_pairx = requires(seq_t seq, item_t * ptr, uxx length){
        // 为了兼容 msvc 16.8 以下版本：使用 seq.operator item_t & []() 会出现编译器内部错误
        ptr     = xref(seq[uxx(0)]);
        length  = seq.length();
    };

    template<class seq_t>
    concept has_indexer_length_pair = 
        has_indexer_length_pairx<seq_t, inc::item_origin_of<seq_t>>;

    template<class seq_t, class item_t>
    concept can_rangerlizex = 
        inc::is_origin_arrayx<seq_t, item_t> or has_indexer_length_pairx<seq_t, item_t>;

    template<class seq_t>
    concept can_rangerlize = 
        can_rangerlizex<seq_t, inc::item_origin_of<seq_t>>;

    template<class object_t>
    struct helper{
        // 为了兼容 msvc 16.8 以下版本：全局静态内联模板变量对齐无效的 bug，只能套个结构体曲线救国
        static inline xalign(sizeof(voidp) * 2) voidp itrs[2];
    };

    xstruct(
        xname(ranger_base),
        xprof(m_ptr, voidp  ),
        xprof(m_itr, voidp *),
        xprof(m_len, uxx    ),
        xprof(m_ofs, uxx    )
    )
    private:
        using final_t = the_t;
        enum{ mask  = sizeof(voidp) * 2 - 1 };

        template<class object, class return_type>
        static return_type & pos(ranger_base * this_ptr, uxx index){
            return (*(object *)this_ptr->m_ptr)[this_ptr->m_ofs + index];
        }

        template<class object, class return_type>
        static return_type & neg(ranger_base * this_ptr, uxx index){
            return (*(object *)this_ptr->m_ptr)[this_ptr->m_ofs - index];
        }

        template<class return_type>
        static return_type & posx(ranger_base * this_ptr, uxx index){
            return ((return_type *)this_ptr->m_ptr)[this_ptr->m_ofs + index];
        }

        template<class return_type>
        static return_type & negx(ranger_base * this_ptr, uxx index){
            return ((return_type *)this_ptr->m_ptr)[this_ptr->m_ofs - index];
        }

    protected:
        void turn_positive_order() {
            m_itr = (voidp *)(uxx(m_itr) & ~mask);
        }

        void turn_negtive_order() {
            m_itr = (voidp *)(uxx(m_itr) | sizeof(uxx));
        }

        template<class item_t>
        item_t & access(uxx index) const {
            using invoke_t = item_t &(*)(ranger_base const *, uxx);
            return invoke_t(*m_itr)(this, index);
        }

    private:
        template<class object_t>
        void init(object_t const * ptr, uxx len){
            this->m_ptr     = (object_t *)ptr;
            this->m_itr     = (voidp *)helper<object_t>::itrs;
            this->m_len     = (uxx)len;
            this->m_ofs     = (0);
        }

    public:
        template<class item_t>
        ranger_base(item_t * seq, uxx len){
            init(seq, len);
            m_itr[0]        = voidp(& posx<item_t>);
            m_itr[1]        = voidp(& negx<item_t>);
        }

        template<has_indexer_length_pair seq_t>
        ranger_base(seq_t const & seq){
            init(xref(seq), seq.length());
            using item_t    = inc::item_origin_of<seq_t>;
            m_itr[0]        = voidp(& pos<seq_t, item_t>);
            m_itr[1]        = voidp(& neg<seq_t, item_t>);
        }

        template<inc::is_origin_array seq_t>
        ranger_base(seq_t const & seq):
            ranger_base((seq_t &)seq, sizeof(seq) / sizeof(inc::item_origin_of<seq_t>)){
        }

        xpubgetx(is_positive_order, bool) {
            return (uxx(m_itr) & mask) == 0;
        }

        xpubgetx(is_negtive_order, bool) {
            return (uxx(m_itr) & mask) != 0;
        }

        xpubgetx(length, uxx) {
            return m_len;
        }

        xpubgetx(is_support_logic_inverse, bool){
            return true;
        }
    $

    template<class item_type>
    xstruct(
        xtmpl(ranger, item_type),
        xpubb(ranger_base)
    )
    public:
        using item_t    = item_type;
        using final_t   = inc::adapter_array_access<the_t>;

        template<inc::is_origin_array seq_t>
        requires(
            // 这里不使用 is_origin_array，因为对于原始数组可能会约束失败
            inc::is_same<
                inc::item_origin_of<seq_t>, item_t
            > and

            // 避免和字符串约束冲突
            inc::is_origin_stringx<seq_t, item_t> == false
        )
        // 这里不使用 is_origin_stringx，因为对于原始数组可能会约束失败
        constexpr ranger(seq_t const & ary) : 
            ranger(ary, sizeof(seq_t) / sizeof(ary[0])){}

        template<inc::is_origin_string seq_t>
        requires(
            // 这里不使用 is_origin_stringx，因为对于原始数组可能会约束失败
            inc::is_same<
                inc::item_origin_of<seq_t>, item_t
            >
        )
        constexpr ranger(seq_t const & list) : 
            ranger(list, sizeof(seq_t) / sizeof(list[0]) - 1){
        }

        constexpr ranger(the_t const &) = default;

        constexpr ranger(item_t const * ptr, uxx len) :
            ranger_base((item_t *)ptr, len){}

        constexpr ranger(inc::initializer_list<item_t> const & seq) :
            ranger(seq.begin(), seq.size()){
        }

        constexpr ranger(inc::seqptr<item_t> const & seq) :
            ranger(seq, seq.length()){
        }

        item_t & operator[](uxx index){
            return access<item_t>(index);
        }

        item_t & operator[](uxx index) const {
            return access<item_t>(index);
        }

        final_t backward(uxx offset) const {
            if (the_t r = the; is_positive_order()){
                r.m_ofs += offset;
                r.m_len -= offset;
                return r;
            }
            else{
                r.m_ofs -= offset;
                r.m_len -= offset;
                return r;
            }
        }

        final_t forward(uxx offset) const {
            return backward(uxx(0) - offset);
        }

        template<can_interval interval_t>
        final_t subseq(interval_t const & i) const {
            the_t r = the;
            i.normalize(the.length());

            if (is_positive_order()){ // 正序
                if (i.left() <= i.right()){ // 正序
                    r.m_ofs += i.left();
                    r.m_len  = i.right() - i.left() + 1;
                }
                else{ // 反序
                    r.m_ofs += i.left();
                    r.m_len  = i.left() - i.right() + 1;
                    r.turn_negtive_order();
                }
            }
            else{ // 反序
                if (i.left() <= i.right()){ // 反序 & 正序 -> 反序
                    r.m_ofs -= i.left();
                    r.m_len  = i.right() - i.left() + 1;
                }
                else{ // 反序 & 反序 -> 正序
                    r.m_ofs -= i.left();
                    r.m_len  = i.left() - i.right() + 1;
                    r.turn_positive_order();
                }
            }
            return r;
        }
    $
}

namespace mixc::interface_private_ranger::origin{
    using mixc::interface_private_ranger::can_rangerlize;
    using mixc::interface_private_ranger::can_rangerlizex;

    template<class item_t>
    using ranger = inc::adapter_array_access<
        mixc::interface_private_ranger::ranger<item_t>
    >;
}

#define xranger(...)                                        \
template<can_interval interval_t>                           \
::mixc::interface_private_ranger::origin                    \
::ranger<__VA_ARGS__> subseq(interval_t const & i) const {  \
    return                                                  \
        ::mixc::interface_private_ranger::origin            \
        ::ranger<__VA_ARGS__>(*this).subseq(i);             \
}

#endif

xexport_space(mixc::interface_private_ranger::origin)