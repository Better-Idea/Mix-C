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
        union __typeid {
            using the_type = typename remove_ref<
                typename remove_const<type>::result
            >::result;

            union{
                operator asciis() const {
                    #define xgen(type)      else if constexpr (is_same<the_type, type>){ return # type; }

                    if constexpr (is_class<the_type>){
                        return the_type::__self_name;
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
                        return "wait implement";
                    }

                    #undef  xgen
                }

                auto operator ()() const {
                    return operator asciis();
                }
            }name;

            union{
                operator uxx () const {
                    #define xgen(type)      else if constexpr (is_same<the_type, type>){ return __COUNTER__ - __start; }
                    enum { __start = __COUNTER__ + 1 };

                    if constexpr (is_class<the_type>){
                        return the_type::__class_id;
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
                        return not_exist;
                    }

                    #undef  xgen
                }

                auto operator ()() const {
                    return operator uxx();
                }
            } class_id;
        };
    }

    #define xtypeid(...)      mixc::macro_xtypeid::__typeid<__VA_ARGS__>()
#endif
