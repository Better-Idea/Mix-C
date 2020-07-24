#ifndef xpack_gc_private_token
#define xpack_gc_private_token
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_token
#include"dumb/struct_type.hpp"
#include"lock/atom_add.hpp"
#include"lock/atom_or.hpp"
#include"lock/atom_sub.hpp"
#include"macro/xtypeid.hpp"
#include"meta/is_same.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_tuple{
    template<class root_t> struct tuple;
}

namespace mixc::gc_ref{
    template<class impl, class a, class b, bool is_array> struct meta;
}

namespace mixc::macro_xsv{
    template<class item> struct static_string_holder;
}

namespace mixc::memory_alloctor::origin{
    struct memory_size;
    template<class t> void free_with_destroy(t *, memory_size);
}

namespace mixc::gc_private_token::origin{
    constexpr uxx step = uxx(1);

    xstruct(
        xname(token)
    )
        token(uxx) : record(step) { }
    protected:
        template<class, class, class, bool> friend struct mixc::gc_ref::meta;
        template<class> friend struct mixc::gc_private_tuple::tuple;

        constexpr uxx  this_length() { return uxx(0); }
        constexpr void this_length(uxx) { }

        mutable uxx record;

        uxx owners() const {
            return record;
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
        uxx length;

        token_plus(uxx length) : 
            token(0), length(length) { }

        uxx this_length() {
            return length;
        }

        void this_length(uxx value) {
            length = value;
        }
    $

    template<class type, class attribute, class addition>
    struct token_mix;

    template<class type, class addition>
    xstruct(
        xspec(token_mix, type, void, addition),
        xprob(addition)
    )
        using addition::addition;

        token_mix(token_mix const &)        = delete;
        void operator=(token_mix const &)   = delete;
    protected:
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

        template<class impl, class a, class b, bool is_array> friend struct mixc::gc_ref::meta;
        template<class t> friend void mixc::memory_alloctor::origin::free_with_destroy(t *, mixc::memory_alloctor::origin::memory_size);
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

        template<class impl, class a, class b, bool is_array> friend struct mixc::gc_ref::meta;
        template<class t> friend void mixc::memory_alloctor::origin::free_with_destroy(t *, mixc::memory_alloctor::origin::memory_size);
    $
}

#endif

namespace xuser::inc{
    using namespace ::mixc::gc_private_token::origin;
}
