// #define xa_args_list
// #define xa_invoke
// #define xa_is_friend
// #define xa_is_const
// #define xa_is_outside

#ifndef xa_args_list
    #error "miss macro 'xa_args_list'"
#endif

#ifndef xa_invoke
    #error "miss macro 'xa_invoke'"
#endif

#ifndef xa_is_friend
    #define xa_is_friend
#else
    #undef  xa_is_friend
    #define xa_is_friend   friend
#endif

#ifndef xa_is_const
    #define xa_is_const
#endif

#ifndef xa_is_outside
    #define xa_is_outside(op)                                                           \
        xa_is_friend inline bool operator op (xa_args_list) xa_is_const {               \
            return (xa_invoke) op 0;                                                    \
        }
#else
    #undef  xa_is_outside
    #define xa_is_outside(op)                                                           \
        xa_is_friend inline bool operator op (xa_args_list) xa_is_const;
#endif

xa_is_outside(>)
xa_is_outside(>=)
xa_is_outside(<)
xa_is_outside(<=)
xa_is_outside(==)
xa_is_outside(!=)

#undef  xa_args_list
#undef  xa_invoke
#undef  xa_is_friend
#undef  xa_is_const
#undef  xa_is_outside