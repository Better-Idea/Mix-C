#ifndef xpack_math_index_system
#define xpack_math_index_system
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::math_index_system
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"macro/xitf.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::math_index_system{
        xitf(iinterval,
            xitem(normalize, void, uxx),
            xitem(left, uxx),
            xitem(right, uxx)
        );

        // left open right open interval
        struct oo{
            xgc_fields(
                xiam(oo),
                xpro(pleft,  ixx),
                xpro(pright, ixx)
            );
        public:
            oo(ixx left, ixx right = -1) : 
                pleft(left), pright(right){}

            void normalize(uxx length){
                xindex_rollback(length, pleft , +1);
                xindex_rollback(length, pright, +1);
            }

            uxx left() const {
                return pleft;
            }

            uxx right() const {
                return pright;
            }
        };

        // left open right close interval
        struct oc : oo{
            using oo::oo;

            void normalize(uxx length){
                oo::normalize(length);
                pright -= (pleft <= pright ? 1 : -1);
            }
        };

        // left close right open interval
        struct co : oo{
            using oo::oo;

            void normalize(uxx length){
                oo::normalize(length);
                pleft += (pleft <= pright ? 1 : -1);
            }
        };

        // left close right close interval
        struct cc : oo{
            using oo::oo;

            void normalize(uxx length){
                oo::normalize(length);
                auto asc = pleft <= pright;
                pleft  += (asc ? 1 : -1);
                pright -= (asc ? 1 : -1);
            }
        };

        namespace pack{
            using ::mixc::math_index_system::oo;
            using ::mixc::math_index_system::oc;
            using ::mixc::math_index_system::co;
            using ::mixc::math_index_system::cc;
            using ::mixc::math_index_system::iinterval;
        }
    }
#endif

namespace xuser::inc{
    using namespace ::mixc::math_index_system::pack;
}
