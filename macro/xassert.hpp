#ifndef xpack_macro_xassert
#define xpack_macro_xassert
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xassert::inc
#include"dumb/place_holder.hpp"
#include"macro/private/log.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_xassert::origin{
    struct mixc_test_context{
        uxx     case_count = 0;
        uxx     pass_count = 0;
        uxx     line;
        asciis  path;
        asciis  func_name;

        mixc_test_context(asciis path, uxx line, asciis func_name);
        ~mixc_test_context();

        template<class a0, class ... args>
        bool test(bool pass, uxx line, asciis message, a0 const & first, args const & ... rest){
            using namespace inc;

            if (case_count++; pass){
                pass_count++;
                return pass;
            }
            if (case_count == pass_count + 1){
                log(log_type_t::for_test, path, line, func_name, "[FAIL]\n", message_type_t::fail);
            }

            log(log_type_t::for_fail, path, line, func_name, message, message_type_t::fail, first, rest...);
            return false;
        }
    };
}

#define xtest(func_name)                                                                \
inline void test(                                                                       \
    mixc::macro_xassert::inc::place_holder<__COUNTER__>,                                \
    mixc::macro_xassert::origin::mixc_test_context && __test_context =                  \
    mixc::macro_xassert::origin::mixc_test_context(__FILE__, __LINE__, func_name))      \

#endif

xexport_space(mixc::macro_xassert::origin);