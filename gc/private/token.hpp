#ifndef xpack_gc_token
#define xpack_gc_token
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::gc_token
        #include"define/base_type.hpp"
        #include"dumb/dummy_t.hpp"
        #include"lock/atom_add.hpp"
        #include"lock/atom_and.hpp"
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

    namespace mixc::memory_alloctor{
        struct memory_size;
        template<class t> void free_with_destroy(t *, mixc::memory_alloctor::memory_size);
    }

    namespace mixc::gc_token{
        constexpr uxx  mark_visited         = uxx(1) << (sizeof(uxx) * 8 - 1);
        constexpr uxx  mark_can_arrive_root = uxx(1) << (sizeof(uxx) * 8 - 2);
        constexpr uxx  mark_in_free         = uxx(1) << (sizeof(uxx) * 8 - 3);
        constexpr uxx  mask_owners          = uxx(-1) >> 3;
        constexpr uxx  step                 = uxx(1);

        struct token {
            token(uxx length) : record(step) { }
        protected:
            template<class impl, class a, class b, bool is_array> friend struct mixc::gc_ref::meta;
            template<class root_t, class list> friend union mixc::gc_tuple::tuple;

            constexpr auto this_length() { return uxx(0); }
            constexpr auto this_length(uxx value) { }

            uxx record;

            uxx owners(){
                return record & mask_owners;
            }

            bool visited(){
                return (record & mark_visited) != 0;
            }

            bool can_arrive_root(){
                return (record & mark_can_arrive_root) != 0;
            }

            bool in_free(){
                return (record & mark_in_free) != 0;
            }
            
            uxx owners_inc(){
                return inc::atom_add(& record, step) & mask_owners;
            }

            uxx owners_dec(){
                return inc::atom_sub(& record, step) & mask_owners;
            }

            void set_visit(){
                inc::atom_or(& record, mark_visited);
            }

            void reset_visit(){
                inc::atom_and(& record, ~mark_visited);
            }

            void set_can_arrive_root(){
                inc::atom_or(& record, mark_can_arrive_root);
            }

            void reset_can_arrive_root(){
                inc::atom_and(& record, ~mark_can_arrive_root);
            }

            void set_in_free(){
                inc::atom_or(& record, mark_in_free);
            }

            void reset_in_free(){
                inc::atom_and(& record, ~mark_in_free);
            }
        };

        
        struct token_plus : token {
            token_plus(uxx length) : length(length), token(0) { }
            const uxx length;
        protected:
            auto this_length() {
                return length;
            }

            auto this_length(uxx value) {
                (uxx &)length = value;
            }
        };

        template<class type, class attribute = inc::dummy_t, class addition = token>
        struct token_mix : addition, attribute {
            template<class ... args>
            token_mix(uxx length, args const ... list) : 
                addition(length), attribute(list...) {}
        private:
            ~token_mix() {
                xdebug(im_inner_gc_$token_mix, "%s free\n", xtypeid(attribute).name());

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
    using mixc::gc_token::token;
    using mixc::gc_token::token_plus;
    using mixc::gc_token::token_mix;
}
