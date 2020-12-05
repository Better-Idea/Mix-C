#ifndef xpack_interface_private_ranger
#define xpack_interface_private_ranger
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_private_ranger::inc
#include"interface/private/check.hpp"
#include"interface/initializer_list.hpp"
#include"interface/iterator.hpp"
#include"macro/xalign.hpp"
#include"macro/xnew.hpp"
#include"math/index_system.hpp"
#include"memop/signature.hpp"
#include"meta/is_initializer_list.hpp"
#include"meta/is_origin_array.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser") 

namespace mixc::docker_adapter_array_access {
    template<class base_t> struct adapter_array_access;
}

namespace mixc::interface_private_ranger{
    using namespace inc;

    template<class object>
    static inline xalign(sizeof(voidp) * 2) voidp itrs[2];

    xstruct(
        xname(base)
    )
    private:
        enum{ mask = sizeof(uxx) * 2 - 1 };

        template<class object, class return_type>
        return_type & pos(uxx index){
            return (*(object *)ptr)[ofs + index];
        }

        template<class object, class return_type>
        return_type * neg(uxx index){
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
        template<class seq_t>
        base(seq_t const & list) {
            using item_t = inc::item_origin_of<seq_t>;

            if constexpr (inc::check_initializer_list<seq_t>){
                xnew(this) base(list.begin(), list.size(), 0);
            }
            else if constexpr (inc::check_length<seq_t>){
                xnew(this) base(& list, list.length(), 0);
            }
            else{
                xnew(this) base(& list, sizeof(seq_t) / sizeof(list[0]), 0);
            }

            if constexpr (inc::check_length<seq_t>){ // is object
                itr[0] = inc::signature<item_t &(uxx)>::check(& base::pos<seq_t, item_t>);
                itr[1] = inc::signature<item_t &(uxx)>::check(& base::neg<seq_t, item_t>);
            }
            else{
                itr[0] = inc::signature<item_t &(uxx)>::check(& base::posx<item_t>);
                itr[1] = inc::signature<item_t &(uxx)>::check(& base::negx<item_t>);
            }
        }

        template<class object_t>
        base(object_t const * ptr, uxx len, uxx ofs, bool use_negtive_order = false): 
            ptr((object_t *)ptr),
            itr(itrs<object_t>),
            len(len),
            ofs(ofs){

            if (use_negtive_order){
                turn_negtive_order();
            }
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
        ranger(base impl) : 
            base(impl){}

        ranger(item_t const * ptr, uxx len) : 
            base(ptr, len, 0){}

        template<class seq_t>
        requires(can_rangerlizex<seq_t, item_t>)
        ranger(seq_t const & list) : 
            base(list){
        }

        ranger(inc::initializer_list<item_t> const & list){ // 使用显式的 initializer_list
            xnew(this) ranger(list.begin(), list.size());
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

        template<can_interval interval_t>
        ranger<item_t> subseq(interval_t const & i){
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
    $

}

namespace mixc::interface_private_ranger::origin{
    using mixc::interface_private_ranger::can_rangerlize;
    using mixc::interface_private_ranger::can_rangerlizex;

    template<class item_t>
    using ranger = mixc::docker_adapter_array_access::adapter_array_access<
        mixc::interface_private_ranger::ranger<item_t>
    >;
}

#define xranger(...)                                                                        \
template<can_interval interval_t>                                                           \
::mixc::interface_private_ranger::origin                                                    \
::ranger<__VA_ARGS__> range(interval_t const & i) const {                                   \
    using namespace ::mixc::interface_private_ranger;                                       \
    if (i.normalize(this->length());                                                        \
        i.left() <= i.right()){                                                             \
        return base(this, i.right() - i.left() + 1, i.left());                              \
    }                                                                                       \
    else{                                                                                   \
        return base(this, i.left() - i.right() + 1, i.left(), true/*negtive order*/);       \
    }                                                                                       \
}

#endif

xexport_space(mixc::interface_private_ranger::origin)