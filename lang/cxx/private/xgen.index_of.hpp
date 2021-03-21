// #define xa_name         index_of_first

template<class final_t, class base_t, class item_t>
struct meta : base_t {
    using base_t::base_t;
    using the_t         = core<item_t>;
    using default_cmp_t = decltype(inc::default_compare<item_t>);

    template<class cmp_t = default_cmp_t>
    requires(inc::can_compare<cmp_t, item_t>)
    uxx xa_name(item_t value, cmp_t const & compare = inc::default_compare<item_t>) const {
        return the.xlink2(xa_name, _char)(final_t{xref value, 1}, compare);
    }

    template<
        class seq_t         = inc::initializer_list<item_t>,
        class cmp_t         = default_cmp_t, 
        class seq_item_t    = inc::item_origin_of<seq_t>
    >
    requires(
        inc::can_unified_seqlize<seq_t> and 
        inc::has_cast<item_t, seq_item_t> and
        inc::has_cast<final_t, seq_t> == false
    )
    uxx xa_name(seq_t const & char_seq, cmp_t const & compare = inc::default_compare<item_t>) const {
        return the.xlink2(xa_name, _char)(inc::unified_seq<seq_t>{char_seq}, compare);
    }

    template<class cmp_t = default_cmp_t>
    requires(inc::can_compare<cmp_t, item_t>)
    uxx xa_name(final_t value, cmp_t const & compare = inc::default_compare<item_t>) const {
        return the.xa_name(value, compare);
    }
};

#undef  xa_name
