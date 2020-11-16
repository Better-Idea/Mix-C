#include"configure.hpp"
#include"define/base_type.hpp"

template<uxx unit>
struct big_uint{
    u64 & operator[](uxx i){
        return items[i];
    }

    auto & l(big_uint<unit / 2> const & value){
        using bup = big_uint<unit / 2> *;
        (bup(this))[0] = value;
        return this[0];
    }

    auto & h(big_uint<unit / 2> const & value){
        using bup = big_uint<unit / 2> *;
        (bup(this))[1] = value;
        return this[1];
    }

    auto & l(){
        using bup = big_uint<unit / 2> *;
        return (bup(this))[0];
    }

    auto & h(){
        using bup = big_uint<unit / 2> *;
        return (bup(this))[1];
    }
private:
    u64 items[unit];
};

template<> struct big_uint<0>{};

template<class big_int_t>
struct carry : big_int_t{
    carry(big_int_t const & value, u08 has_carry = false) : 
        big_int_t(value), has_carry(has_carry){
    }
    u08    has_carry  = false;
};

template<uxx unit>
inline carry<big_uint<unit>> operator + (big_uint<unit> const & a, carry<big_uint<unit>> const & b){
    carry<big_uint<unit>> r;

    #if xis_os64
        #if xis_x86
        for(uxx i = 0; i < unit; i++){
            r.has_carry = __builtin_ia32_addcarryx_u64(b.has_carry, a[i], b[i], & r[i]);
        }
        #define xhas_isa
        #endif
    #else
        #if xis_x86
        for(uxx i = 0; i < unit * 2; i++){
            r.has_carry = __builtin_ia32_addcarryx_u32(b.has_carry, u32p(& a)[i], u32p(& b)[i], u32p(& r) + i);
        }
        #define xhas_isa
        #endif
    #endif

    #ifndef xhas_isa
        r.has_carry = b.has_carry;

        for(uxx i = 0; i < unit; i++){
            auto l          = (u64)(u32)(a[i]      ) + (u64)(u32)(b[i]      ) + (r.has_carry);
            auto h          = (u64)(u32)(a[i] >> 32) + (u64)(u32)(b[i] >> 32) + (l  >> 32);
            r[i]            = (h << 32) | l;
            r.has_carry     = (h >> 32);
        }
    #else
        #undef xhas_isa
    #endif
    return r;
}

template<uxx unit>
inline carry<big_uint<unit>> operator - (big_uint<unit> const & a, carry<big_uint<unit>> const & b){
    carry<big_uint<unit>> r;

    #if xis_os64
        #if xis_x86
        for(uxx i = 0; i < unit; i++){
            r.has_carry = __builtin_ia32_sbb_u64(b.has_carry, a[i], b[i], & r[i]);
        }
        #define xhas_isa
        #endif
    #else
        #if xis_x86
        for(uxx i = 0; i < unit * 2; i++){
            r.has_carry = __builtin_ia32_sbb_u32(b.has_carry, u32p(& a)[i], u32p(& b)[i], u32p(& r) + i);
        }
        #define xhas_isa
        #endif
    #endif

    #ifndef xhas_isa
        r.has_carry = b.has_carry;

        for(uxx i = 0; i < unit; i++){
            auto l          = (u64)(u32)(a[i]      ) - (u64)(u32)(b[i]      ) - (r.has_carry);
            auto h          = (u64)(u32)(a[i] >> 32) - (u64)(u32)(b[i] >> 32) - (1 & (l  >> 32));
            r[i]            = (h << 32) | l;
            r.has_carry     = (h >> 32) & 1;
        }
    #else
        #undef xhas_isa
    #endif
    return r;
}

template<uxx unit>
inline big_uint<unit * 2> operator * (big_uint<unit> const & a, big_uint<unit> const & b){
    #ifndef xhas_isa
        using bu                    = big_uint<unit>;
        using bup                   = big_uint<unit> *;
        u32p ptr_a                  = u32p(& a);
        u32p ptr_b                  = u32p(& b);
        u32  product_l[unit * 4]    = {};
        u32  product_h[unit * 4]    = {};

        for(uxx i = 0; i < unit * 2; i++){
            for(uxx j = 0; j < unit * 2; j++){
                uxx idx             = i + j;
                u64 t               = u64(ptr_a[i]) * u64(ptr_b[j]);
                product_l[idx]     += u32(t >> 0 );
                product_h[idx + 1] += u32(t >> 32);
            }
        }

        auto && l                   = bup(product_l)[0] + bup(product_h)[0];
        auto && h                   = bup(product_l)[1] + carry<bu>{bup(product_h)[1], l.has_carry};
        auto && r                   = big_uint<unit * 2>{};
        return r.l(l).h(h);

    #else
        #undef xhas_isa
    #endif
}
