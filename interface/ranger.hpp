#ifndef xpack_interface_ranger
#define xpack_interface_ranger
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_ranger
        #include"define/base_type.hpp"
        #include"interface/initializer_list.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"memop/signature.hpp"
        #include"memop/addressof.hpp"
        #include"meta/is_based_on.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::interface_ranger{
        struct data{
            voidp   ptr;
            uxx     ofs;
            uxx     len;

            data() = default;
            data(void const * ptr, uxx ofs, uxx len) : 
                ptr(voidp(ptr)), ofs(ofs), len(len){}

        };

        #define xgen(name,op,...)                                           \
        template<class owner, class item>                                   \
        struct name : data {                                                \
            using base = data;                                              \
                                                                            \
            name(owner const * ptr, item const &, uxx ofs, uxx len) :       \
                base(ptr, ofs, len){}                                       \
            item & operator[](uxx index) {                                  \
                return (*(owner *)base::ptr)__VA_ARGS__[base::ofs op index];\
            }                                                               \
            item const & operator[](uxx index) const {                      \
                return (*(owner *)base::ptr)__VA_ARGS__[base::ofs op index];\
            }                                                               \
        }
        
        xgen(positive, +);
        xgen(special , +, .begin());
        xgen(negative, -);
        #undef xgen

        template<class object_t, class item_t>
        concept can_random_access = requires(object_t items, uxx i, item_t value){
            value = items[i];
        };

        template<class item>
        struct ranger {
        private:
            mutable data  dat;
            mutable voidp func;
        public:
            #ifdef xfor_msvc_hint
                template<class impl> ranger(impl const &){}
            #else
                template<class impl> requires 
                can_random_access<impl, item>
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
            #endif

            ranger(inc::initializer_list<item> list){
                auto p = special{ inc::addressof(list), list.begin()[0], 0, list.size() };
                dat    = p;
                func   = inc::signature<const item &, uxx>::check(& decltype(p)::operator[]);
            }

            item & operator[](uxx index){
                return inc::signature<item &, uxx>::call(& dat, func, index);
            }

            const item & operator[](uxx index) const {
                return inc::signature<item &, uxx>::call(& dat, func, index);
            }

            uxx length() const {
                return dat.len;
            }
        };
    }

    #define xrange(...)                                                                 \
    ::mixc::interface_ranger::ranger<__VA_ARGS__> range(ixx start, ixx end = -1) const {\
        using namespace ::mixc::interface_ranger;                                       \
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
    using ::mixc::interface_ranger::ranger;
}
