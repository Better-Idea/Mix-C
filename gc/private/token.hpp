#ifndef xpack_gc_private_token
#define xpack_gc_private_token
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_token::inc
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"dumb/struct_type.hpp"
#include"meta/is_same.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_token::origin{
    constexpr uxx step = uxx(1);

    xstruct(
        xname(token),
        xprof(record, uxx)
    )
        token(uxx) : record(step) { }

        constexpr uxx  this_length() const { return uxx(0); }
        constexpr void this_length(uxx) const { }

        uxx owners() const {
            return inc::atom_load(& record);
        }

        uxx owners_inc() const {
            return inc::atom_add(& record, step);
        }

        uxx owners_dec() const {
            return inc::atom_sub(& record, step);
        }
    $

    xstruct(
        xname(token_plus),
        xpubb(token)
    )
        mutable uxx length;

        token_plus(uxx length) : 
            token(0), length(length) { }

        uxx this_length() const {
            return length;
        }

        void this_length(uxx value) const {
            length = value;
        }
    $

    template<class type, class attribute, class addition>
    struct token_mix;

    template<class type, class addition>
    xstruct(
        xspec(token_mix, type, void, addition),
        xpubb(addition)
    )
        using base_t = addition;
        using base_t::base_t;

        token_mix(token_mix const &)        = delete;
        void operator=(token_mix const &)   = delete;

        type * item_ptr(uxx index){
            if constexpr (inc::is_same<void, type>){
                return nullptr;
            }
            else{
                return ((type *)(this + 1)) + index;
            }
        }

        ~token_mix() {
            if constexpr (not inc::is_same<void, type>){
                for(uxx i = 0; i < addition::this_length(); i++) {
                    item_ptr(i)->~type();
                }
            }
        }
    $

    template<class type, class attribute, class addition>
    xstruct(
        xtmpl(token_mix, type, attribute, addition),
        xpubb(token_mix<type, void, addition>), 
        xpubb(inc::struct_type<attribute>)
    )
        using base_t = token_mix<type, void, addition>;

        template<class ... args>
        token_mix(uxx length, args const & ... list) : 
            base_t(length), inc::struct_type<attribute>(list...) {}

        attribute * attribute_ptr() const {
            return (attribute *)(inc::struct_type<attribute> *)this;
        }
    $
}

#endif

xexport_space(mixc::gc_private_token::origin)