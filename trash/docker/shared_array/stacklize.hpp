#ifndef xusing_docker_shared_array
#include"docker/private/shared_array.hpp"
#endif

#ifndef xpack_docker_shared_array_stacklize
#define xpack_docker_shared_array_stacklize
#pragma push_macro("xuser")
#pragma push_macro("xusing_docker_shared_array")
#undef  xuser
#define xuser mixc::docker_shared_array_stacklize::inc
#include"define/base_type.hpp"
#include"docker/shared_array.hpp"
#include"docker/transmitter.hpp"
#include"dumb/mirror.hpp"
#include"macro/xexport.hpp"
#include"macro/xnew.hpp"
#include"macro/xref.hpp"
#include"memop/cast.hpp"
#include"memop/copy.hpp"
#pragma pop_macro("xusing_docker_shared_array")
#pragma pop_macro("xuser")

namespace mixc::docker_shared_array_stacklize::origin {
    using namespace xusing_docker_shared_array::origin;
}

namespace mixc::docker_shared_array_stacklize {
    template<class final_t, class type, uxx rank, class attribute>
    xstruct(
        xtmpl(shared_array, final_t, type, rank, attribute),
        xpubb(inc::shared_array<type, rank, attribute, true /* is_binary_aligned_alloc */>)
    )
        using base_t                 = inc::shared_array<type, rank, attribute, true /* is_binary_aligned_alloc */>;
        using mirror_item_t          = inc::mirror<type>;
        using mirror_without_destroy = inc::shared_array<mirror_item_t, rank, attribute, true>;
        using item_t                 = typename base_t::item_t;
        using base_t::base_t;

        void clear() {
            the_t old_array;
            base_t::swap(xref(old_array));

            if (not old_array.is_empty()){
                for(uxx i = 0; i < old_array.length(); i++) {
                    old_array[i].~item_t();
                }
                inc::cast<mirror_without_destroy>(old_array).~mirror_without_destroy();
            }
        }

        void push(item_t const & value) {
            uxx len = the.length();

            if (len == 0 or (len & (len + 1))) {
                the.length(len + 1);
            }
            // 到达容量边界
            // length       capacity
            // 0        ->  1 
            // 1        ->  2
            // 2        ->  4
            // 3        ->  4
            // 4        ->  8
            // 5        ->  8
            // 6        ->  8
            // ...
            else{
                u08 mirror[sizeof(the_t)];

                the_t & new_array = * xnew(mirror) the_t(
                    ::length{len + 1} // 由于设置了 is_binary_aligned_alloc=true， 所以内部默认为分配容量为 capacity() * 2 的内存
                );

                inc::copy(new_array, the, len);
                the.swap(xref(new_array));
                inc::cast<mirror_without_destroy>(new_array).~mirror_without_destroy();
            }

            item_t * cur = xref(the[len]);
            xnew (cur)item_t(value);
        }

        inc::transmitter<item_t> pop() {
            uxx                      i = the.length() - 1;
            inc::transmitter<item_t> r = the[i];

            // 到达容量边界开始缩容
            // 2^n & (2^n - 1) -> 0
            if ((i & (i - 1)) == 0){
                the_t new_array{
                    ::length(i)
                };

                inc::copy(new_array, the, i);
                the.swap(xref(new_array));
            }
            else{
                the[i].~item_t();
                the.length(i);
            }
            return r;
        }

        xpubgetx(capacity, uxx){
            return base_t::capacity();
        }
    $
}

#endif

#undef  xusing_docker_shared_array  
#define xdocker_shared_array_disable_is_binary_aligned_alloc
#define xusing_docker_shared_array  ::mixc::docker_shared_array_stacklize
