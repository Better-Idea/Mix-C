#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_split
#define xpack_lang_cxx_split
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_split::inc
#include"algo/mmu.hpp"
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"dumb/move.hpp"
#include"interface/can_callback.hpp"
#include"interface/can_compare.hpp"
#include"lang/cxx/find.hpp"
#include"lang/cxx.hpp"
#include"macro/xstruct.hpp"
#include"memop/copy.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_split{
    xstruct(
        xname(cxx_split_item),
        xprif(pindex, uxx),
        xprif(plength, uxx)
    )
        using final_t = the_t;
        template<class> friend struct core;

        constexpr cxx_split_item():
            pindex(0),
            plength(0){
        }

        xpubget(index)
        xpubget(length)
    $

    enum class keep_empty_entries_t{
        remove_empty_entries,
        keep_empty_entries,
    };

    constexpr uxx                   begin_length        = 128;
    constexpr keep_empty_entries_t  keep_empty_entries
         = keep_empty_entries_t::keep_empty_entries;

    using var_array =
        inc::var_array<begin_length/*起始长度*/, inc::with_fixed_page_table>;

    template<class item_t>
    xstruct(
        xtmpl(cxx_split_info, item_t),
        xpubb(inc::disable_copy),
        xprif(pmatch_count, uxx),
        xprif(psegment_count, uxx),
        xprif(ptable, cxx_split_item **),
        xprif(pcontent, item_t const *)
    )
    private:
        using final_t = the_t;
        template<class> friend struct core;

        constexpr cxx_split_info():
            pmatch_count(0),
            psegment_count(0),
            ptable(nullptr),
            pcontent(nullptr){
        }

    public:
        cxx_split_info(cxx_split_info && self){
            inc::copy(xref the, self);
        }

        xpubget(match_count)
        xpubget(segment_count)
        xpubgetx(empty_entries_count, uxx){
            return pmatch_count - psegment_count;
        }
    public:
        cxx_split_item segment_of(uxx index) const {
            return var_array::access(ptable, index);
        }

        inc::cxx<item_t> segment_content_of(uxx index) const {
            auto item = var_array::access(ptable, index);
            return { pcontent + item.index(), item.length() };
        }
    $

    template<class item_t>
    struct core {
        using the_t = inc::cxx<item_t>;

        template<class func_t, class cmp_t>
        auto split(
            the_t                   value, 
            keep_empty_entries_t    keep_empty_entries,
            cmp_t           const & compare,
            func_t          const & invoke
        ) const {
            using csi           = cxx_split_item;
            csi                     last{};
            cxx_split_info<item_t>  info{};

            // 使用 u08 数组代替 csi 数组避免不必要的构造初始化
            u08     buffer[sizeof(csi) * begin_length];
            csi  *  table[40];
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

            if (info.ptable = table; uxx(keep_empty_entries)){
                info.pmatch_count   = the.find(value, compare, [&](uxx index){
                    last.plength    = index - last.pindex;
                    var_array::push(xref info.ptable, xref info.psegment_count, last, allocx, freex);

                    if (index == 0){
                        last.pindex = index + value.length();
                        return;
                    }

                    last.pindex     = index;
                    last.plength    = 0;
                    var_array::push(xref info.ptable, xref info.psegment_count, last, allocx, freex);
                    last.pindex    += value.length();
                });
            }
            else{
                info.pmatch_count   = the.find(value, compare, [&](uxx index){
                    if (last.plength = index - last.pindex; last.plength != 0){
                        var_array::push(xref info.ptable, xref info.psegment_count, last, allocx, freex);
                    }
                    last.pindex     = index + value.length();
                });
            }

            if (last.pindex + 1 < the.length()){
                last.plength        = the.length() - last.pindex;
                var_array::push(xref info.ptable, xref info.psegment_count, last, allocx, freex);
            }

            info.pcontent           = (item_t *)the;
            invoke(inc::move(info));
            var_array::clear(xref info.ptable, xref info.pmatch_count, freex);
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;

        template<class func_t>
        static constexpr bool is_cxx_split_callback =
            inc::can_callback<func_t, void(cxx_split_info<item_t> const &)> or
            inc::can_callback<func_t, void(cxx_split_info<item_t> &&)> or
            inc::can_callback<func_t, void(cxx_split_info<item_t>)>;

        template<class func_t>
        requires(is_cxx_split_callback<func_t>)
        void split(final_t value, func_t const & invoke) const {
            the.split(
                value, 
                keep_empty_entries_t::remove_empty_entries, 
                inc::default_compare<item_t>, 
                invoke
            );
        }

        template<class func_t, class cmp_t>
        requires(
            is_cxx_split_callback<func_t> and
            inc::can_compare<cmp_t, item_t>
        )
        void split(
            final_t                 value,
            cmp_t   const &         compare, 
            func_t  const &         invoke
        ) const {
            the.split(
                value, 
                keep_empty_entries_t::remove_empty_entries,
                compare, 
                invoke
            );
        }

        template<class func_t>
        requires(is_cxx_split_callback<func_t>)
        void split(
            final_t                 value,
            keep_empty_entries_t    keep_empty_entries,
            func_t  const &         invoke
        ) const {
            the.split(
                value, 
                keep_empty_entries,
                inc::default_compare<item_t>, 
                invoke
            );
        }

        template<class func_t, class cmp_t>
        requires(
            is_cxx_split_callback<func_t> and
            inc::can_compare<cmp_t, item_t>
        )
        void split(
            final_t                 value,
            keep_empty_entries_t    keep_empty_entries,
            cmp_t   const &         compare,
            func_t  const &         invoke
        ) const {
            the.split(
                value, 
                keep_empty_entries,
                compare, 
                invoke
            );
        }
    };
}

namespace mixc::lang_cxx_split::origin{
    using mixc::lang_cxx_split::cxx_split_item;
    using mixc::lang_cxx_split::cxx_split_info;
    using mixc::lang_cxx_split::keep_empty_entries;

    using c08_split_item = cxx_split_item;
    using c08_split_info = cxx_split_info<char>;
    using c16_split_item = cxx_split_item;
    using c16_split_info = cxx_split_info<char16_t>;
}

#endif

namespace mixc::lang_cxx_split::xuser{
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

xexport_space(mixc::lang_cxx_split::origin)

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_split::xuser
