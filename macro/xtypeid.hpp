#ifndef xpack_macro_xtypeid
#define xpack_macro_xtypeid
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_xtypeid
    #include"define/base_type.hpp"
    #include"meta/is_belong_to.hpp"
    #include"meta/is_class.hpp"
    #include"meta/is_same.hpp"
    #include"meta/remove_const.hpp"
    #include"meta/remove_ref.hpp"
    #include"meta_seq/tlist.hpp"
    #include"meta_seq/vlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xtypeid{
        using namespace inc;

        struct __dummy{};

        template<class type, class dummy> struct __typeid;
        template<class type>
        struct __typeid<type, __dummy> {
            using the_type = remove_ref<
                remove_const<type>
            >;

            union{
                operator asciis() const {
                    #define xgen(target)      else if constexpr (is_same<type, target>){ return # target; }
                    constexpr int len       = 32;
                    constexpr int leave_out = 3;
                    static char the_name[len + leave_out + 1/* \0 */] = { 0 };

                    if constexpr (is_class<the_type>){
                        auto ptr = the_type::__my_name;
                        the_name[len + 0] = '.';
                        the_name[len + 1] = '.';
                        the_name[len + 2] = '.';

                        if (the_name[0] == 0) for (int i = 0; i < len; i++) {
                            if (auto c = ptr[i];
                                ('a' <= c and c <= 'z') or
                                ('A' <= c and c <= 'Z') or
                                ('0' <= c and c <= '9') or
                                ('$' == c) or
                                ('_' == c)) {
                                the_name[i] = ptr[i];
                            }
                            else {
                                the_name[i] = '\0';
                            }
                        }
                        return the_name;
                    }

                    xgen(char)
                    xgen(char *)
                    xgen(u08)
                    xgen(u08p)
                    xgen(u16)
                    xgen(u16p)
                    xgen(u32)
                    xgen(u32p)
                    xgen(u64)
                    xgen(u64p)
                    xgen(i08)
                    xgen(i08p)
                    xgen(i16)
                    xgen(i16p)
                    xgen(i32)
                    xgen(i32p)
                    xgen(i64)
                    xgen(i64p)
                    xgen(f32)
                    xgen(f32p)
                    xgen(f64)
                    xgen(f64p)
                    xgen(asciis)
                    xgen(void)
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
                        return the_type::__my_class_id;
                    }

                    xgen(char)
                    xgen(char *)
                    xgen(u08)
                    xgen(u08p)
                    xgen(u16)
                    xgen(u16p)
                    xgen(u32)
                    xgen(u32p)
                    xgen(u64)
                    xgen(u64p)
                    xgen(i08)
                    xgen(i08p)
                    xgen(i16)
                    xgen(i16p)
                    xgen(i32)
                    xgen(i32p)
                    xgen(i64)
                    xgen(i64p)
                    xgen(f32)
                    xgen(f32p)
                    xgen(f64)
                    xgen(f64p)
                    xgen(asciis)
                    xgen(void)
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

        template<class type, class dummy>
        struct __typeid : __typeid<type, __dummy> {
            template<class callback>
            void foreach_fields(type const & value, callback const & call){
                using bl    = typename type::base_list;
                foreach_base(bl(), value, call);
            }
        private:
            template<class callback, class expand, class base, class ... rest_base>
            void foreach_base(tlist<base, rest_base...>, expand const & child, callback const & call){
                using bl    = typename base::base_list;

                foreach_base(bl(), (base &)child/*二重李氏转换*/, call);
                foreach_base(tlist<rest_base...>(), child/*李氏转换*/, call);
            }

            template<class callback, class expand>
            void foreach_base(tlist<>, expand const & child, callback const & call){
                using mlp   = typename expand::member_list_partial;
                foreach_fields(0, mlp(), child, call);
            }

            template<class callback, class expand, auto field_ptr, auto ... rest_field_ptr>
            void foreach_fields(uxx i, vlist<field_ptr, rest_field_ptr...>, expand const & current, callback const & call){
                if constexpr (is_belong_to<expand, field_ptr>){
                    call(i, expand::__my_field_name[i], current.*field_ptr);
                }
                foreach_fields(i + 1, vlist<rest_field_ptr...>(), current, call);
            }

            template<class callback, class expand>
            void foreach_fields(uxx, vlist<>, expand const &, callback const &){}
        };

        
        template<>
        struct __typeid<void, void> : __typeid<void, __dummy>{};
    }

    #define xtypeid(...)      mixc::macro_xtypeid::__typeid<__VA_ARGS__, void>()
#endif
