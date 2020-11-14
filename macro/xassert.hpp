#ifndef xpack_macro_xassert
#define xpack_macro_xassert
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xassert::inc
#include"dumb/place_holder.hpp"
#include"macro/private/log.hpp"
#include"macro/xexport.hpp"
#include"macro/xlink.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_xassert::origin{
    struct closure{
        template<class lambda>
        closure(lambda && call){
            call();
        }
    };

    struct mixc_test_context{
        uxx     case_count = 0;
        uxx     pass_count = 0;
        uxx     line;
        asciis  path;
        asciis  func_name;

        mixc_test_context(asciis path, uxx line, asciis func_name);
        ~mixc_test_context();

        template<class ... args>
        bool fail_if(bool fail, uxx line, asciis message, args const & ... rest){
            using namespace inc;

            if (case_count++; not fail){
                pass_count++;
                return false;
            }
            if (case_count == pass_count + 1){
                log(log_type_t::for_test, path, line, func_name, "[FAIL]", message_type_t::fail);
            }

            log(log_type_t::for_fail, path, line, func_name, message, message_type_t::fail, rest...);
            return true;
        }
    };
}
#define xtest(func_name)    ::mixc::macro_xassert::origin::closure xlink2(__test, __COUNTER__) =    \
    [](::mixc::macro_xassert::origin::mixc_test_context && __test_context =                         \
       ::mixc::macro_xassert::origin::mixc_test_context(__FILE__, __LINE__, func_name))

#define xfail_if(condition,...)     if (__test_context.fail_if((condition), __LINE__, #__VA_ARGS__ ",", ## __VA_ARGS__))

#endif

xexport_space(mixc::macro_xassert::origin);