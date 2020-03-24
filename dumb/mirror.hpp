#ifndef xpack_dumb_mirror
#define xpack_dumb_mirror
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::dumb_mirror
        #include"define/base_type.hpp"
        #include"memop/cast.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_mirror{
        template<uxx count>
        xgc(mirror, xtmpl(count))
            using items_t = u08 [count];
            xgc_fields(
                xpub(bytes, items_t),
            );

            static constexpr uxx length = count;

            mirror(): bytes{0}{}

            template<class a>
            mirror(a const & value){
                this[0] = inc::cast<mirror<length>>(value);
            }
        xgc_end();
    }

#endif

namespace xuser::inc{
    using mixc::dumb_mirror::mirror;
}
