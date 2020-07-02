#ifndef xpack_extern_gui_private_addressing
#define xpack_extern_gui_private_addressing
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::extern_gui_private_addressing
    #include"mixc.hpp"

    namespace xuser::origin {
        template<uxx arg_width, uxx arg_height>
        struct addressing{
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

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc {
    using namespace ::mixc::extern_gui_private_addressing::origin;
}
