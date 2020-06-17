#define xgen(name,...)                          \
u32 name() const {                              \
    return p ## name;                           \
}                                               \
final & name(u32 * value) const {               \
    value[0] = p ## name;                       \
    return thex;                                \
}                                               \
final & name(u32 value) {                       \
    p ## name = value;                          \
    return thex;                                \
}
