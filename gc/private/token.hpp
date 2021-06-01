#ifndef xpack_gc_private_token
#define xpack_gc_private_token
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_token::inc
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_fetch_and.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"define/base_type.hpp"
#include"dumb/struct_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_token::origin{
    constexpr uxx shift_to_get_owners   = 1;
    constexpr uxx step                  = uxx(1 << shift_to_get_owners);
    constexpr uxx bit_in_gc_queue       = 1 << 0;

    xstruct(
        xname(token),
        xprof(m_record, uxx)
    )
        token(uxx) : m_record(step) { }

        constexpr uxx  this_length() const { return uxx(0); }
        constexpr void this_length(uxx) const { }

        bool in_gc_queue() const {
            return inc::atom_load(& m_record) & bit_in_gc_queue;
        }

        bool in_gc_queue(bool value) {
            if (value){
                return (inc::atom_fetch_or(& m_record, bit_in_gc_queue) & bit_in_gc_queue) != 0;
            }
            else{
                return (inc::atom_fetch_and(& m_record, ~bit_in_gc_queue) & bit_in_gc_queue) != 0;
            }
        }

        uxx owners() const {
            return inc::atom_load(& m_record) >> shift_to_get_owners;
        }

        uxx owners_increase() const {
            return inc::atom_add(& m_record, step) >> shift_to_get_owners;
        }

        uxx owners_decrease() const {
            return inc::atom_sub(& m_record, step) >> shift_to_get_owners;
        }
    $

    xstruct(
        xname(token_plus),
        xpubb(token),
        xpubf(m_length, uxx)
    )
        token_plus(uxx length) : 
            token(0), m_length(length) { }

        uxx this_length() const {
            return m_length;
        }

        void this_length(uxx value) const {
            m_length = value;
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