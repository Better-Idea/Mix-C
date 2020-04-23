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
        struct stack{
            struct node : inc::struct_t<item_t>{
                xgc_fields(
                    xiam(node, inc::struct_t<item_t>),
                    xpub(next, node *)
                );

                template<class ... args>
                node(args const & ... list) : 
                    inc::struct_t<item_t>(list...){
                    next = nullptr;
                }
            };

            using nodep = node *;

            xgc_fields(
                xiam(stack<item_t>, node),
                xpub(top, mutable nodep)
            );

            stack(){
                top = nullptr;
            }

            ~stack() {
                clear();
            }

            template<class ... args>
            void push_back(item_t const & first, args const & ... list) const {
                inc::implicit<item_t &> items[] = { first, list... };
                for (uxx i = 0; i < 1 + sizeof...(args); i++){
                    push_back(items[i]);
                }
            }

            void push_back(item_t const & value) const {
                auto new_top  = inc::alloc_with_initial<node>(value);
                new_top->next = the_t::top;
                the_t::top    = new_top;
            }

            template<class ... args>
            void pop_back(item_t * first, args ... list) const {
                inc::implicit<item_t *> items[] = { first, list... };
                for (uxx i = 0; i < 1 + sizeof...(args); i++){
                    pop_back(items[i]);
                }
            }

            void pop_back(item_t * result) const {
                auto tmp   = the_t::top;
                result[0]  = the_t::top[0];
                inc::free_with_destroy<node>(the_t::top);
                the_t::top = the_t::top->next;
            }

            void clear() {
                while (the_t::top != nullptr){
                    auto tmp   = the_t::top;
                    the_t::top = the_t::top->next;
                    inc::free_with_destroy<node>(tmp);
                }
            }

            inc::transmitter<item_t> pop_back() const {
                inc::transmitter<item_t> r(the_t::top[0]);
                inc::free_with_destroy<node>(the_t::top);
                the_t::top = the_t::top->next;
                return r;
            }

            item_t & top(){
                return *(item_t *)top;
            }

            const item_t & top() const {
                return *(item_t *)top;
            }

            bool is_empty() const {
                return top == nullptr;
            }
        };

        template<class final, class item_t>
        struct stack_t : stack<item_t> {
            using stack<item_t>::stack;
            using the_t = stack<item_t>;
            using node  = typename the_t::node;

            template<class ... args>
            final & push_back(item_t const & first, args const & ... list) const {
                the.push_back(first, list...);
                return thex;
            }

            final & push_back(item_t const & value) const {
                auto new_top  = inc::alloc_with_initial<node>(value);
                new_top->next = the_t::top;
                the_t::top    = new_top;
            }

            template<class ... args>
            void pop_back(item_t * first, args ... list) const {
                inc::implicit<item_t *> items[] = { first, list... };
                for (uxx i = 0; i < 1 + sizeof...(args); i++){
                    pop_back(items[i]);
                }
            }

            void pop_back(item_t * result) const {
                auto tmp   = the_t::top;
                result[0]  = the_t::top[0];
                inc::free_with_destroy<node>(the_t::top);
                the_t::top = the_t::top->next;
            }

            void clear() {
            

            
        };
    }
#endif

#define xusing_docker_stack     mixc::docker_stack
