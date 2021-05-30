#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_logger_helper
#define xpack_lang_cxx_logger_helper
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_logger_helper::inc
#include"interface/can_alloc.hpp"
#include"lang/cxx/strlize.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#include"memop/cast.hpp"
#include"memop/copy.hpp"
#include"math/min.hpp"
#include"meta/has_cast.hpp"
#include"meta/is_base_char.hpp"
#include"meta/is_cxx.hpp"
#include"meta/is_float.hpp"
#include"meta/is_same.hpp"
#include"meta/more_fit.hpp"
#include"meta_seq/tget.hpp"
#include"meta_seq/tlist.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_logger_helper{
    template<class item_t>
    struct mix{
        template<class object_t>
        mix(object_t const & value){
            using result = inc::more_fit<
                object_t, i08, i16, i32, i64, u08, u16, u32, u64, bool, f32, f64, asciis, voidp, char, inc::ialloc<item_t>
            >;
            using match = typename result::type;
            static_assert(inc::is_cxx<object_t> or result::index != not_exist);

            if constexpr (inc::is_same<match, inc::ialloc<item_t>>){
                return; // pass
            }
            else if constexpr (inc::is_same<match, asciis> or inc::is_cxx<object_t>){
                auto   item             = (inc::cxx<item_t>{ value });
                auto & target           = (inc::cast<match>(this->value));
                this->length            = (item.length());
                this->fmt               = (xref mix::template logger_helper<match>);
                target                  = (asciis(item));
            }
            else{
                auto & target           = (inc::cast<match>(this->value));
                this->fmt               = (xref mix::template logger_helper<match>);
                target                  = (match)(object_t &)(value);
            }

        }

        inc::cxx<item_t> logger_helper(inc::ialloc<item_t> alloc) const {
            return this->fmt(this, alloc);
        }

        bool is_string() const {
            return this->length != 0;
        }

        operator inc::cxx<item_t>() const {
            return inc::cxx<item_t>{
                inc::cast<item_t *>(this->value), this->length
            };
        }
    private:
        template<class object_t>
        static inc::cxx<item_t> logger_helper(mix const * self, inc::ialloc<char> alloc){
            if constexpr (inc::is_float<object_t>){
                return inc::cxx<item_t>(
                    inc::cast<object_t>(self->value), inc::float_format_t::fmt_s1p2Es3, alloc
                );
            }
            else if constexpr (inc::is_same<asciis, object_t>){
                return inc::cxx<item_t>(
                    inc::cxx<item_t>(
                        inc::cast<object_t>(self->value), self->length
                    ), alloc
                );
            }
            else if constexpr (inc::is_same<voidp, object_t>) {
                return inc::cxx<item_t>(
                    inc::cast<uxx>(self->value), inc::numeration_t::hex, alloc
                );
            }
            else{
                return inc::cxx<item_t>(inc::cast<object_t>(self->value), alloc);
            }
        }

        using fmt_t             = inc::cxx<item_t>(*)(mix const * self, inc::ialloc<char> alloc);
        mutable u64     value   = 0;
        mutable fmt_t   fmt     = nullptr;
        mutable uxx     length  = 0;
    };

    template<class item_t>
    inline inc::cxx<item_t> logger_helper(
        inc::cxx<item_t>    message, 
        mix<item_t> const * items, 
        uxx                 items_length,
        inc::ialloc<item_t> alloc
    ){
        using cxx = inc::cxx<item_t>;
        item_t * table[32];
        item_t   bufx[128];
        item_t   bufi[128];
        auto   inital_size = sizeof(bufx) / sizeof(bufx[0]);
        auto   line_size = inital_size;
        auto   current = bufx;
        auto   i_tab = uxx(0);
        auto   total_length = uxx(0);
        auto   end = bufx + line_size;
        auto   push = [&](auto value, uxx length){
            auto ptr                    = asciis(nullptr);
            auto rest_buffer            = uxx(end - current);
            auto can_push               = inc::min(rest_buffer, length);
            auto rest_push              = length - can_push;

            if constexpr (inc::is_base_char<decltype(value)>){
                ptr                     = xref value;
            }
            else{
                ptr                     = value;
            }

            inc::copy_with_operator_unsafe(current, ptr, can_push);

            if (total_length += length; rest_push == 0){
                current                += length;
                return;
            }

            table[i_tab += 1]           = inc::alloc<item_t>(inc::memory_size{ line_size });
            table[i_tab + 1]            = nullptr;
            current                     = table[i_tab];
            end                         = current + line_size;
            line_size                  *= 2;
            inc::copy_with_operator_unsafe(current, xref ptr[can_push], rest_push);
            current                    += rest_push;
        };

        table[0]                        = bufx;
        table[1]                        = nullptr;

        for (auto finish = false; not finish;) {
            // quotes_first 和 label_first 只有一个为 true
            // 指示当前元素是否是纯字符串
            bool quotes_first           = false;
            bool label_first            = false;
            bool in_origin_text         = false;
            uxx  brackets               = 0;
            uxx  one_char               = 1;

            do {
                auto item               = message[0];

                if (message = message.backward(1); not in_origin_text){
                    switch(push(item, one_char); item){
                    case '(':  brackets++;  break;
                    case ')':  brackets--;  break;
                    case '\"': 
                        quotes_first    = not label_first; 
                        in_origin_text  = true;
                        break;
                    default:
                        label_first     = label_first or (item != ' ' and not quotes_first);
                        break;
                    }
                }
                else {
                    switch(item){
                    case '\\':
                        push('\\', 1);
                        push(message[0], one_char);
                        message = message.backward(1);
                        break;
                    case '\"':
                        push('\"', one_char);
                        in_origin_text  = false;
                        break;
                    default:
                        push(item, one_char);
                        break;
                    }
                }

                if (message.length() == 0) {
                    finish              = true;
                    break;
                }

                // message 表达式可能包含函数调用，小括号间包含个 ',' 分隔符
                // 例如：message = "a, b, c.call(a, \"hello\", c),"
                // 但需要下列表达式同时满足时才表示结束语义
            }while(message[0] != ',' or brackets != 0 or in_origin_text);

            if (message = message.backward(1); quotes_first){
                continue;
            }

            bool need_copy              = true;
            bool need_free              = false;
            cxx  str;

            if (push(':', one_char); not items->is_string()) {
                str = items->logger_helper([&](uxx length){
                    if (auto rest = uxx(end - current); rest >= length){
                        auto buffer     = current;
                        need_copy       = false;
                        current        += length;
                        return buffer;
                    }
                    if (sizeof(bufi) / sizeof(bufi[0]) < length){
                        need_free       = true;
                        return inc::alloc<item_t>(inc::memory_size{ length });
                    }
                    else{
                        return bufi;
                    }
                });
            }
            else{
                str                     = items[0];
            }

            if (need_copy){
                if (items->is_string()) {
                    push('\"', one_char);
                }
                if (push(asciis(str), str.length()); need_free){
                    inc::free(asciis(str), inc::memory_size{ str.length() });
                }
                if (items->is_string()) {
                    push('\"', one_char);
                }
            }

            items                      += 1;
        }

        auto buffer                     = alloc(total_length);
        auto result                     = buffer;
        auto last_size                  = inital_size;
        line_size                       = inital_size;

        for(uxx i = 0; i <= i_tab; i++){
            if (last_size = line_size; i != i_tab){
                inc::copy_with_operator_unsafe(buffer, table[i], line_size);
                buffer                 += line_size;
                line_size               = inital_size;
                inital_size            *= 2;
            }
            else{
                inc::copy_with_operator_unsafe(buffer, table[i], current - table[i]);
            }

            if (i != 0){
                inc::free(table[i], inc::memory_size{last_size});
            }
        }
        return cxx{ result, total_length };
    }

    template<class last_t>
    inline decltype(auto) get_last(last_t const & last){
        return last;
    }

    template<class first_t, class ... args_t>
    inline decltype(auto) get_last(first_t const & first, args_t const & ... args){
        return get_last(args...);
    }

    template<class final_t, class base_t, class item_t>
    struct meta : base_t{
        using base_t::base_t;
        using the_t = inc::cxx<item_t>;

        template<class ... args_t>
        requires(
            sizeof...(args_t) >= 2 and 

            // 要求最后一个参数满足 ialloc<T> 约束
            inc::has_cast<
                inc::ialloc<
                    item_t
                >,
                inc::tget<
                    inc::tlist<args_t...>, -1/*最后一个*/
                >
            >
        )
        final_t logger_helper(args_t const & ... args){
            auto & alloc = 
                ::mixc::lang_cxx_logger_helper::get_last(args...);
                ::mixc::lang_cxx_logger_helper::mix<item_t> list[] = { args... };
            return 
                ::mixc::lang_cxx_logger_helper::template logger_helper<item_t>(
                    the, list, sizeof...(args_t) - 1, alloc
                );
        }
    };
}

#endif

namespace mixc::lang_cxx_logger_helper::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_logger_helper::xuser
