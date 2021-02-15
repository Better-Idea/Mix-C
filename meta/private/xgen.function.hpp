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

template<xa_object_t class ret_type, class ... list>
struct function<ret_type xa_object (list...) xa_has_const> {
    static constexpr uxx args_count = sizeof...(list);

    using return_type = ret_type;

    template<ixx index>
    using args = inc::tget<inc::tlist<list...>, index>;
};

#undef  xa_object_t
#undef  xa_object
#undef  xa_has_const
