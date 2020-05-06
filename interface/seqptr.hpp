#ifndef xpack_interface_seqptr
#define xpack_interface_seqptr
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_seqptr
        #include"define/base_type.hpp"
        #include"macro/xdebug_fail.hpp"
        #include"macro/xitf.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"math/index_system.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::interface_seqptr{
        xitf(seqptr,
            xtmpl(class item_t),
            xcast(item_t *),
            xitem(length, uxx),
        );
    }

    #define xseqptr(...)                                                    \
    inc::seqptr<__VA_ARGS__> seq(inc::iinterval i) const {                  \
        using ptr_t = __VA_ARGS__ *;                                        \
        auto  len   = the.length();                                         \
        auto  ptr   = ptr_t(this->operator const __VA_ARGS__ *());          \
        i.normalize(len);                                                   \
        struct{                                                             \
            ptr_t  ptr;                                                     \
            ixx    len;                                                     \
            uxx length(){                                                   \
                return uxx(len);                                            \
            }                                                               \
            operator ptr_t (){                                              \
                return ptr;                                                 \
            }                                                               \
        } seq { ptr + i.left(), i.right() - i.left() + 1 };                 \
        return seq;                                                         \
    }
#endif

namespace xuser::inc{
    using ::mixc::interface_seqptr::seqptr;
}

#include"math/index_system.hpp"
