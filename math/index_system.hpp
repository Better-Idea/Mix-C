#ifndef xpack_math_index_system
#define xpack_math_index_system
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::math_index_system
    #include"define/base_type.hpp"
    #include"macro/xgc.hpp"
    #include"macro/xindex_rollback.hpp"
    #include"macro/xitf.hpp"
    
    namespace mixc{
        xitf(iinterval,
            xitem(normalize, void, uxx),
            xitem(left, uxx),
            xitem(right, uxx)
        );

        // bend close interval
        struct cc{
            xgc_fields(
                xiam(cc),
                xpro(pleft,  ixx),
                xpro(pright, ixx)
            );
        public:
            template<class left_t, class right_t = ixx>
            cc(left_t left, right_t right = right_t(-1)) : 
                pleft(ixx(left)), pright(ixx(right)){}

            void normalize(uxx length){
                xindex_rollback(length, pleft,  +1);
                xindex_rollback(length, pright, +1);
            }

            uxx left() const {
                return pleft;
            }

            uxx right() const {
                return pright;
            }
        };

        // left close right open interval
        struct co : cc{
            using cc::cc;

            void normalize(uxx length){
                cc::normalize(length);
                pright -= (pleft <= pright ? 1 : -1);
            }
        };

        // left open right close interval
        struct oc : cc{
            using cc::cc;

            void normalize(uxx length){
                cc::normalize(length);
                pleft += (pleft <= pright ? 1 : -1);
            }
        };

        // bend open interval
        struct oo : cc{
            using cc::cc;

            void normalize(uxx length){
                cc::normalize(length);
                auto asc = pleft <= pright;
                pleft  += (asc ? 1 : -1);
                pright -= (asc ? 1 : -1);
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

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::pack;
}
