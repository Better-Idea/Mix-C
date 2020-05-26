#ifndef xpack_interface_ranger
#define xpack_interface_ranger
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_ranger
        #include"configure.hpp"
        #include"define/base_type.hpp"
        #include"interface/initializer_list.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"math/index_system.hpp"
        #include"memop/signature.hpp"
        #include"memop/addressof.hpp"
        #include"meta/is_based_on.hpp"
    #pragma pop_macro("xuser")

    #define xranger(...)                                                                    \
    inc::ranger<__VA_ARGS__> range(::mixc::iinterval i){                                    \
        using namespace ::mixc::interface_ranger;                                           \
        if (i.normalize(the.length());                                                      \
            i.left() <= i.right()){                                                         \
            return positive{                                                                \
                this,                                                                       \
                this[0][0],                                                                 \
                uxx(i.left()),                                                              \
                uxx(i.right() - i.left())                                                   \
            };                                                                              \
        }                                                                                   \
        else{                                                                               \
            return negative{                                                                \
                this,                                                                       \
                this[0][0],                                                                 \
                uxx(i.left()),                                                              \
                uxx(i.left() - i.right())                                                   \
            };                                                                              \
        }                                                                                   \
    }

    namespace mixc::interface_ranger{
        struct data{
            voidp   ptr;
            uxx     ofs;
            uxx     len;

            data() = default;
            data(void const * ptr, uxx ofs, uxx len) : 
                ptr(voidp(ptr)), ofs(ofs), len(len){}
        };

        #define xgen(name,op,...)                                                           \
        template<class owner, class item_t>                                                 \
        struct name : data {                                                                \
            using base = data;                                                              \
                                                                                            \
            name(owner const * ptr, item_t const &, uxx ofs, uxx len) :                     \
                base(ptr, ofs, len){}                                                       \
            item_t & operator[](uxx index) {                                                \
                return (*(owner *)base::ptr)__VA_ARGS__[base::ofs op index];                \
            }                                                                               \
            item_t const & operator[](uxx index) const {                                    \
                return (*(owner *)base::ptr)__VA_ARGS__[base::ofs op index];                \
            }                                                                               \
        }
        
        xgen(positive, +);
        xgen(special , +, .begin());
        xgen(negative, -);
        #undef xgen

        #if not xfor_msvc_hint
            template<class object_t, class item_t>
            concept can_random_access = requires(object_t items, uxx i, item_t value){
                value = items[i];
            };
        #endif

        template<class item_t> struct ranger;

        namespace inc{
            using ::mixc::interface_ranger::ranger;
        };

        template<class item_t>
        struct ranger {
        private:
            mutable data  dat;
            mutable voidp func;
            xgc_fields(
                xiam(ranger<item_t>)
            );
        public:
            #if xfor_msvc_hint
                template<class impl> ranger(impl const &){}
            #else
                template<class impl> requires 
                can_random_access<impl, item_t>
                ranger(impl const & imp) {
                    if constexpr (inc::is_based_on<data, impl>){
                        dat    = imp;
                        func   = inc::signature<item_t &, uxx>::check(& impl::operator[]);
                    }
                    else{
                        auto p = positive{ inc::addressof(imp), imp[0], 0, imp.length() };
                        dat    = p;
                        func   = inc::signature<item_t &, uxx>::check(& decltype(p)::operator[]);
                    }
                }
            #endif

            ranger(inc::initializer_list<item_t> list){
                auto p = special{ inc::addressof(list), list.begin()[0], 0, list.size() };
                dat    = p;
                func   = inc::signature<const item_t &, uxx>::check(& decltype(p)::operator[]);
            }

            item_t & operator[](uxx index){
                return inc::signature<item_t &, uxx>::call(& dat, func, index);
            }

            const item_t & operator[](uxx index) const {
                return inc::signature<item_t &, uxx>::call(& dat, func, index);
            }

            uxx length() const {
                return dat.len;
            }

            xranger(item_t);
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::interface_ranger::ranger;
}

#include"math/index_system.hpp"
