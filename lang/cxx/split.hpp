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
#include"interface/initializer_list.hpp"
#include"interface/unified_seq.hpp"
#include"lang/cxx/find.hpp"
#include"lang/cxx.hpp"
#include"macro/xstruct.hpp"
#include"memop/copy.hpp"
#include"memory/allocator.hpp"
#include"meta/has_cast.hpp"
#include"meta/is_cxx.hpp"
#include"meta/item_origin_of.hpp"
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
        xprif(psegment_count, uxx),
        xprif(pempty_entries_count, uxx),
        xprif(ptable, cxx_split_item **),
        xprif(pcontent, item_t const *)
    )
    private:
        using final_t = the_t;
        template<class> friend struct core;

        constexpr cxx_split_info():
            psegment_count(0),
            pempty_entries_count(0),
            ptable(nullptr),
            pcontent(nullptr){
        }

    public:
        cxx_split_info(cxx_split_info && self){
            inc::copy(xref the, self);
        }

        xpubget(segment_count)
        xpubget(empty_entries_count)
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

        template<class seq_t, class func_t, class cmp_t>
        auto split(
            seq_t           const & value, 
            keep_empty_entries_t    keep_empty_entries,
            cmp_t           const & compare,
            func_t          const & invoke
        ) const {
            using csi               = cxx_split_item;
            csi                     last{};
            cxx_split_info<item_t>  info{};

            // 使用 u08 数组代替 csi 数组避免不必要的构造初始化
            u08     buffer[sizeof(csi) * begin_length];
            csi  *  table[40];
            bool    once            = true;

            // 融合栈内存和堆内存
            auto allocx             = [&](uxx bytes) -> voidp {
                if (once){
                    once = false;
                    return buffer;
                }
                else{
                    return inc::alloc<void>(inc::memory_size{bytes});
                }
            };

            auto freex              = [&](voidp ptr, uxx bytes){
                if (ptr == voidp(buffer)){
                    return;
                }
                inc::free(ptr, inc::memory_size{bytes});
            };

            uxx length              = 1/*字符数组*/;

            if constexpr (inc::is_cxx<seq_t>){
                length              = value.length();
            }

            if (info.ptable = table; uxx(keep_empty_entries)){
                the.find(value, compare, [&](uxx index){
                    last.plength    = index - last.pindex;
                    var_array::push(xref info.ptable, xref info.psegment_count, last, allocx, freex);
                    info.pempty_entries_count
                                  += 1;

                    if (index == 0){
                        last.pindex = index + length;
                        return;
                    }

                    last.pindex     = index;
                    last.plength    = 0;
                    var_array::push(xref info.ptable, xref info.psegment_count, last, allocx, freex);
                    last.pindex    += length;
                });
            }
            else{
                the.find(value, compare, [&](uxx index){
                    if (last.plength = index - last.pindex; last.plength != 0){
                        var_array::push(xref info.ptable, xref info.psegment_count, last, allocx, freex);
                    }
                    info.pempty_entries_count
                                   += 1;
                    last.pindex     = index + length;
                });
            }

            if (last.pindex + 1 < the.length()){
                last.plength        = the.length() - last.pindex;
                var_array::push(xref info.ptable, xref info.psegment_count, last, allocx, freex);
            }

            info.pcontent           = (item_t *)the;
            invoke(inc::move(info));
            var_array::clear(xref info.ptable, xref info.psegment_count, freex);
        }
    };

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = core<item_t>;
    private:
        template<class func_t>
        static constexpr bool is_cxx_split_callback =
            inc::can_callback<func_t, void(cxx_split_info<item_t> const &)> or
            inc::can_callback<func_t, void(cxx_split_info<item_t> &&)> or
            inc::can_callback<func_t, void(cxx_split_info<item_t>)>;

        template<class seq_t>
        static constexpr bool is_char_seq = 
            inc::can_unified_seqlize<seq_t> and 
            inc::has_cast<item_t, inc::item_origin_of<seq_t>> and
            inc::is_cxx<seq_t> == false;

        template<class type_t>
        static constexpr bool is_pattern =
            is_char_seq<type_t> or inc::is_cxx<type_t>;
    public:
        template<class func_t, class type_t = inc::initializer_list<item_t>>
        requires(is_pattern<type_t> and is_cxx_split_callback<func_t>)
        void split(type_t const & value, func_t const & invoke) const {
            the.split(
                value, 
                keep_empty_entries_t::remove_empty_entries, 
                inc::default_compare<item_t>, 
                invoke
            );
        }

        template<
            class func_t, 
            class cmp_t, 
            class type_t = inc::initializer_list<item_t>
        >
        requires(
            is_pattern<type_t> and
            is_cxx_split_callback<func_t> and
            inc::can_compare<cmp_t, item_t>
        )
        void split(
            type_t  const &         value,
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

        template<class func_t, class type_t = inc::initializer_list<item_t>>
        requires(is_pattern<type_t> and is_cxx_split_callback<func_t>)
        void split(
            type_t  const &         value,
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

        template<
            class func_t, 
            class cmp_t, 
            class type_t = inc::initializer_list<item_t>
        >
        requires(
            is_pattern<type_t> and
            is_cxx_split_callback<func_t> and
            inc::can_compare<cmp_t, item_t>
        )
        void split(
            type_t  const &         value,
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
