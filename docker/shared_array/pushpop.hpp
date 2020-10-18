#ifndef xusing_docker_shared_array
#include"docker/private/shared_array.hpp"
#endif

#ifndef xpack_docker_shared_array_pushpop
#define xpack_docker_shared_array_pushpop
#pragma push_macro("xuser")
#pragma push_macro("xusing_docker_shared_array")
#undef  xuser
#define xuser mixc::docker_shared_array_pushpop
#include"define/base_type.hpp"
#include"docker/shared_array.hpp"
#include"docker/transmitter.hpp"
#include"dumb/mirror.hpp"
#include"macro/xstruct.hpp"
#include"memory/new.hpp"
#include"memop/addressof.hpp"
#include"memop/cast.hpp"
#include"memop/copy.hpp"
#pragma pop_macro("xusing_docker_shared_array")
#pragma pop_macro("xuser")

namespace mixc::docker_shared_array_pushpop {
    template<class base_t> struct extern_fields;
    template<> 
    xstruct(
        xspec(extern_fields, void),
        xpubf(count, uxx)
    )
        extern_fields() : 
            count(0){}
    $

    template<class base_t>
    xstruct(
        xtmpl(extern_fields, base_t),
        xpubb(base_t),
        xpubf(count, uxx)
    )
        extern_fields() : 
            count(0), base_t() {}
    $

    template<class item_t, uxx rank, class attribute>
    struct core : inc::shared_array<item_t, rank, extern_fields<attribute>> {
        using the_t                  = inc::shared_array<item_t, rank, extern_fields<attribute>>;
        using mirror_item_t          = inc::mirror<item_t>;
        using mirror_without_destroy = inc::shared_array<mirror_item_t, rank, extern_fields<attribute>>;
        using the_t::the_t;

        uxx length() const {
            return the->count;
        }

        uxx capacity() const {
            return the.length();
        }

        bool is_empty() const {
            return length() == 0;
        }

        void clear() {
            the_t old_array;
            the.swap(xref old_array);

            for(uxx i = 0; i < old_array->count; i++) {
                old_array[i].~item_t();
            }
            inc::cast<mirror_without_destroy>(old_array).~mirror_without_destroy();
        }

        void push(item_t const & value) {
            if (the->count == capacity()) {
                u08 mirror[sizeof(the_t)];

                auto & new_array = * new(mirror) the_t(
                    ::length(capacity() * 2 + 1)
                );

                inc::copy(new_array, the, length());
                the.swap(xref new_array);
                the->count = new_array->count;
                inc::cast<mirror_without_destroy>(new_array).~mirror_without_destroy();
            }

            auto cur = xref the[the->count++];
            new (cur)item_t(value);
        }

        inc::transmitter<item_t> pop() {
            the->count -= 1;
            inc::transmitter<item_t> r = the[the->count];
            the[the->count].~item_t();
            return r;
        }
    };
}
#endif

namespace mixc::docker_shared_array_pushpop::xuser {
    template<class final, class item_t, uxx rank, class attribute>
    using shared_array = inc::adapter_pushpop<final, core<item_t, rank, attribute>, item_t>;
}

#undef  xusing_docker_shared_array  
#define xusing_docker_shared_array  ::mixc::docker_shared_array_pushpop::xuser
