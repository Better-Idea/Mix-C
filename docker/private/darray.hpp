#ifndef xpack_docker_darray
#define xpack_docker_darray
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_darray
        #include"define/base_type.hpp"
        #include"dumb/dummy_t.hpp"
        #include"gc/ref.hpp"
        #include"interface/ranger.hpp"
        #include"macro/xgc.hpp"
        #include"memory/new.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_darray{
        template<class type, uxx rank, class attribute>
        struct darray_t : public inc::ref_array<
            darray_t<type, rank, attribute>,
            typename 
            darray_t<type, rank - 1, attribute>::the_t,
            attribute
        >{
            using item_t = typename darray_t<type, rank - 1, attribute>::the_t;
            using base_t = inc::ref_array<darray_t<type, rank, attribute>, item_t, attribute>;
            using base_t::operator[];
            using base_t::operator->;

            xgc_fields(
                xiam(darray_t<type, rank, attribute>, base_t)
            );
        public:
            xrange(item_t);

            darray_t() : 
                darray_t(inc::empty_array){
                static_assert(base_t::header_size() <= sizeof(inc::empty_array));
            }

            darray_t(darray_t const &) = default;

            darray_t(inc::length length) :
                base_t(length) {}

            template<class ... args>
            darray_t(inc::length length, args const & ... list) : 
                base_t(length, list...) {}

            template<class ... args>
            auto & operator()(inc::length length, args const & ... list){
                using metap = base_t *;
                the.~base_t();
                new (metap(this)) base_t(length, list...);
                return the;
            }

            bool is_empty() const {
                return length() == 0;
            }

            uxx length() const {
                return base_t::length();
            }
        };

        template<class type, class attribute>
        struct darray_t<type, 0, attribute>{
            using the_t = type;
        };

        template<class final, class type, uxx rank, class attribute>
        struct darray : darray_t<type, rank, attribute> {
            using darray_t<type, rank, attribute>::darray_t;
        };
    }

#endif

#define xusing_docker_darray        ::mixc::docker_darray
