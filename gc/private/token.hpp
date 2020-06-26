#ifndef xpack_gc_token
#define xpack_gc_token
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::gc_token
    #include"define/base_type.hpp"
    #include"dumb/dummy_t.hpp"
    #include"lock/atom_add.hpp"
    #include"lock/atom_or.hpp"
    #include"lock/atom_sub.hpp"
    #include"macro/xdebug.hpp"

    namespace mixc::gc_tuple{
        template<class root_t, class list> union tuple;
    }

    namespace mixc::gc_ref{
        template<class impl, class a, class b, bool is_array> struct meta;
    }

    namespace mixc::macro_xsv{
        template<class item> struct static_string_holder;
    }

    namespace mixc::memory_alloctor::origin{
        struct memory_size;
        template<class t> void free_with_destroy(t *, memory_size);
    }

    namespace xuser::origin{
        constexpr uxx step              = uxx(1);

        struct token {
            token(uxx) : record(step) { }
        protected:
            template<class impl, class a, class b, bool is_array> friend struct mixc::gc_ref::meta;
            template<class root_t, class list> friend union mixc::gc_tuple::tuple;

            constexpr auto this_length() { return uxx(0); }
            constexpr auto this_length(uxx) { }

            uxx record;

            uxx owners(){
                return record;
            }

            uxx owners_inc(){
                return inc::atom_add(& record, step);
            }

            uxx owners_dec(){
                return inc::atom_sub(& record, step);
            }
        };

        struct token_plus : token {
            uxx length;

            token_plus(uxx length) : 
                token(0), length(length) { }

            auto this_length() {
                return length;
            }

            auto this_length(uxx value) {
                length = value;
            }
        };

        template<class type, class attribute = inc::dummy_t, class addition = token>
        struct token_mix : protected addition, public attribute {
            template<class ... args>
            token_mix(uxx length, args const & ... list) : 
                addition(length), attribute(list...) {}

            token_mix(token_mix const &)        = delete;
            void operator=(token_mix const &)   = delete;
        private:
            ~token_mix() {
                xdebug(im_gc_$token_mix, xtypeid(attribute).name);

                for (uxx i = 0; i < addition::this_length(); i++) {
                    (*this)[i].~type();
                }
            }

            auto * ptr(uxx index){
                return ((type *)(this + 1)) + index;
            }

            auto & operator [] (uxx index) {
                return ptr(index)[0];
            }

            template<class impl, class a, class b, bool is_array> friend struct mixc::gc_ref::meta;
            template<class t> friend void mixc::memory_alloctor::origin::free_with_destroy(t *, mixc::memory_alloctor::origin::memory_size);
        };
    }
    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::gc_token::origin;
}
