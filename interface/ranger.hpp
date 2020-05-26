#ifndef xpack_interface_ranger
#define xpack_interface_ranger
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_ranger
        #include"configure.hpp"
        #include"define/base_type.hpp"
        #include"interface/initializer_list.hpp"
        #include"math/index_system.hpp"
        #include"memop/addressof.hpp"
        #include"memop/signature.hpp"
        #include"meta/has_cast.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::interface_ranger{
        enum : uxx{
            positive    = 0,
            negtive     = uxx(-1)
        };

        struct base{
            voidp   ptr = nullptr;
            voidp   itr = nullptr;
            uxx     ofs = 0;
            uxx     len = 0;
            uxx     msk = 0;

            base(){}

            template<class object>
            base(object * ptr, uxx ofs, uxx len, uxx msk) : 
                ptr(ptr),
                itr(convert(& object::operator[])),
                ofs(ofs),
                len(len),
                msk(msk){
            }

            template<class item_t>
            item_t & access(uxx index){
                return inc::signature<item_t &, uxx>::call(ptr, itr, (index ^ msk) + ofs);
            }

        private:
            template<class object, class item_t>
            static voidp convert(item_t & (object::* func)(uxx)){
                union{
                    decltype(func)  invoke;
                    voidp           call;
                } r { func };
                return r.call;
            }

            template<class object, class item_t>
            static voidp convert(item_t const & (object::* func)(uxx)){
                union{
                    decltype(func)  invoke;
                    voidp           call;
                } r { func };
                return r.call;
            }
        };

        template<class item_t>
        struct ranger{
        private:
            base    dat;
            xgc_fields(
                xiam(ranger<item_t>)
            );
        public:
            ranger(){}

            ranger(base impl) : 
                dat(impl){}


            template<class random_access_t> 
            #if not xfor_msvc_hint
            requires(
                inc::signature<item_t &, uxx>::has(& random_access_t::operator[]) and
                inc::signature<uxx>::has(& random_access_t::length) 
            )
            #endif
            ranger(random_access_t const & impl){
                dat = base(xref impl, 0, impl.length(), positive);
            }

            ranger(inc::initializer_list<item_t> impl){
                dat = base(xref impl, 0, impl.size(), positive);
            }

            uxx length() const {
                return dat.len;
            }

            item_t & operator[](uxx index){
                return dat.access<item_t>(index);
            }

            ranger<item_t> range(iinterval i){
                ranger<item_t> r = the;
                i.normalize(the.length());

                if (dat.msk == positive){ // 正序
                    if (i.left() <= i.right()){ // 正序
                        r.dat.ofs += i.left();
                        r.dat.len  = i.right() - i.left() + 1;
                        r.dat.msk  = positive;
                    }
                    else{ // 反序
                        r.dat.ofs += i.left();
                        r.dat.len  = i.left() - i.right() + 1;
                        r.dat.msk  = negtive;
                    }
                }
                else{ // 反序
                    if (i.left() <= i.right()){ // 反序 & 正序 -> 反序
                        r.dat.ofs -= i.left();
                        r.dat.len  = i.right() - i.left() + 1;
                        r.dat.msk  = negtive;
                    }
                    else{ // 反序 & 反序 -> 正序
                        r.dat.ofs -= i.left() + 1;
                        r.dat.len  = i.left() - i.right() + 1;
                        r.dat.msk  = positive;
                    }
                }
                return r;
            }
        };

        #define xranger(...)                                                        \
        inc::ranger<__VA_ARGS__> range(::mixc::iinterval i){                        \
            using namespace ::mixc::interface_ranger;                               \
            if (i.normalize(this->length());                                        \
                i.left() <= i.right()){                                             \
                return base(this, i.left(), i.right() - i.left() + 1, positive);    \
            }                                                                       \
            else{                                                                   \
                return base(this, i.left() + 1, i.left() - i.right() + 1, negtive); \
            }                                                                       \
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::interface_ranger::ranger;
}

#include"math/index_system.hpp"
