#ifndef xpack_chrono_private_base_t
#define xpack_chrono_private_base_t
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::chrono_private_base_t
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::chrono_private_base_t{
        template<class final, class value_t = u32>
        struct base_t{
            xgc_fields(
                xiam(base_t<final, value_t>)
            );
        public:
            base_t(value_t value = 0) : 
                pvalue(value){}

            operator value_t & (){
                return pvalue;
            }

            operator value_t const & () const {
                return pvalue;
            }

            final & operator = (uxx value){
                pvalue = value;
                return thex;
            }
        protected:
            value_t pvalue;
        };
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::chrono_private_base_t;
}
