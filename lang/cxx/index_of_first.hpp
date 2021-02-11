#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_index_of_first
#define xpack_lang_cxx_index_of_first
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_index_of_first::inc
#include"interface/can_callback.hpp"
#include"interface/can_compare.hpp"
#include"interface/initializer_list.hpp"
#include"lang/cxx.hpp"
#include"mixc.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_index_of_first{
    template<class item_t>
    struct core : inc::cxx<item_t> {
        using base_t = inc::cxx<item_t>;
        using base_t::base_t;
        using the_t = core<item_t>;

        core(base_t const & self) : 
            base_t(self){}

        template<class cmp_t>
        uxx index_of_first(item_t const * value, uxx length, cmp_t const & compare) const {
            for(uxx i = 0; i < the.length(); i++){
                for(uxx ii = 0; ii < length; ii++){
                    if (compare(the[i], value[ii]) == 0){
                        return i;
                    }
                }
            }
            return not_exist;
        }

        template<class cmp_t>
        uxx index_of_first(item_t value, cmp_t const & compare) const {
            return index_of_first(xref value, 1, compare);
        }

        template<class compare_invoke>
        uxx index_of_first(the_t value, compare_invoke const & compare) const {
            the_t origin = the;
            uxx   miss   = 0;
            uxx   index;

            if (origin.length() < value.length() or value.length() == 0) {
                return not_exist;
            }
            while(true){
                if (index = origin.index_of_first(value[miss], compare); index == not_exist){
                    break;
                }
                if (origin = origin.forward(miss - index); origin.length() < value.length()) {
                    break;
                }
                for (index = 0; ; index++){
                    if (index == value.length()) {
                        return uxx(asciis(origin) - asciis(the));
                    }
                    if (compare(origin[index], value[index]) != 0) {
                        origin = origin.backward(miss);
                        miss = index;
                        break;
                    }
                }
            }
            return not_exist;
        }

        template<class call_t, class cmp_t>
        requires(
            inc::can_callback<call_t, void(uxx index)> and  // 此处保留 requires 约束以区分 index_of_first 重载
            inc::can_compare<cmp_t, item_t>
        )
        void index_of_first(the_t value, call_t const & match, cmp_t const & compare) const {
            for(auto cur = the;;){
                if (uxx i = cur.index_of_first(value, compare); i == not_exist){
                    return;
                }
                else{
                    match(asciis(cur) - asciis(the) + i);
                    cur = cur.backward(i + value.length());
                }
            }
        }
    };

    template<class final_t, class base, class item_t>
    struct meta : base {
        using base::base;
        using the_t         = core<item_t>;
        using default_cmp_t = decltype(inc::default_compare<item_t>);

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        uxx index_of_first(item_t value, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.index_of_first(& value, 1, compare);
        }

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        uxx index_of_first(inc::initializer_list<item_t> values, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.index_of_first(values.begin(), values.size(), compare);
        }

        template<class cmp_t = default_cmp_t>
        requires(
            inc::can_compare<cmp_t, item_t>
        )
        uxx index_of_first(final_t value, cmp_t const & compare = inc::default_compare<item_t>) const {
            return the.index_of_first(value, compare);
        }

        template<class call_t, class cmp_t = default_cmp_t>
        requires(
            inc::can_callback<call_t, void(uxx index)> and
            inc::can_compare<cmp_t, item_t>
        )
        void index_of_first(
            the_t           value, 
            call_t  const & match, 
            cmp_t   const & compare = inc::default_compare<item_t>) const {

            the.index_of_first(value, match, compare);
        }

        template<class call_t, class cmp_t = default_cmp_t>
        requires(
            inc::can_callback<call_t, void(uxx index)> and
            inc::can_compare<cmp_t, item_t>
        )
        void index_of_first(
            item_t          value, 
            call_t  const & match, 
            cmp_t   const & compare = inc::default_compare<item_t>) const {

            the.index_of_first(final_t{& value, 1}, match, compare);
        }
    };
}

#endif

namespace mixc::lang_cxx_index_of_first::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_index_of_first::xuser
