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
    #pragma pop_macro("xuser")

    namespace mixc::gc_tuple{
        template<class root_t, class list> union tuple;
    }

    namespace mixc::gc_ref{
        template<class impl, class a, class b, bool is_array> struct meta;
    }

    namespace mixc::macro_xsv{
        template<class item> struct static_string_holder;
    }

    namespace mixc::memory_alloctor{
        struct memory_size;
        template<class t> void free_with_destroy(t *, mixc::memory_alloctor::memory_size);
    }

    namespace mixc::gc_token{
        constexpr uxx step              = uxx(1);
        constexpr uxx mark_under_free   = uxx(1) << (sizeof(uxx) * 8 - 1);
        constexpr uxx mask_owners       = mark_under_free - 1;

        struct token {
            token(uxx) : record(step) { }
        protected:
            template<class impl, class a, class b, bool is_array> friend struct mixc::gc_ref::meta;
            template<class root_t, class list> friend union mixc::gc_tuple::tuple;

            constexpr auto this_length() { return uxx(0); }
            constexpr auto this_length(uxx) { }

            uxx record;

            uxx owners(){
                return record & mask_owners;
            }

            uxx owners_inc(){
                return inc::atom_add(& record, step);
            }

            uxx owners_dec(){
                return inc::atom_sub(& record, step);
            }

            void mark_under_free(){
                inc::atom_or(& record, mixc::gc_token::mark_under_free);
            }

            bool is_under_free(){
                return (record & mixc::gc_token::mark_under_free) != 0;
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
            template<class t> friend void mixc::memory_alloctor::free_with_destroy(t *, mixc::memory_alloctor::memory_size);
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::gc_token::token;
    using ::mixc::gc_token::token_plus;
    using ::mixc::gc_token::token_mix;
}
