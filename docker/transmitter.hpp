#ifndef xpack_docker_transmitter
#define xpack_docker_transmitter
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::docker_transmitter
    #include"define/base_type.hpp"
    #include"dumb/struct_t.hpp"

    namespace xuser::origin{
        template<class type>
        struct transmitter : inc::struct_t<type>{
        private:
            bool phas_hold_value = true;
        public:
            transmitter(){}
            transmitter(type const & value) : 
                inc::struct_t<type>(value){
                phas_hold_value = false;
            }

            auto & operator=(type const & value){
                auto & ori = *(type *)this;
                ori = value;
                phas_hold_value = true;
                return this[0];
            }

            auto & operator=(transmitter<type> const & value){
                this[0] = (inc::struct_t<type> &) value;
                phas_hold_value = value.phas_hold_value;
                return this[0];
            }

            bool has_hold_value() const {
                return phas_hold_value;
            }
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::docker_transmitter::origin;
}
