// 暂不支持[大端模式]

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

    static constexpr uxx total_bits(){
        return sizeof(items) * 8;
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

template<uxx unit>
inline carry<big_uint<unit>> operator >> (big_uint<unit> const & a, uxx bits){
    carry<big_uint<unit>> r{};

    #ifndef xhas_isa
        if (bits >= a.total_bits()){
            if (bits == a.total_bits()){
                r.has_carry         = u08(a[unit - 1] >> 63);
            }
            return r;
        }
        if (bits == 0){
            return a;
        }

        uxx i                       = bits / 64;
        uxx j                       = bits % 64;
        uxx k                       = 0;

        if (j != 0){
            r.has_carry             = u08(1 & (a[i] >> (j - 1)));

            for(; i < unit - 1; i++, k++){
                u64 l               = a[i + 0] >> (j);
                u64 h               = a[i + 1] << (64 - j);
                r[k]                = h | l;
            }

            r[k]                    = a[i] >> j;
        }
        else{
            r.has_carry             = u08(1 & (a[i - 1] >> 63));

            for(; i < unit; i++, k++){
                r[k]                = a[i];
            }
        }
        return r;
    #else
        #undef xhas_isa
    #endif
}

template<uxx unit>
inline carry<big_uint<unit>> operator << (big_uint<unit> const & a, uxx bits){
    carry<big_uint<unit>> r{};

    #ifndef xhas_isa
        if (bits >= a.total_bits()){
            if (bits == a.total_bits()){
                r.has_carry         = u08(a[0] & 1);
            }
            return r;
        }
        if (bits == 0){
            return a;
        }

        uxx i                       = unit - 1 - bits / 64;
        uxx j                       = bits % 64;
        uxx k                       = unit - 1;

        if (j != 0){
            r.has_carry             = u08(1 & (a[i] >> (64 - j)));

            for(; i > 0; i--, k--){
                u64 l               = a[i] << (j);
                u64 h               = a[i - 1] >> (64 - j);
                r[k]                = h | l;
            }

            r[k]                    = a[i] << j;
        }
        else{
            r.has_carry             = u08(a[i] & 1);

            for(; i != not_exist; i--, k--){
                r[k]                = a[i];
            }
        }
        return r;
    #else
        #undef xhas_isa
    #endif
}

template<uxx unit>
inline carry<big_uint<unit>> operator | (big_uint<unit> const & a, big_uint<unit> const & b){
    carry<big_uint<unit>> r{};

    #ifndef xhas_isa
        for(uxx i = 0; i < unit; i++){
            r[i]                    = a[i] | b[i];
            r.has_carry            |= r[i] != 0;
        }
        return r;
    #else
        #undef xhas_isa
    #endif
}

template<uxx unit>
inline carry<big_uint<unit>> operator & (big_uint<unit> const & a, big_uint<unit> const & b){
    carry<big_uint<unit>> r{};

    #ifndef xhas_isa
        for(uxx i = 0; i < unit; i++){
            r[i]                    = a[i] & b[i];
            r.has_carry            |= r[i] != 0;
        }
        return r;
    #else
        #undef xhas_isa
    #endif
}

template<uxx unit>
inline carry<big_uint<unit>> operator ^ (big_uint<unit> const & a, big_uint<unit> const & b){
    carry<big_uint<unit>> r{};

    #ifndef xhas_isa
        for(uxx i = 0; i < unit; i++){
            r[i]                    = a[i] ^ b[i];
            r.has_carry            |= r[i] != 0;
        }
        return r;
    #else
        #undef xhas_isa
    #endif
}

template<uxx unit>
inline carry<big_uint<unit>> operator ~ (big_uint<unit> const & a){
    carry<big_uint<unit>> r{};

    #ifndef xhas_isa
        for(uxx i = 0; i < unit; i++){
            r[i]                    = ~a[i];
            r.has_carry            |= r[i] != 0;
        }
        return r;
    #else
        #undef xhas_isa
    #endif
}

template<uxx unit>
inline bool operator ! (big_uint<unit> const & a){
    bool r = false;

    #ifndef xhas_isa
        for(uxx i = 0; i < unit; i++){
            r                      &= a[i] == 0;
        }
        return r;
    #else
        #undef xhas_isa
    #endif
}

template<uxx unit>
inline ixx compare (big_uint<unit> const & a, big_uint<unit> const & b){
    for(uxx i = 0; i < unit; i++){
        if (a[i] != b[i]){
            return a[i] > b[i] ? 1 : -1;
        }
    }
    return 0;
}

template<uxx unit>
inline bool operator >  (big_uint<unit> const & a, big_uint<unit> const & b){
    return compare<unit>(a, b) > 0;
}

template<uxx unit>
inline bool operator >= (big_uint<unit> const & a, big_uint<unit> const & b){
    return compare<unit>(a, b) >= 0;
}

template<uxx unit>
inline bool operator <  (big_uint<unit> const & a, big_uint<unit> const & b){
    return compare<unit>(a, b) < 0;
}

template<uxx unit>
inline bool operator <= (big_uint<unit> const & a, big_uint<unit> const & b){
    return compare<unit>(a, b) <= 0;
}

template<uxx unit>
inline bool operator == (big_uint<unit> const & a, big_uint<unit> const & b){
    return compare<unit>(a, b) == 0;
}

template<uxx unit>
inline bool operator != (big_uint<unit> const & a, big_uint<unit> const & b){
    return compare<unit>(a, b) != 0;
}
