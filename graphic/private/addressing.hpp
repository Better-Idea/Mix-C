#ifndef xpack_extern_gui_private_addressing
#define xpack_extern_gui_private_addressing
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_private_addressing::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_private_addressing {
    template<uxx arg_width, uxx arg_height>
    xstruct(
        xtmpl(addressing, arg_width, arg_height)
    )
    protected:
        uxx operator()(uxx x, uxx y){
            return y * arg_width + x;
        }
    public:
        constexpr uxx width(){
            return arg_width;
        }

        constexpr uxx height(){
            return arg_height;
        }

        constexpr uxx pixels(){
            return arg_width * arg_height;
        }
    };
}

#endif

xexport(mixc::extern_gui_private_addressing::addressing)