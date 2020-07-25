// 注意：在 msvc 2019 早期版本不能正确处理该 concept
#ifndef xpack_meta_has_operator_cast
#define xpack_meta_has_operator_cast
namespace mixc::meta_has_operator_cast{
    template<class target, class source>
    concept has_operator_cast = requires(source value){
        value.operator target();
    };
}

#endif

namespace xuser::inc{
    using ::mixc::meta_has_operator_cast::has_operator_cast;
}
