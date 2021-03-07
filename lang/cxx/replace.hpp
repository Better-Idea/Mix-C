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
#include"algo/mmu.hpp"
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"interface/can_callback.hpp"
#include"interface/can_compare.hpp"
#include"lang/cxx/find.hpp"
#include"lang/cxx.hpp"
#include"memop/copy.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_replace{
    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class cmp_t, class alloc_t>
        auto replace(the_t old_value, the_t new_value, cmp_t const & compare, alloc_t const & alloc) const {
            constexpr uxx buf_size = 128;
            using var           = inc::var_array<buf_size, inc::with_fixed_page_table>;

            uxx     buffer[buf_size];
            uxx *   table[40];
            uxx **  page_table  = table;
            uxx     count       = 0;
            uxx     last_index  = 0;
            bool    once        = true;

            // 融合栈内存和堆内存
            auto allocx = [&](uxx bytes) -> voidp {
                if (once){
                    once = false;
                    return buffer;
                }
                else{
                    return inc::alloc<void>(inc::memory_size{bytes});
                }
            };

            auto freex = [&](voidp ptr, uxx bytes){
                if (ptr == voidp(buffer)){
                    return;
                }
                inc::free(ptr, inc::memory_size{bytes});
            };

            the.find(old_value, [&](uxx index){
                var::push(& page_table, & count, index - last_index, allocx, freex);
                last_index      = index + old_value.length();
            }, compare);

            auto dis            = (new_value.length() - old_value.length());
            auto len_this       = (the.length());
            auto len_old        = (old_value.length());
            auto len_new        = (new_value.length());
            auto len            = (len_this + dis * count);
            auto buf            = (alloc(len));
            auto ptr            = (buf);
            auto ptr_origin     = (item_t *)the;
            auto ptr_new        = (item_t *)new_value;
            auto offset         = (uxx)0;
            auto copy_size      = (uxx)offset;

            for(uxx i = 0; i < count; i++){
                offset          = var::access(page_table, i);
                copy_size       = offset;
                inc::copy_with_operator(ptr, ptr_origin, copy_size);
                ptr            += offset;
                ptr_origin     += offset + len_old;

                if (len_new != 0){
                    inc::copy_with_operator(ptr, ptr_new, len_new);
                    ptr        += len_new;
                }
            }

            inc::copy_with_operator(ptr, ptr_origin, len_this - last_index);
            var::clear(& page_table, & count, freex);
            return the_t{ buf, len };
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;

        template<class alloc_t>
        requires(
            inc::can_alloc<alloc_t, item_t>
        )
        final_t replace(final_t old_value, final_t new_value, alloc_t const & alloc) const {
            return the.replace(old_value, new_value, inc::default_compare<item_t>, alloc);
        }

        template<class cmp_t, class alloc_t>
        requires(
            inc::can_compare<cmp_t, item_t> and
            inc::can_alloc<alloc_t, item_t>
        )
        final_t replace(
            final_t                   old_value, 
            final_t                   new_value, 
            cmp_t   const &         compare, 
            alloc_t const &         alloc) const {
            return the.replace(old_value, new_value, compare, alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_replace::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_replace::xuser
