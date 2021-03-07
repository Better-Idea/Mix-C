// #define xa_name              find
// #define xa_find_mix_type     0/*从前往后找*/

template<class final_t, class base_t, class item_t>
struct meta : base_t {
    using base_t::base_t;
    using default_cmp_t = decltype(inc::default_compare<item_t>);

    template<
        class   func_t, 
        class   cmp_t   = default_cmp_t, 
        auto    mode_v  = inc::itr_detect_mode<func_t, uxx>
    >
    requires(
        inc::can_compare<cmp_t, item_t> and 
        mode_v != inc::itr_miss_match
    )
    uxx xa_name(
        item_t          value, 
        func_t  const & match, 
        cmp_t   const & compare = inc::default_compare<item_t>
    ) const {
        return inc::xa_name<item_t, mode_v>
            ::invoke(thex, final_t{xref value, 1}, match, compare);
    }

    template<
        class   func_t, 
        class   cmp_t   = default_cmp_t, 
        auto    mode_v  = inc::itr_detect_mode<func_t, uxx>
    >
    requires(
        inc::can_compare<cmp_t, item_t> and 
        mode_v != inc::itr_miss_match
    )
    uxx xa_name(
        final_t         value, 
        func_t  const & match, 
        cmp_t   const & compare = inc::default_compare<item_t>
    ) const {
        return inc::xa_name<item_t, mode_v>
            ::invoke(thex, value, match, compare);
    }

    template<class cmp_t = default_cmp_t>
    requires(inc::can_compare<cmp_t, item_t>)
    uxx xa_name(
        item_t          value, 
        ixx             n_th, 
        cmp_t   const & compare = inc::default_compare<item_t>
    ) const {
        return inc::find_mix<item_t, xa_find_mix_type>
            ::invoke(thex, final_t{xref value, 1}, n_th, compare);
    }

    template<class cmp_t = default_cmp_t>
    requires(inc::can_compare<cmp_t, item_t>)
    uxx xa_name(
        final_t         value, 
        ixx             n_th, 
        cmp_t   const & compare = inc::default_compare<item_t>
    ) const {
        return inc::find_mix<item_t, xa_find_mix_type>
            ::invoke(thex, value, n_th, compare);
    }
};

#undef  xa_name
#undef  xa_find_mix_type
