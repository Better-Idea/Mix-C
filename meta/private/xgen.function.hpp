#ifndef xa_has_object
    #define xa_object_t
    #define xa_object
#else
    #undef  xa_has_object
    #define xa_object_t         class object_t,
    #define xa_object           (object_t::*)
#endif

#ifndef xa_has_const
    #define xa_has_const
#else
    #undef  xa_has_const
    #define xa_has_const        const
#endif

template<xa_object_t class ret_t, class ... args_t>
struct function<ret_t xa_object (args_t...) xa_has_const> {
    static constexpr uxx args_count = sizeof...(args_t);

    using return_type = ret_t;

    template<ixx index>
    using args = inc::tget<inc::tlist<args_t...>, index>;
};

#undef  xa_object_t
#undef  xa_object
#undef  xa_has_const
