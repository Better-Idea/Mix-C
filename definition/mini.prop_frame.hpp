#pragma once
#define thes                            __skeleton__()
#define thei                            __interface__()
#define xprop_meta(the_firend)                              \
public:                                                     \
    typedef skeleton<xargs>  __ske;                         \
    typedef interface<xargs> __itf;                         \
    friend the_firend;                                      \
private:                                                    \
    __ske & __skeleton__() const {                          \
        return *(__ske *)(this);                            \
    }                                                       \
    __itf & __interface__() const {                         \
        return *(__itf *)(this);                            \
    }                                                       \
public:

#define xskeleton(...)                                      \
    xprefix struct skeleton;                                \
    xprefix struct interface;                               \
    xprefix struct skeleton __VA_ARGS__ {                   \
        xprop_meta(__itf)                                   \
        __itf * operator->() const {                        \
            return & __interface__();                       \
        }                                                   \
        xtest_reflect

#define xinterface(...)                                     \
    xprefix struct interface __VA_ARGS__ {                  \
        xprop_meta(__ske)                                   \
        xtest_can_show

#define xpropx(type,name)   xprop(type,name,)
#define xprop(type,name,a,...)                              \
    struct prop_ ## name; prop_ ## name name;               \
    struct prop_ ## name {                                  \
    protected:                                              \
        typedef type read_type;                             \
        typedef mixc::mini_private::pure_type<type> const & \
            write_type;                                     \
        typedef prop_ ## name this_prop;                    \
        friend __ske;                                       \
        friend __itf;                                       \
        void operator=(this_prop const &) = delete;         \
        static constexpr uxx this_offset() {                \
            typedef __itf * i;                              \
            return uxx(& i(magic_number)->name) -           \
                magic_number;                               \
        }                                                   \
        __ske & __skeleton__() const {                      \
            return *(__ske *)(uxx(this) - this_offset());   \
        }                                                   \
        __itf & __interface__() const {                     \
            return *(__itf *)(uxx(this) - this_offset());   \
        }                                                   \
        a ## _redirection(name);                            \
        __VA_ARGS__ ## _redirection(name);

#define xpubget                         public   : operator read_type () {
#define xproget                         protected: operator read_type () {
#define xpriget                         private  : operator read_type () {
#define xpubget_redirection(name)       xpubget return thes.name; $;
#define xproget_redirection(name)       xproget return thes.name; $;
#define xpriget_redirection(name)       xpriget return thes.name; $;

#define xpubset                         public   : void operator = (write_type value) {
#define xproset                         protected: void operator = (write_type value) {
#define xpriset                         private  : void operator = (write_type value) {
#define xpubset_redirection(name)       xpubset thes.name = value; $;
#define xproset_redirection(name)       xproset thes.name = value; $;
#define xpriset_redirection(name)       xpriset thes.name = value; $;
#define $$                              xtested __ske & __field__ = thes;
