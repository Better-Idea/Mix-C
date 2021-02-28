#ifndef xpack_gc_private_token
#define xpack_gc_private_token
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_token::inc
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_sub.hpp"
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

    template<class type_t, class attribute_t, class addition_t>
    struct token_mix;

    template<class type_t, class addition_t>
    xstruct(
        xspec(token_mix, type_t, void, addition_t),
        xpubb(addition_t)
    )
        using base_t = addition_t;
        using base_t::base_t;

        token_mix(token_mix const &)        = delete;
        void operator=(token_mix const &)   = delete;

        type_t * item_ptr(uxx index){
            if constexpr (inc::is_same<void, type_t>){
                return nullptr;
            }
            else{
                return ((type_t *)(this + 1)) + index;
            }
        }

        ~token_mix() {
            if constexpr (not inc::is_same<void, type_t>){
                for(uxx i = 0; i < addition_t::this_length(); i++) {
                    item_ptr(i)->~type_t();
                }
            }
        }
    $

    template<class type_t, class attribute_t, class addition_t>
    xstruct(
        xtmpl(token_mix, type_t, attribute_t, addition_t),
        xpubb(token_mix<type_t, void, addition_t>), 
        xpubb(inc::struct_type<attribute_t>)
    )
        using base_t = token_mix<type_t, void, addition_t>;

        template<class ... args_t>
        token_mix(uxx length, args_t const & ... list) : 
            base_t(length), inc::struct_type<attribute_t>(list...) {}

        attribute_t * attribute_ptr() const {
            return (attribute_t *)(inc::struct_type<attribute_t> *)this;
        }
    $
}

#endif

xexport_space(mixc::gc_private_token::origin)