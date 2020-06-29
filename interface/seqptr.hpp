#ifndef xpack_interface_seqptr
#define xpack_interface_seqptr
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::interface_seqptr
    #include"define/base_type.hpp"
    #include"interface/initializer_list.hpp"
    #include"interface/ranger.hpp"
    #include"macro/xgc.hpp"
    #include"math/index_system.hpp"
    #include"memop/signature.hpp"
    
    #define xseqptr(...)                                                            \
    ::mixc::interface_seqptr::seqptr<__VA_ARGS__> seq(::mixc::iinterval i) const {  \
        using ptr_t  = __VA_ARGS__ *;                                               \
        using ptrc_t = __VA_ARGS__ const *;                                         \
        auto  len    = the.length();                                                \
        auto  ptr    = (ptr_t)(ptrc_t)this[0];                                      \
        i.normalize(len);                                                           \
        return ::mixc::interface_seqptr::seqptr<__VA_ARGS__>(                       \
            ptr + i.left(),                                                         \
            i.right() - i.left() + 1                                                \
        );                                                                          \
    }                                                                               \
    xranger(__VA_ARGS__)

    namespace xuser{
        template<class item_t> struct seqptr;

        template<class item_t>
        struct seqptr{
        private:
            xgc_fields(
                xiam(seqptr<item_t>)
            );
            item_t * ptr = nullptr;
            uxx      len = 0;
        public:
            seqptr(){}

            seqptr(item_t * ptr, uxx len) : 
                ptr(ptr), len(len){}

            template<class object> requires(
                inner::signature<uxx>::has(& object::length) and
                (
                    inner::signature<item_t const *>::has(& object::operator item_t const *) or
                    inner::signature<item_t *>::has(& object::operator item_t *)
                )
            )
            seqptr(object const & impl){
                len = impl.length();
                ptr = (item_t *)(item_t const *)impl;
            }

            seqptr(inc::initializer_list<item_t> list){
                len = list.size();
                ptr = list.begin();
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
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::interface_seqptr::seqptr;
}

#include"math/index_system.hpp"
