#ifndef xpack_docker_stack
#define xpack_docker_stack
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_stack
        #include"define/base_type.hpp"
        #include"docker/transmitter.hpp"
        #include"dumb/struct_t.hpp"
        #include"dumb/implicit.hpp"
        #include"macro/xgc.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_stack{
        template<class item_t>
        xgc(core)
            xgc(node, xpub(inc::struct_t<item_t>))
                xgc_fields(
                    xpub(next, node *)
                );

                template<class ... args>
                node(args const & ... list) : 
                    inc::struct_t<item_t>(list...){
                    next = nullptr;
                }
            xgc_end();

            using nodep = node *;

            xgc_fields(
                xpub(top, mutable nodep)
            );

            core(){
                top = nullptr;
            }
        xgc_end();

        template<class final, class item_t>
        struct stack : core<item_t> {
            using core<item_t>::core;
            using the_t = core<item_t>;
            using node  = typename the_t::node;

            ~stack(){
                clear();
            }

            template<class ... args>
            final & push_back(item_t const & first, args const & ... list) const {
                inc::implicit<item_t &> items[] = { first, list... };
                for (uxx i = 0; i < 1 + sizeof...(args); i++){
                    push_back(items[i]);
                }
                return thex;
            }

            final & push_back(item_t const & value) const {
                auto new_top  = inc::alloc_with_initial<node>(value);
                new_top->next = the_t::top;
                the_t::top    = new_top;
                return thex;
            }

            template<class ... args>
            final & pop_back(item_t * first, args ... list) const {
                inc::implicit<item_t *> items[] = { first, list... };
                for (uxx i = 0; i < 1 + sizeof...(args); i++){
                    pop_back(items[i]);
                }
                return thex;
            }

            final & pop_back(item_t * result) const {
                auto tmp   = the_t::top;
                result[0]  = the_t::top[0];
                inc::free_with_destroy<node>(the_t::top);
                the_t::top = the_t::top->next;
                return thex;
            }

            final & clear() const {
                while (the_t::top != nullptr){
                    auto tmp   = the_t::top;
                    the_t::top = the_t::top->next;
                    inc::free_with_destroy<node>(tmp);
                }
                return thex;
            }

            inc::transmitter<item_t> pop_back() const {
                inc::transmitter<item_t> r(the_t::top[0]);
                inc::free_with_destroy<node>(the_t::top);
                the_t::top = the_t::top->next;
                return r;
            }

            item_t & top(){
                return *(item_t *)the_t::top;
            }

            const item_t & top() const {
                return *(item_t *)the_t::top;
            }

            bool is_empty() const {
                return the_t::top == nullptr;
            }
        };
    }
#endif

#define xusing_docker_stack     mixc::docker_stack
