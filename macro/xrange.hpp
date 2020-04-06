#ifndef xpack_macro_xrange
#define xpack_macro_xrange
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xrange
        #include"define/base_type.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"memop/signature.hpp"
        #include"memop/addressof.hpp"
        #include"meta/is_based_on.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xrange{
        struct data{
            voidp   ptr;
            uxx     ofs;
            uxx     len;

            data() = default;
            data(void const * ptr, uxx ofs, uxx len) : 
                ptr(voidp(ptr)), ofs(ofs), len(len){}

        };

        #define xgen(name,op)                                               \
        template<class owner, class item>                                   \
        struct name : data {                                                \
            using base = data;                                              \
                                                                            \
            name(owner const * ptr, item const &, uxx ofs, uxx len) :       \
                base(ptr, ofs, len){}                                       \
                                                                            \
            item & operator[](uxx index){                                   \
                return (*(owner *)base::ptr)[base::ofs op index];           \
            }                                                               \
                                                                            \
            const item & operator[](uxx index) const {                      \
                return (*(owner *)base::ptr)[base::ofs op index];           \
            }                                                               \
        }
        
        xgen(positive, +);
        xgen(negative, -);
        #undef xgen

        template<class item>
        struct ranger {
        private:
            data  dat;
            voidp func;
        public:
            template<class impl>
            ranger(impl const & imp) {
                if constexpr (inc::is_based_on<data, impl>){
                    dat    = imp;
                    func   = inc::signature<item &, uxx>::check(& impl::operator[]);
                }
                else{
                    auto p = positive{ inc::addressof(imp), imp[0], 0, imp.length() };
                    dat    = p;
                    func   = inc::signature<item &, uxx>::check(& decltype(p)::operator[]);
                }
            }

            item & operator[](uxx index){
                return inc::signature<item &, uxx>::call(& dat, func, index);
            }

            uxx length() const {
                return dat.len;
            }
        };

    }

    #define xrange()                                                                    \
    mixc::macro_xrange::ranger<uxx> range(ixx start, ixx end = -1) const {              \
        using namespace mixc::macro_xrange;                                             \
        xindex_rollback(length(), start);                                               \
        xindex_rollback(length(), end);                                                 \
        if (start <= end){                                                              \
            return positive{ this, this[0][0], uxx(start), uxx(end - start) + 1 };      \
        }                                                                               \
        else{                                                                           \
            return negative{ this, this[0][0], uxx(start), uxx(start - end) + 1 };      \
        }                                                                               \
    }
#endif

namespace xuser::inc{
    using ::mixc::macro_xrange::ranger;
}
