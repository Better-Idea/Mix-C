#ifndef xpack_docker_array
#define xpack_docker_array
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_array
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_array{
        template<class type, uxx count = 0, uxx ... rest>
        xgc(core)
            using self  = core<type, count, rest...>;
            using item  = typename core<type, rest...>::self;
            using itemp = item *;
            using items = item[count];

            xgc_fields(
                xpro(data, items)
            );

            template<class ... args>
            core(args const & ... list) : 
                data { item(list)... } {}
        xgc_end();

        template<class type>
        struct core<type>{
            using self = type;
            using item = self;
        };

        template<class final, class type, uxx count, uxx ... rest>
        struct array : core<type, count, rest...> {
            using core<type, count, rest...>::core;
            using the_t = core<type, count, rest...>;
            using typename the_t::item;

            item & operator[] (uxx index) {
                return the_t::data[index];
            }

            const item & operator[] (uxx index) const {
                return the_t::data[index];
            }

            constexpr uxx length() const {
                return count;
            }
        };
    }
#endif

#undef  xusing_docker_array
#define xusing_docker_array     mixc::docker_array
