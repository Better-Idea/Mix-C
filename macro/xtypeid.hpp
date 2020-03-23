#ifndef xpack_macro_xtypeid
#define xpack_macro_xtypeid
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xtypeid
        #include"define/base_type.hpp"
        #include"meta/is_same.hpp"
        #include"meta/is_class.hpp"
        #include"meta/remove_const.hpp"
        #include"meta/remove_ref.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xtypeid{
        using namespace inc;

        template<class type>
        union __typeid__ {
            using r0 = typename remove_const<type>::result;
            using r1 = typename remove_ref<r0>::result;

            union{
                operator asciis() const {
                    #define xgen(type)      else if constexpr (is_same<r1, type>){ return # type; }

                    if constexpr (is_class<r1>){
                        return type::__self_name__;
                    }
                    xgen(char)
                    xgen(u08)
                    xgen(u16)
                    xgen(u32)
                    xgen(u64)
                    xgen(i08)
                    xgen(i16)
                    xgen(i32)
                    xgen(i64)
                    xgen(f32)
                    xgen(f64)
                    xgen(asciis)
                    else{
                        return "base_type";
                    }

                    #undef  xgen
                }

                auto operator ()(){
                    return operator asciis();
                }
            }name;
        };
    }

    #define xtypeid(...)      mixc::macro_xtypeid::__typeid__<__VA_ARGS__>()
#endif
