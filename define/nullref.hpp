#pragma once
#include"define/base_type.hpp"

namespace mixc{
    struct nullref_t {
        template<class a> friend auto operator == (a const & value, nullref_t) {
            return voidp(& value) == nullref_t::value;
        }

        template<class a> friend auto operator == (nullref_t, a const & value) {
            return voidp(& value) == nullref_t::value;
        }

        template<class a> friend auto operator != (a const & value, nullref_t) {
            return voidp(& value) != nullref_t::value;
        }

        template<class a> friend auto operator != (nullref_t, a const & value) {
            return voidp(& value) != nullref_t::value;
        }

        template<class a> operator a & () const {
            return *(a *)value;
        }
    private:
        inline static voidp temp = nullptr;
        inline static volatile voidp * tempp = & temp;
        inline static voidp value = tempp[0]; // 以防显示的 *(type *)nullptr会被一些编译器翻译成错误的指令
    };
    constexpr mixc::nullref_t    nullref {};
}
