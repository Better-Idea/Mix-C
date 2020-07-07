#ifndef xpack_math_index_system
#define xpack_math_index_system
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::math_index_system
    #include"define/base_type.hpp"
    #include"macro/xindex_rollback.hpp"
    #include"macro/xitf.hpp"
    #include"macro/xstruct.hpp"
    #pragma pop_macro("xuser")
    
    namespace mixc{
        xitf(iinterval,
            xitem(normalize, void, uxx),
            xitem(left, uxx),
            xitem(right, uxx)
        );

        // bend close interval
        xstruct(
            xiam(cc),
            xitm(pleft, ixx),
            xitm(pright, ixx)
        )
            using final = cc;

            template<class left_t, class right_t = ixx>
            cc(left_t left, right_t right = right_t(-1)) : 
                pleft(ixx(left)), pright(ixx(right)){}

            void normalize(uxx length){
                xindex_rollback(length, pleft,  +1);
                xindex_rollback(length, pright, +1);
            }

            xpubget_proset(left)
            xpubget_proset(right)
        };

        // left close right open interval
        struct co : cc{
            using cc::cc;

            void normalize(uxx length){
                cc::normalize(length);
                right(
                    right() - (left() <= right() ? 1 : -1)
                );
            }
        };

        // left open right close interval
        struct oc : cc{
            using cc::cc;

            void normalize(uxx length){
                cc::normalize(length);
                left(
                    left() + (left() <= right() ? 1 : -1)
                );
            }
        };

        // bend open interval
        struct oo : cc{
            using cc::cc;

            void normalize(uxx length){
                cc::normalize(length);
                auto asc = left() <= right();

                left(
                    left() + (asc ? 1 : -1)
                );
                right(
                    right() - (asc ? 1 : -1)
                );
            }
        };

        namespace pack{
            using ::mixc::cc;
            using ::mixc::oc;
            using ::mixc::co;
            using ::mixc::cc;
            using ::mixc::iinterval;
        }
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::pack;
}
