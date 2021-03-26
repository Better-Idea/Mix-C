#ifndef xpack_macro_private_extern_xassert
#define xpack_macro_private_extern_xassert
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xassert::ext
#include"configure/switch.hpp"
#include"define/base_type.hpp"
#include"macro/private/log.hpp"
#include"macro/xassert.hpp"
#include"io/tty.hpp"
#include"lang/cxx/index_of_last.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_xassert::origin{
    mixc_test_context::mixc_test_context(asciis path, uxx line, asciis func_name){
        using namespace ext;

        static uxx offset = not_exist;

        if (offset == not_exist){
            c08 token   = xmixc;
            offset      = c08{path}.index_of_last(token);
            offset     += token.length() + 1; // maybe \\ or /
        }

        this->path      = path + offset;
        this->line      = line;
        this->func_name = func_name;
    }

    mixc_test_context::~mixc_test_context(){
        using namespace ext;

        if (case_count == pass_count){
            log(log_type_t::for_test, path, line, func_name, "[PASS]", message_type_t::success);
        }
        else{
            log(log_type_t::for_hint, path, line, func_name, "case, pass, fail, pass_rate,", message_type_t::normal, 
                case_count, pass_count, case_count - pass_count, 1.0 * pass_count / case_count
            );
        }
    }
}

#endif
