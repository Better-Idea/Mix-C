#ifndef xpack_interface_ranger
#define xpack_interface_ranger
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_ranger::inc
#include"interface/private/check.hpp"
#include"interface/initializer_list.hpp"
#include"interface/iterator.hpp"
#include"macro/xalign.hpp"
#include"macro/xnew.hpp"
#include"math/index_system.hpp"
#include"memop/signature.hpp"
#include"meta/is_initializer_list.hpp"
#include"meta/is_origin_array.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser") 

namespace mixc::interface_ranger{
    using namespace inc;

    template<class object>
    static inline xalign(sizeof(voidp) * 2) voidp itrs[2];

    xstruct(
        xname(base)
    )
    private:
        enum{ mask = sizeof(uxx) * 2 - 1 };

        template<class object, class return_type>
        return_type pos(uxx index){
            return (*(object *)ptr)[ofs + index];
        }

        template<class object, class return_type>
        return_type neg(uxx index){
            return (*(object *)ptr)[ofs - index];
        }

        template<class return_type>
        return_type & posx(uxx index){
            return ((return_type *)ptr)[ofs + index];
        }

        template<class return_type>
        return_type & negx(uxx index){
            return ((return_type *)ptr)[ofs - index];
        }

    protected:
        mutable voidp    ptr = nullptr;
        mutable voidp *  itr = nullptr;
        mutable uxx      len = 0;
        mutable uxx      ofs = 0;

        void turn_positive_order() {
            itr = (voidp *)(uxx(itr) & ~mask);
        }

        void turn_negtive_order() {
            itr = (voidp *)(uxx(itr) | sizeof(uxx));
        }

        template<class item_t>
        item_t access(uxx index) const {
            return inc::signature<item_t(uxx)>::call(this, itr[0], index);
        }
    public:
        base(){}

        template<class object>
        base(object const * ptr, uxx len, uxx ofs):
            ptr((object *)ptr),
            itr(itrs<object>),
            len(len),
            ofs(ofs){
            using ret_type = decltype((*(object *)ptr)[uxx(0)]);
            itr[0] = inc::signature<ret_type(uxx)>::check(& base::pos<object, ret_type>);
            itr[1] = inc::signature<ret_type(uxx)>::check(& base::neg<object, ret_type>);
        }

        template<class item_t>
        base(inc::initializer_list<item_t> const * ptr, uxx len, uxx ofs):
            ptr((item_t *)ptr->begin()),
            itr(itrs<item_t>),
            len(len),
            ofs(ofs){
            itr[0] = inc::signature<item_t & (uxx)>::check(& base::posx<item_t>);
            itr[1] = inc::signature<item_t & (uxx)>::check(& base::negx<item_t>);
        }

        template<class object>
        base(object const * ptr, uxx len, uxx ofs, bool negtive_order): 
            base(ptr, len, ofs){
            turn_negtive_order();
        }

        bool is_positive_order() const {
            return (uxx(itr) & mask) == 0;
        }

        bool is_negtive_order() const {
            return (uxx(itr) & mask) != 0;
        }

        uxx length() const {
            return len;
        }
    $
}

namespace mixc::interface_ranger::origin{
    template<class seq_t, class item_t>
    concept can_rangerlizex = 
        (inc::check_initializer_listx<seq_t, item_t> and inc::is_initializer_list<seq_t>) or
        (inc::check_arrayx<seq_t, item_t> and inc::is_origin_array<seq_t>) or
        (inc::check_indexablex<seq_t, item_t> and bool(inc::is_origin_array<seq_t> ^ inc::check_length<seq_t>));

    template<class seq_t>
    concept can_rangerlize = 
        (inc::check_initializer_list<seq_t> and inc::is_initializer_list<seq_t>) or
        (inc::check_array<seq_t> and inc::is_origin_array<seq_t>) or
        (inc::check_indexable<seq_t> and bool(inc::is_origin_array<seq_t> ^ inc::check_length<seq_t>));

    template<class item_t>
    xstruct(
        xtmpl(ranger, item_t),
        xpubb(base)
    )
    public:
        ranger(){}
        ranger(base impl) : 
            base(impl){}

        ranger(item_t const * items, uxx length) : 
            base((item_t *)items, length, 0){
        }

        template<class seq_t>
        requires(can_rangerlizex<seq_t, item_t>)
        ranger(seq_t const & list){
            if constexpr (inc::check_initializer_list<seq_t>){
                xnew(this) ranger(list.begin(), list.size());
            }
            else if constexpr (inc::check_length<seq_t>){
                xnew(this) ranger(list, list.length());
            }
            else{
                xnew(this) ranger(list, sizeof(seq_t) / sizeof(list[0]));
            }
        }

        item_t & operator[](uxx index){
            return access<item_t &>(index);
        }

        item_t & operator[](uxx index) const {
            return access<item_t &>(index);
        }

        the_t backward(uxx offset) const {
            if (the_t r = the; is_positive_order()){
                r.ofs += offset;
                r.len -= offset;
                return r;
            }
            else{
                r.ofs -= offset;
                r.len -= offset;
                return r;
            }
        }

        the_t forward(uxx offset) const {
            return backward(uxx(0) - offset);
        }

        ranger<item_t> subseq(iinterval i){
            ranger<item_t> r = the;
            i.normalize(the.length());

            if (is_positive_order()){ // 正序
                if (i.left() <= i.right()){ // 正序
                    r.ofs += i.left();
                    r.len  = i.right() - i.left() + 1;
                }
                else{ // 反序
                    r.ofs += i.left();
                    r.len  = i.left() - i.right() + 1;
                    r.turn_negtive_order();
                }
            }
            else{ // 反序
                if (i.left() <= i.right()){ // 反序 & 正序 -> 反序
                    r.ofs -= i.left();
                    r.len  = i.right() - i.left() + 1;
                }
                else{ // 反序 & 反序 -> 正序
                    r.ofs -= i.left();
                    r.len  = i.left() - i.right() + 1;
                    r.turn_positive_order();
                }
            }
            return r;
        }

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

    #define xranger(...)                                                                        \
    ::mixc::interface_ranger::origin::ranger<__VA_ARGS__> range(::mixc::iinterval i) const {    \
        using namespace ::mixc::interface_ranger;                                               \
        if (i.normalize(this->length());                                                        \
            i.left() <= i.right()){                                                             \
            return base(this, i.right() - i.left() + 1, i.left());                              \
        }                                                                                       \
        else{                                                                                   \
            return base(this, i.left() - i.right() + 1, i.left(), true);                        \
        }                                                                                       \
    }
}

#endif

xexport_space(mixc::interface_ranger::origin)