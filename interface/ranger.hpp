#ifndef xpack_interface_ranger
#define xpack_interface_ranger
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_ranger
#include"configure.hpp"
#include"define/base_type.hpp"
#include"interface/initializer_list.hpp"
#include"math/index_system.hpp"
#include"memop/signature.hpp"
#pragma pop_macro("xuser") 

namespace mixc::interface_ranger{
    using namespace inc;

    xstruct(
        xname(base)
    )
        base(){}

        template<class object>
        base(object const * ptr, uxx len, uxx ofs):
            ptr((object *)ptr),
            itr((voidp *)& itrs<object, decltype((*(object *)ptr)[uxx(0)])>),
            len(len),
            ofs(ofs){
        }

        template<class item_t>
        base(inc::initializer_list<item_t> const * ptr, uxx len, uxx ofs):
            ptr((item_t *)ptr->begin()),
            itr((voidp *)& itrsx<item_t>),
            len(len),
            ofs(ofs){
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
    protected:
        voidp    ptr = nullptr;
        voidp *  itr = nullptr;
        uxx      len = 0;
        uxx      ofs = 0;

        void turn_positive_order() {
            itr = (voidp *)(uxx(itr) & ~mask);
        }

        void turn_negtive_order() {
            itr = (voidp *)(uxx(itr) | sizeof(uxx));
        }
    private:
        template<class object, class ret>
        static ret * return_type_core(ret &(object::*)(uxx)){
            return (ret *)nullptr;
        }

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

        enum{ mask = sizeof(uxx) * 2 - 1 };

        template<class object, class return_type>
        alignas(mask + 1) static inline voidp itrs[] = {
            inc::signature<return_type(uxx)>::check(& base::pos<object, return_type>),
            inc::signature<return_type(uxx)>::check(& base::neg<object, return_type>),
        };

        template<class return_type>
        alignas(mask + 1) static inline voidp itrsx[] = {
            inc::signature<return_type &(uxx)>::check(& base::posx<return_type>),
            inc::signature<return_type &(uxx)>::check(& base::negx<return_type>),
        };
    protected:
        template<class item_t>
        item_t access(uxx index) {
            return inc::signature<item_t(uxx)>::call(this, itr[0], index);
        }
    $

    template<class object, class item_t>
    concept ranger_format = requires(object range, item_t item, uxx length, uxx index){
        item   = (item_t)range[index];
        length = range.length();
    };

    template<class item_t>
    xstruct(
        xtmpl(ranger, item_t),
        xpubb(base)
    )
    public:
        ranger(){}
        ranger(base impl) : 
            base(impl){}

        template<class object> 
        #if not xfor_msvc_hint
        requires(ranger_format<object, item_t &>)
        #endif
        ranger(object const & impl) : 
            base(xref impl, impl.length(), 0){
        }

        template<uxx len> 
        ranger(item_t origin_array[len]) : 
            base(xref origin_array, len, 0){
        }

        ranger(inc::initializer_list<item_t> impl):
            base(xref impl, impl.size(), 0){
        }

        item_t & operator[](uxx index){
            return access<item_t &>(index);
        }

        item_t & operator[](uxx index) const {
            return access<item_t &>(index);
        }

        ranger<item_t> range(iinterval i){
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

    #define xranger(...)                                                               \
    ::mixc::interface_ranger::ranger<__VA_ARGS__> range(::mixc::iinterval i) const {   \
        using namespace ::mixc::interface_ranger;                                      \
        if (i.normalize(this->length());                                               \
            i.left() <= i.right()){                                                    \
            return base(this, i.right() - i.left() + 1, i.left());                     \
        }                                                                              \
        else{                                                                          \
            return base(this, i.left() - i.right() + 1, i.left(), true);               \
        }                                                                              \
    }
}

#endif

namespace xuser::inc{
    using ::mixc::interface_ranger::ranger;
}

#include"math/index_system.hpp"
