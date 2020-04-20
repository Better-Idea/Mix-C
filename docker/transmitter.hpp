#ifndef xpack_docker_transmitter
#define xpack_docker_transmitter
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_transmitter
        #include"define/base_type.hpp"
        #include"dumb/mirror.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xis_nullptr.hpp"
        #include"macro/xref.hpp"
        #include"memop/copy.hpp"
        #include"memory/new.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_transmitter{
        template<class type>
        struct transmitter{
            xgc_fields(
                xthe(transmitter<type>),
                xpri(data,    inc::mirror<type>);
                xpri(is_none, mutable bool);
            );

            transmitter() : 
                is_none(true) {}

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

            void operator=(type const & value) {
                this->~transmitter();
                new (this) transmitter(value);
            }

            operator type & (){
                is_none = true;
                return data;
            }

            xis_nullptr(is_none == true);
        };
    }

#endif

namespace xuser::inc{
    using mixc::docker_transmitter::transmitter;
}
