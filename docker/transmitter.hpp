#ifndef xpack_docker_transmitter
#define xpack_docker_transmitter
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_transmitter
        #include"define/base_type.hpp"
        #include"dumb/mirror.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xref.hpp"
        #include"memop/copy.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_transmitter{
        template<class type>
        xgc(transmitter)
            xgc_fields(
                xpri(data, inc::mirror<type>),
                xpri(is_none, mutable bool)
            );

            transmitter(type const & value){
                inc::copy(xref data, value);
                is_none = false;
            }

            transmitter(transmitter<type> const & value) {
                inc::copy(xref data, value.data);
                value.is_none = true;
            }

            ~transmitter(){
                if (not is_none){
                    ((type &)data).~type();
                }
            }

            operator type & (){
                is_none = true;
                return data;
            }
        xgc_end();
    }

#endif

namespace xuser::inc{
    using mixc::docker_transmitter::transmitter;
}
