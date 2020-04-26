#ifndef xusing_docker_darray
	#include"docker/private/darray.hpp"
#endif

#ifndef xpack_docker_darray_pushpop
#define xpack_docker_darray_pushpop
	#pragma push_macro("xuser")
	#pragma push_macro("xusing_docker_darray")
		#undef  xuser
		#define xuser mixc::docker_darray_pushpop
		#include"define/base_type.hpp"
		#include"docker/darray.hpp"
        #include"docker/transmitter.hpp"
        #include"docker/private/pushpop_t.hpp"
        #include"dumb/mirror.hpp"
        #include"memory/new.hpp"
        #include"memop/addressof.hpp"
        #include"memop/cast.hpp"
        #include"memop/copy.hpp"
	#pragma pop_macro("xusing_docker_darray")
	#pragma pop_macro("xuser")

	namespace mixc::docker_darray_pushpop {
        template<class base_t> struct extern_fields;
        template<> 
        struct extern_fields<void>{
            xgc_fields(
                xiam(extern_fields<void>),
                xpub(count, uxx)
            );
        public:
            extern_fields() : 
                count(0){}
        };

        template<class base_t>
        struct extern_fields : base_t {
            xgc_fields(
                xiam(extern_fields<base_t>),
                xpub(count, uxx)
            );
        public:
            extern_fields() : 
                count(0), base_t() {}
        };

		template<class item_t, uxx rank, class attribute>
		struct core : inc::darray<item_t, rank, extern_fields<attribute>> {
			using the_t                  = inc::darray<item_t, rank, extern_fields<attribute>>;
            using mirror_item_t          = inc::mirror<item_t>;
            using mirror_without_destroy = inc::darray<mirror_item_t, rank, extern_fields<attribute>>;
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
                        inc::length(capacity() * 2 + 1)
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

namespace mixc::docker_darray_pushpop::xuser {
	template<class final, class item_t, uxx rank, class attribute>
    using darray = inc::pushpop_t<final, core<item_t, rank, attribute>, item_t>;
}

#undef  xusing_docker_darray  
#define xusing_docker_darray  ::mixc::docker_darray_pushpop::xuser
