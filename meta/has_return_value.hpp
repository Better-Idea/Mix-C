#ifndef xpack_meta_has_return_value
#define xpack_meta_has_return_value
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_return_value::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/function.hpp"
#include"meta/is_func.hpp"
#include"meta/is_lambda.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_return_value::origin{
    template<class call_t>
    inline constexpr bool invoke(){
        if constexpr (inc::is_lambda<call_t>){
            using lambda_t = decltype(& call_t::operator());
            return not inc::is_same<void, typename inc::function<lambda_t>::return_type>;
        }
        else if constexpr (inc::is_func<call_t>){
            return not inc::is_same<void, typename inc::function<call_t>::return_type>;
        }
        else{
            return false;
        }
    }

    template<class func_t>
    concept has_return_value = invoke<func_t>();
}

#endif

xexport_space(mixc::meta_has_return_value::origin)