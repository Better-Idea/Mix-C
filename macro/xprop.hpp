#pragma once

namespace mixc::macro_xprop{
    template<class type>
    struct repeat{
        using item_t = type;
    };
}

#define __xget__(token_get,name,...)                \
token_get:                                          \
    final & name(__VA_ARGS__ * get) const {         \
        get[0] = (*(prop_ ## name *)this)();        \
        return thex;                                \
    }                                               \
    __VA_ARGS__ name() const {                      \
        return (*(prop_ ## name *)this)();          \
    }

#define __xset__(token_set,name,...)                \
token_set:                                          \
    final & name(__VA_ARGS__ set) {                 \
        (*(prop_ ## name *)this)(set);              \
        return thex;                                \
    }

#define __xprop__(token_get,token_set,name,...)     \
private:                                            \
    struct prop_ ## name;                           \
    __xget__(token_get,name,__VA_ARGS__)            \
    __xset__(token_set,name,__VA_ARGS__)            \
private:                                            \
    struct prop_ ## name :                          \
        mixc::macro_xprop::repeat<__VA_ARGS__>

#define xr      item_t operator()()
#define xw      void operator()(item_t value)

#define xpubget_pubset(name,...)    __xprop__(public   , public   , name,__VA_ARGS__)
#define xpubget_proset(name,...)    __xprop__(public   , protected, name,__VA_ARGS__)
#define xpubget_priset(name,...)    __xprop__(public   , private  , name,__VA_ARGS__)

#define xproget_pubset(name,...)    __xprop__(protected, public   , name, __VA_ARGS__)
#define xproget_proset(name,...)    __xprop__(protected, protected, name, __VA_ARGS__)
#define xproget_priset(name,...)    __xprop__(protected, private  , name, __VA_ARGS__)

#define xpriget_pubset(name,...)    __xprop__(private  , public   , name, __VA_ARGS__)
#define xpriget_proset(name,...)    __xprop__(private  , protected, name, __VA_ARGS__)
#define xpriget_priset(name,...)    __xprop__(private  , private  , name, __VA_ARGS__)

#define __xget_only__(token_get,name,...)           \
token_get:                                          \
    final & name(__VA_ARGS__ * get) const {         \
        get[0] = the.name();                        \
        return thex;                                \
    }                                               \
    __VA_ARGS__ name() const 

#define __xset_only__(token_get,name,...)           \
token_set:                                          \
    final & name(__VA_ARGS__ get)

#define xpubget(name,...)   __xget_only__(public   , name, __VA_ARGS__)
#define xproget(name,...)   __xget_only__(protected, name, __VA_ARGS__)
#define xpriget(name,...)   __xget_only__(private  , name, __VA_ARGS__)

#define xpubset(name,...)   __xset_only__(public   , name, __VA_ARGS__)
#define xproset(name,...)   __xset_only__(protected, name, __VA_ARGS__)
#define xpriset(name,...)   __xset_only__(private  , name, __VA_ARGS__)
