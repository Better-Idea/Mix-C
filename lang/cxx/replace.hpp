#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_replace
#define xpack_lang_cxx_replace
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_replace::inc
#include"define/base_type.hpp"
#include"docker/shared_array.hpp"
#include"interface/can_alloc.hpp"
#include"interface/can_callback.hpp"
#include"interface/can_compare.hpp"
#include"lang/cxx/index_of_first.hpp"
#include"lang/cxx.hpp"
#include"memop/copy.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_replace{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        static constexpr uxx buf_size = 64;

        template<class cmp_t, class alloc_t>
        requires(
            inc::can_compare<cmp_t, item_t> and
            inc::can_alloc<alloc_t, item_t>
        )
        auto replace(
            the_t                   old_value, 
            the_t                   new_value, 
            cmp_t   const &         compare, 
            alloc_t const &         alloc) const {

            struct {
                uxx              i = 0;
                uxx              buf[buf_size];
                uxx              total_length;
                uxx              dis;
                inc::shared_array<uxx> heap;

                void push(uxx value){
                    uxx index = i++;
                    
                    if (total_length += dis; index < buf_size){
                        buf[index] = value;
                        return;
                    }
                    if (index - buf_size - heap.length() == 0){
                        inc::shared_array<uxx> new_heap { ::length(index) };
                        inc::copy_with_operator(new_heap, heap, heap.length());
                        heap = new_heap;
                    }
                    heap[index - buf_size] = value;
                }
            } pack;

            pack.dis          = new_value.length() - old_value.length();
            pack.total_length = the.length();

            the.index_of_first(old_value, [&](uxx index){
                pack.push(index);
            }, compare);

            the_t r { alloc(pack.total_length), pack.total_length };
            the_t target     = r;
            the_t source     = the;
            bool  only_stack = pack.i < buf_size;
            uxx   top        = only_stack ? pack.i : buf_size;
            uxx   last_index = 0;


            auto && replace = [&](uxx index/*要移除字符串的索引*/){
                // 将 source 要移除字符串之前的保留字符串拷贝到 target
                auto 
                head_length = index - last_index;
                inc::copy_with_operator(target, source, head_length);
                target      = target.backward(head_length);
                head_length = head_length + old_value.length();
                source      = source.backward(head_length);
                inc::copy_with_operator(target, new_value, new_value.length());
                target      = target.backward(new_value.length());
                last_index  = index + old_value.length();
            };

            auto && remove = [&](uxx index){
                auto 
                head_length = index - last_index;
                inc::copy_with_operator(target, source, head_length);
                target      = target.backward(head_length);
                source      = source.backward(head_length + old_value.length());
                last_index  = index + old_value.length();
            };

            using change_t = inc::can_callback<void(uxx)>;

            auto && change = 
                new_value.is_empty() ? change_t(remove) : change_t(replace);

            for(uxx i = 0; i < top; i++){
                change(pack.buf[i]);
            }
            for(uxx i = 0; ixx(i) < ixx(pack.i - buf_size); i++){
                change(pack.heap[i]);
            }
            inc::copy_with_operator(target, source, source.length());
            return r;
        }
    };

    template<class final, class base, class item_t>
    struct meta : base{
        using base::base;
        using the_t = core<item_t>;

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final replace(final old_value, final new_value, alloc_t const & alloc) const {
            return the.replace(old_value, new_value, inc::default_compare<item_t>, alloc);
        }

        template<class cmp_t, class alloc_t>
        requires(
            inc::can_compare<cmp_t, item_t> and
            inc::can_alloc<alloc_t, item_t>
        )
        final replace(
            final                   old_value, 
            final                   new_value, 
            cmp_t   const &         compare, 
            alloc_t const &         alloc) const {
            return the.replace(old_value, new_value, compare, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_replace::xuser{
    template<class final, class item_t>
    using cxx = meta<final, xusing_lang_cxx::cxx<final, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_replace::xuser
