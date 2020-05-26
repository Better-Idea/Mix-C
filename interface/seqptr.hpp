#ifndef xpack_interface_seqptr
#define xpack_interface_seqptr
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_seqptr
        #include"define/base_type.hpp"
        #include"macro/xdebug_fail.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"memop/signature.hpp"
        #include"meta/has_cast.hpp"
        #include"math/index_system.hpp"
    #pragma pop_macro("xuser")

    #define xseqptr(...)                                                            \
    inc::seqptr<__VA_ARGS__> seq(::mixc::iinterval i) const {                       \
        using ptr_t = __VA_ARGS__ *;                                                \
        auto  len   = the.length();                                                 \
        auto  ptr   = ptr_t(this->operator item_t const *());                       \
        i.normalize(len);                                                           \
        return inc::seqptr<__VA_ARGS__>(ptr + i.left(), i.right() - i.left() + 1);  \
    }

    namespace mixc::interface_seqptr{
        template<class item_t> struct seqptr;

        namespace inc{
            using ::mixc::interface_seqptr::seqptr;
        }

        template<class item_t>
        struct seqptr{
            seqptr(){}

            seqptr(item_t * ptr, uxx len) : 
                ptr(ptr), len(len){}

            template<class object> requires(
                inner::signature<uxx>::check(& object::length) and
                inner::signature<uxx>::check(& object::operator item_t const *)
            )
            seqptr(object const & impl){
                len = impl.length();
                ptr = (item_t *)(item_t const *)impl;
            }

            operator item_t *(){
                return ptr;
            }

            operator item_t const *() const{
                return ptr;
            }

            uxx length() const {
                return len;
            }

            xseqptr(item_t);
        private:
            item_t * ptr = nullptr;
            uxx      len = 0;
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::interface_seqptr::seqptr;
}

#include"math/index_system.hpp"
