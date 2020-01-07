#pragma once
#include"definition/mini.hpp"

xspace(mixc)
    constexpr uxx tag_ref   = 1;
    constexpr uxx tag_const = 2;
    xspace(reflect)
        template<bool choice_a, class a, class b>
        xstruct(condition_select)
            typedef a result;
        $;

        template<class a, class b>
        xstruct(condition_select<false, a, b>)
            typedef b result;
        $;

        template<bool choice_a, class type, type a, type b>
        xstruct(condition_select_value)
            static constexpr type result = a;
        $;

        template<class type, type a, type b>
        xstruct(condition_select_value<false, type, a, b>)
            static constexpr type result = b;
        $;

        template<class a>
        xstruct(remove_ref_tag)
            typedef a result;
        $;

        template<class a>
        xstruct(remove_ref_tag<a &>)
            typedef a result;
        $;

        template<class a>
        xstruct(remove_const_tag)
            typedef a result;
        $;

        template<class a>
        xstruct(remove_const_tag<const a>)
            typedef a result;
        $;

        template<class a, uxx tag = 0>
        xstruct(remove)
        private:
            typedef typename condition_select<(tag & tag_ref) != 0, typename remove_ref_tag<a>::result, a>::result r0;
            typedef typename condition_select<(tag & tag_const) != 0, typename remove_const_tag<r0>::result, r0>::result r1;
        public:
            typedef r1 result;
        $;

        template<uxx> 
        struct unsigned_format{};
        template<> xstruct(unsigned_format<1>) typedef u08 result; $;
        template<> xstruct(unsigned_format<2>) typedef u16 result; $;
        template<> xstruct(unsigned_format<4>) typedef u32 result; $;
        template<> xstruct(unsigned_format<8>) typedef u64 result; $;

        template<class a> xstruct(is_enum)
            static constexpr bool result = __is_enum(a);
        $;

        template<class a> xstruct(is_ptr)
            static constexpr bool result = false;
        $;

        template<class a> xstruct(is_ptr<a *>)
            static constexpr bool result = true;
        $;

        template<class a> xstruct(is_ref)
            static constexpr bool result = false;
        $;

        template<class a> xstruct(is_ref<a &>)
            static constexpr bool result = true;
        $;

        template<class a> xstruct(is_volatile)
            static constexpr bool result = false;
        $;

        template<class a> xstruct(is_volatile<volatile a>)
            static constexpr bool result = true;
        $;

        template<class a> xstruct(is_const)
            static constexpr bool result = false;
        $;

        template<class a> xstruct(is_const<const a>)
            static constexpr bool result = true;
        $;

        template<class a> xstruct(is_float)
            static constexpr bool result = false;
        $;

        template<> xstruct(is_float<f32>)
            static constexpr bool result = true;
        $;

        template<> xstruct(is_float<f64>)
            static constexpr bool result = true;
        $;

        template<class a> xstruct(is_unsigned)
            static constexpr bool result = false;
        $;

        template<> xstruct(is_unsigned<u08>)
            static constexpr bool result = true;
        $;

        template<> xstruct(is_unsigned<u16>)
            static constexpr bool result = true;
        $;

        template<> xstruct(is_unsigned<u32>)
            static constexpr bool result = true;
        $;

        template<> xstruct(is_unsigned<u64>)
            static constexpr bool result = true;
        $;

        template<class a> xstruct(is_signed)
            static constexpr bool result = false;
        $;

        template<> xstruct(is_signed<i08>)
            static constexpr bool result = true;
        $;

        template<> xstruct(is_signed<i16>)
            static constexpr bool result = true;
        $;

        template<> xstruct(is_signed<i32>)
            static constexpr bool result = true;
        $;

        template<> xstruct(is_signed<i64>)
            static constexpr bool result = true;
        $;

        template<class a> xstruct(is_digital)
            static constexpr bool result = is_signed<a>::result || is_unsigned<a>::result;
        $;

        template<class a> xstruct(is_number)
            static constexpr bool result = is_digital<a>::result || is_float<a>::result || is_enum<a>::result;
        $;

        template<class a> xstruct(is_union)
            static constexpr bool result = __is_union(a);
        $;

        template<class a> xstruct(is_class)
            static constexpr bool result = __is_class(a);
        $;

        template<class a> xstruct(is_empty_class)
        private:
            static constexpr bool is_class = __is_class(a);
            xstruct(inner : condition_select<is_class, a, dummy_t>::result)
                char dummy;
            $;
        public:
            static constexpr bool result = sizeof(inner) == 1 && is_class;
        $;

        template<class a, class b> xstruct(is_euqal_type)
            static constexpr bool result = false;
        $;

        template<class a> xstruct (is_euqal_type<a, a>)
            static constexpr bool result = true;
        $;

        template<ixx index, class first, class ... param>
        xstruct(select_type)
            typedef typename select_type<(
                    index > 0 ? index - 1 : 
                -index == sizeof...(param) ? 0 : 
                    index
                ),  param...
            >::result result;
        $;
        
        template<class current, class ... param> 
        xstruct(select_type<0, current, param...>)
            typedef current result;
        $;

        template<ixx index, class type, type a, type ... values>
        xstruct(select_value)
            static constexpr type result = select_value<(
                    index > 0 ? index - 1 : 
                -index == sizeof...(values) ? 0 : 
                    index
                ),  type, values...
            >::result;
        $;
        
        template<class type, type a, type ... values> 
        xstruct(select_value<0, type, a, values...>)
            static constexpr type result = a;
        $;

        template<class a>
        xstruct(type)
            template<class b>
            xstruct(and_type)
                static constexpr bool same      = reflect::is_euqal_type<a, b>::result;
                static constexpr bool diffrent  = ! same;
            $;

            template<class b>
            xstruct(based_on)
            private:
                static constexpr xdef(test, b *)
                    return true;
                $;
                static constexpr xdef(test, voidp)
                    return false;
                $;
            public:
                constexpr xcvtc(bool) 
                    return test((a *)nullptr) && and_type<b>::diffrent;
                $;
            $;

            static constexpr bool is_enum           = reflect::is_enum<a>::result;
            static constexpr bool is_ptr            = reflect::is_ptr<a>::result;
            static constexpr bool is_ref            = reflect::is_ref<a>::result;
            static constexpr bool is_volatile       = reflect::is_volatile<a>::result;
            static constexpr bool is_const          = reflect::is_const<a>::result;
            static constexpr bool is_signed         = reflect::is_signed<a>::result;
            static constexpr bool is_unsigned       = reflect::is_unsigned<a>::result;
            static constexpr bool is_digital        = reflect::is_digital<a>::result;
            static constexpr bool is_number         = reflect::is_number<a>::result;
            static constexpr bool is_union          = reflect::is_union<a>::result;
            static constexpr bool is_class          = reflect::is_class<a>::result;
            static constexpr bool is_empty_class    = reflect::is_empty_class<a>::result;
            static constexpr bool is_float          = reflect::is_float<a>::result;

            template<uxx tag>
            using remove                            = reflect::remove<a, tag>;
            using unsigned_format                   = typename reflect::unsigned_format<sizeof(a)>::result;
        $;
        
        template<class type> struct function;

        #define xgen(obj,...)                                                   \
        template<class ret_type, obj class ... list>                            \
        xstruct(function<ret_type __VA_ARGS__(list...)>)                        \
            static constexpr uxx param_count = sizeof...(list);                 \
            typedef ret_type return_type;                                       \
                                                                                \
            template<ixx index>                                                 \
            xstruct(param)                                                      \
                typedef typename                                                \
                reflect::select_type<index, list...>::result result;            \
            $;                                                                  \
        $;

        #define xarg    class object,
        xgen(,)
        xgen(xarg, (object::*));
        #undef xarg
        #undef xgen

        #define xgen(t,max_value,min_value,...)        xge2(t,max_value,min_value,max_value, ## __VA_ARGS__)
        #define xge2(t,max_value,min_value,asciis_min_value,...)            \
        template<> xstruct(type<t>)                                         \
            static constexpr t max         = t(max_value ## __VA_ARGS__);   \
            static constexpr t min         = t(min_value ## __VA_ARGS__);   \
            static constexpr asciis asciis_max  = #max_value;               \
            static constexpr asciis asciis_min  = #asciis_min_value;        \
        $;

        xgen(i08,                  127, -128);
        xgen(i16,                32767, -32768);
        xgen(i32,           2147483647, -2147483648);
        xge2(i64,  9223372036854775807, -9223372036854775807LL - 1, -9223372036854775808);

        xgen(u08,                  255, 0);
        xgen(u16,                65535, 0);
        xgen(u32,           4294967295, 0);
        xgen(u64, 18446744073709551615, 0, ULL);

        xgen(f32,           3.40282347e+38, -3.40282347e+38);
        xgen(f64,  1.7976931348623157e+308, -1.7976931348623157e+308);
        #undef xgen
        #undef xge2

        template<bool condition> xstruct(when)
            template<class a>
            xstruct(select)
                template<class b>
                xstruct(otherwise)
                    typedef typename reflect::condition_select<condition, a, b>::type result;
                $;
            $;

            template<class type>
            xstruct(choice)
                template<type a>
                xstruct(value)
                    template<type b>
                    xstruct(otherwise)
                        constexpr xcvtc(type)
                            return reflect::condition_select_value<condition, type, a, b>();
                        $;
                    $;
                $;
            $;
        $;

        template<class a, class ... param>
        xstruct(from)
            template<uxx index>
            xstruct(select)
                typedef typename reflect::select_type<index, a, param...>::result result;
            $;
            template<a first, a ... rest>
            xstruct(type)
                template<uxx index>
                xstruct(chioce)
                    constexpr xcvtc(type)
                        reflect::select_value<index, a, first, rest...>();
                    $;
                $;
            $;
        $;

        template<class a>
        using pure_type = typename type<a>::template remove<tag_ref | tag_const>::result;

        template<class ... params>
        constexpr xdef(param_count, params const & ... list) 
            return sizeof...(params);
        $

        template<class a, class ... params>
        constexpr xdef(first_param, a const & first, params const & ... list) 
            return first;
        $
    $
$

