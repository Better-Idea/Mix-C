#pragma once
#define xmixc_space(...)                namespace mixc { namespace __VA_ARGS__ {
#define xweak                           __attribute__((weak))
#define xvar(...)                       __VA_ARGS__
#define xlink(a,b)                      a ## b
#define xlink2(a,b)                     xlink(a, b)
#define xline(name)                     xlink2(name, __LINE__)
#define _redirection(...)
#define xnew(type,ref)                  new (& (ref)) type
#define xforword(item,items,...)                                                                \
    for(auto const & _0 = items, * _1 = & _0; _1 != nullptr; _1 = nullptr)                      \
    for(auto item = _0->forword_direction(__VA_ARGS__); ! item.finished(); item = item.next()) {

#define xopposite(item,items,...)                                                               \
    for(auto const & _0 = items, * _1 = & _0; _1 != nullptr; _1 = nullptr)                      \
    for(auto item = _0->opposite_direction(__VA_ARGS__); ! item.finished(); item = item.next()) {

#define xlocal                          {
#define $                               }

#define thex                            (*this)
#define xspace(...)                     namespace __VA_ARGS__ {
#define xstruct(...)                    struct __VA_ARGS__ {
#define xclass(...)                     class __VA_ARGS__ {
#define xunion(...)                     union __VA_ARGS__ {
#define xfor(...)                       for (__VA_ARGS__) {
#define xif(...)                        if (__VA_ARGS__) {
#define xei(...)                        } else if (__VA_ARGS__) {
#define xes                             } else {
#define xswitch(...)                    switch(__VA_ARGS__) {
#define xwhile(...)                     while(__VA_ARGS__){
#define xloop(...)                      } while(__VA_ARGS__)
#define xusing                          using namespace 
#define xmain(...)                      int main(__VA_ARGS__) {

#define xdef(name,...)                  inline auto name(__VA_ARGS__) {
#define xdefc(name,...)                 auto        name(__VA_ARGS__) const {
#define xdefr(ret,name,...)             inline ret  name(__VA_ARGS__) {
#define xdefrc(ret,name,...)            ret         name(__VA_ARGS__) const {

#define xop(op,...)                     inline auto operator op (__VA_ARGS__) {
#define xopc(op,...)                           auto operator op (__VA_ARGS__) const {
#define xopr(ret,op,...)                inline ret  operator op (__VA_ARGS__) {
#define xoprc(ret,op,...)                      ret  operator op (__VA_ARGS__) const {

#define xini(...)                       __VA_ARGS__ {
#define xcvt(type)                      operator type () {
#define xcvtc(type)                     operator type () const {
#define xenum(...)                      enum __VA_ARGS__ {
#define xenumc(type)                    enum class type {

typedef signed char         i08;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;
typedef unsigned char       u08;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
typedef float               f32;
typedef double              f64;
typedef char16_t            word;
typedef const char *        asciis;

typedef i08     *           i08p;
typedef i16     *           i16p;
typedef i32     *           i32p;
typedef i64     *           i64p;
typedef u08     *           u08p;
typedef u16     *           u16p;
typedef u32     *           u32p;
typedef u64     *           u64p;
typedef f32     *           f32p;
typedef f64     *           f64p;
typedef word    *           wordp;
typedef void    *           voidp;

xspace(mixc)
    xspace(mini_private)
        template<u32 bytes> xstruct(env)
            static_assert(bytes != 4 && bytes != 8, "not supported");
        $;

        template<> xstruct(env<4>)
            typedef u32 uxx;
            typedef i32 ixx;
        $;

        template<> xstruct(env<8>)
            typedef u64 uxx;
            typedef i64 ixx;
        $;

        template<class a> xstruct(remove_ref_tag)
            typedef a result;
        $;

        template<class a> xstruct(remove_ref_tag<a &>)
            typedef a result;
        $;

        template<class a> xstruct(remove_const_tag)
            typedef a result;
        $;

        template<class a> xstruct(remove_const_tag<const a>)
            typedef a result;
        $;

        template<class a> 
        using pure_type = typename remove_ref_tag<typename remove_const_tag<a>::result>::result;
    $
$

typedef typename mixc::mini_private::env<sizeof(voidp)>::uxx uxx;
typedef typename mixc::mini_private::env<sizeof(voidp)>::ixx ixx;
typedef ixx     *           ixxp;
typedef uxx     *           uxxp;

template<class float_type, class equivalent_type, uxx decimal_bits, uxx exp_bits, uxx exp_offset>
xunion(mfxx)
    typedef mfxx<float_type, equivalent_type, decimal_bits, exp_bits, exp_offset> * mfxxp;
    xstruct()
        equivalent_type decimal : decimal_bits;
        equivalent_type exp     : exp_bits;
        equivalent_type sign    : 1;
    $;

    float_type value;

    xstruct()
        xcvtc(ixx)
            return ixx(mfxxp(this)->exp) - exp_offset;
        $
    $ real_exp;

    xini(mfxx() : value(0)) $
    xini(mfxx(float_type value) : value(value)) $

    constexpr xini(
        mfxx(equivalent_type sign, equivalent_type exp, equivalent_type decimal) :
        sign(sign), exp(exp), decimal(decimal)
    ) $

    xcvtc(float_type)
        return value;
    $

    xcvtc(float_type &) 
        return value;
    $
$;

typedef mfxx<f32, u32, 23, 8,  127 > mf32;
typedef mfxx<f64, u64, 52, 11, 1023> mf64;

xstruct(nan_t)
    friend xop(==, f32 value, nan_t)
        return !(value == value);
    $

    friend xop(==, nan_t, f32 value)
        return !(value == value);
    $

    friend xop(==, f64 value, nan_t)
        return !(value == value);
    $

    friend xop(==, nan_t, f64 value)
        return !(value == value);
    $

    friend xop(!=, f32 value, nan_t)
        return (value == value);
    $

    friend xop(!=, nan_t, f32 value)
        return (value == value);
    $

    friend xop(!=, f64 value, nan_t)
        return (value == value);
    $

    friend xop(!=, nan_t, f64 value)
        return (value == value);
    $
$;

xstruct(nullref_t)
    template<class a> friend xop(==, a const & value, nullref_t)
        return voidp(& value) == nullref_t::value;
    $

    template<class a> friend xop(==, nullref_t, a const & value)
        return voidp(& value) == nullref_t::value;
    $

    template<class a> friend xop(!=, a const & value, nullref_t)
        return voidp(& value) != nullref_t::value;
    $

    template<class a> friend xop(!=, nullref_t, a const & value)
        return voidp(& value) != nullref_t::value;
    $

    template<class a> xcvtc(a &) 
        return *(a *)value;
    $
private:
    static voidp value;
$;

xspace(mixc)
    xspace(mini_private)
        constexpr ixx greater_equal = 2;
        constexpr ixx greater       = 1;
        constexpr ixx equal         = 0;
        constexpr ixx less          = -1;
        constexpr ixx less_equal    = -2;
        constexpr ixx not_equal     = -3;
    $
$

template<ixx value>
xstruct(comparison_word)
    friend xop(==, comparison_word left, ixx right) 
        xusing mixc::mini_private;
        xswitch(value) 
        case greater_equal: return right >= 0;
        case greater:       return right >  0;
        case equal:         return right == 0;
        case less:          return right <  0;
        case less_equal:    return right <= 0;
        case not_equal:     return right != 0;
        default: throw;
        $
    $
    
    friend xop(==, ixx left, comparison_word right) 
        return right == left;
    $
    
    friend xop(!=, comparison_word left, ixx right) 
        return !(left == right);
    $
    
    friend xop(!=, ixx left, comparison_word right) 
        return !(right == left);
    $
    
    xcvtc(ixx)
        return value;
    $
$;

template<class a>
xstruct(equivalence_bytes)
    u08 bytes[sizeof(a)];

    xini(equivalence_bytes() : bytes{ 0 }) $
    xini(equivalence_bytes(a const & value))
        thex = value;
    $

    xop(=, a const & value)
        xnew(a, thex)(value);
    $

    xop(&)
        return (a *)bytes;
    $

    xcvtc(a &)
        return *(a *)bytes;
    $
    static constexpr uxx length = sizeof(a);
$;

template<class a>
xstruct(implicit_value)
    template<class b>
    xini(implicit_value(b const & value))
        value = ((a)(b &)value);
    $

    xcvtc(a &)
        return value;
    $

    xopc(->)
        return & value;
    $
private:
    a value;
$;

xstruct(dummy_t)
    template<class ... args>
    xini(dummy_t(args const & ... params)) $
$;

constexpr nullref_t nullref {};
constexpr nan_t     nan {};
constexpr mf64      pos_inf { 0ull, 0x7ffull, 0ull };
constexpr mf64      neg_inf { 1ull, 0x7ffull, 0ull };
constexpr auto      equal           = comparison_word<mixc::mini_private::equal>();
constexpr auto      not_equal       = comparison_word<mixc::mini_private::not_equal>();
constexpr auto      less            = comparison_word<mixc::mini_private::less>();
constexpr auto      less_equal      = comparison_word<mixc::mini_private::less_equal>();
constexpr auto      greater         = comparison_word<mixc::mini_private::greater>();
constexpr auto      greater_equal   = comparison_word<mixc::mini_private::greater_equal>();
constexpr uxx       magic_number    = 19961212;
constexpr uxx       not_exist       = uxx(-1);
constexpr bool      go_on           = true;
constexpr bool      finish          = false;
constexpr bool      set_state       = true;
constexpr bool      reset_state     = false;
constexpr bool      need_test       = true;
constexpr bool      tested          = false;

extern i32          global_miss_test_count;
extern i32          global_test_error_count;

xspace(mixc)
    template<class a> xdef(address_of, a const & value)
        xstruct(inner)
            xini(inner(a const & value) : value(value)) $
            a const & value;
        $ got(value);
        return (*(a **)& got);
    $

    template<class source, class target> xdefr(target &, cast, source const & value)
        return *(target *)address_of(value);
    $

    template<class a> xdef(swap, a * left, a * right)
        typedef equivalence_bytes<a> block;
        block l = cast<a, block>(left[0]);
        block r = cast<a, block>(right[0]);
        cast<a, block>(left[0]) = r;
        cast<a, block>(right[0]) = l;
    $

    template<class a> xdef(copy, a * target, a const * source, uxx count)
        a * t = (a *)(target);
        a * s = (a *)(source);

        xif(target > source) 
            xwhile(count--)
                t[count] = s[count];
            $
        xes
            xfor(uxx index = 0; index < count; index++) 
                t[index] = s[index];
            $
        $
    $

    template<class target, class source> xdef(copy, target * a, source const & b)
        typedef equivalence_bytes<target> block;
        cast<target, block>(a[0]) = cast<source, block>(b);
    $

    template<class target, class source> xdef(fill, target * a, source const & value, uxx count)
        xfor(uxx i = 0; i < count; i++) 
            a[i] = (source &)value;
        $
    $

    xdef(clear, voidp target, uxx bytes)
        xwhile(bytes--) 
            ((u08 *)(target))[bytes] = 0;
        $
    $

    template<class a> xdef(clear, a & mem)
        xnew(equivalence_bytes<a>, mem)();
    $

    #define xgen(a)                                                         \
    xdefr(ixx, compare, a const & left, a const & right)                    \
        return                                                              \
            left == right ? ixx(equal) :                                    \
            left >  right ? ixx(greater) : ixx(less);                       \
    $

    template<class a> ixx compare(a const & left, a const & right);

    xgen(i08);
    xgen(i16);
    xgen(i32);
    xgen(i64);
    xgen(u08);
    xgen(u16);
    xgen(u32);
    xgen(u64);
    xgen(f32);
    xgen(f64);
    xgen(word);
    #undef xgen

    template<class a> xdef(compare, a const & left, a const & right)
        typedef equivalence_bytes<a> block;
        block & l = cast<a, block>(left);
        block & r = cast<a, block>(right);
        
        xfor (uxx i = l.length; i-- > 0; ) 
            xif(l.bytes[i] != r.bytes[i])
                return ixx(l.bytes[i] - r.bytes[i]);
            $
        $
        return ixx(equal);
    $
$

#define __PLACEMENT_NEW_INLINE
#ifdef _MSC_VER
    xopr(void *, new, size_t bytes, voidp ptr)
        return ptr;
    $
#else
    xopr(void *, new, unsigned long bytes, voidp ptr)
        return ptr;
    $
#endif

#include"definition/mini.test_frame.hpp"
#include"definition/mini.prop_frame.hpp"

