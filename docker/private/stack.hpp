#ifndef xpack_docker_stack
#define xpack_docker_stack
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_stack
        #include"define/base_type.hpp"
        #include"docker/transmitter.hpp"
        #include"docker/private/pushpop_t.hpp"
        #include"dumb/disable_copy.hpp"
        #include"dumb/struct_t.hpp"
        #include"gc/self_management.hpp"
        #include"interface/ranger.hpp"
        #include"lock/atom_swap.hpp"
        #include"macro/xgc.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_stack{
        template<class item_t>
        struct stack_t : inc::self_management, inc::disable_copy {
            typedef struct node : inc::struct_t<item_t>{
                xgc_fields(
                    xiam(node, inc::struct_t<item_t>),
                    xpub(next, node *)
                );
            public:
                template<class ... args>
                node(args const & ... list) :
                    inc::struct_t<item_t>(list...){
                    next = nullptr;
                }
            } * nodep;

            xgc_fields(
                xiam(stack_t<item_t>, inc::self_management, inc::disable_copy),
                xpro(ptop, mutable nodep),
                xhas(node)
            ) {
                // TODO:====================================================================
                return inc::routing_result();
            }
        public:
            stack_t() : 
                ptop(nullptr){
            }
        protected:
            ~stack_t() {
                clear();
            }
        public:
            void clear() {
                nodep cur = inc::atom_swap(& the.ptop, nodep(nullptr));
                nodep tmp;
                while(cur != nullptr){
                    tmp = cur;
                    cur = cur->next;
                    inc::free_with_destroy<node>(tmp);
                }
            }

            void push(item_t const & value) {
                auto new_top  = inc::alloc_with_initial<node>(value);
                new_top->next = inc::atom_swap(& ptop, new_top);
            }

            inc::transmitter<item_t> pop() {
                inc::transmitter<item_t> r = ptop[0];
                inc::free_with_destroy<node>(ptop);
                ptop = ptop->next;
                return r;
            }

            item_t & top(){
                return *(item_t *)ptop;
            }

            item_t const & top() const {
                return *(item_t *)ptop;
            }

            bool is_empty() const {
                return ptop == nullptr;
            }
        };

        template<class final, class item_t>
        using stack = inc::pushpop_t<final, stack_t<item_t>, item_t>;
    }
#endif

#define xusing_docker_stack     ::mixc::docker_stack

/*
        \   when
    can  \  top     pop     push    clear   routing
    top     true    false   true    false   true
    pop     false   false   false   false   false
    push    true    false   true    false   false
    clear   false   false   false   true    false
    routing true    false   false   false   false
*/