
#ifndef xpack_docker_shared_ptr
#define xpack_docker_shared_ptr
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_shared_ptr
        #include"define/base_type.hpp"
        #include"gc/private/ref.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_shared_ptr{
        template<class type> struct shared_ptr;
        template<class type>
        struct shared_ptr : inc::ref_ptr<shared_ptr<type>, type> {
            using meta = inc::ref_ptr<shared_ptr<type>, type>;
            explicit shared_ptr(ini) : meta(length(0)) {}

            template<class ... args>
            explicit shared_ptr(ini, args const & ... list) : meta(length(0), list...) {}

            shared_ptr() = default;

            template<class ... args>
            shared_ptr<type> & operator()(ini, args const & ... list){
                using metap = meta *;
                xthe.~meta();
                new (metap(this)) meta(length(0), list...);
                return xthe;
            }
        private:
            template<class __type__> friend union mixc::macro_xtypeid::__typeid__;
            static constexpr const char * __self_name__ = "shared_ptr";
        };
    }

#endif

namespace xuser::inc{
    using namespace mixc::docker_shared_ptr;
}
