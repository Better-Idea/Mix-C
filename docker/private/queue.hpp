#ifndef xpack_docker_queue
#define xpack_docker_queue
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_queue
        #include"define/base_type.hpp"
        #include"dumb/disable_copy.hpp"
        #include"dumb/struct_t.hpp"
        #include"docker/transmitter.hpp"
        #include"docker/private/pushpop_t.hpp"
        #include"gc/self_management.hpp"
        #include"interface/ranger.hpp"
        #include"lock/atom_swap.hpp"
        #include"macro/xgc.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_queue {
        template<class item_t>
        struct queue_t : inc::self_management, inc::disable_copy {
            typedef struct node : inc::struct_t<item_t> {
                xgc_fields(
                    xiam(node, inc::struct_t<item_t>),
                    xpub(next,  node *)
                );
            public:
                template<class ... args>
                node(args const & ... list) : 
                    inc::struct_t<item_t>(list...), next(nullptr){}
            } * nodep;

            xgc_fields(
                xiam(queue_t<item_t>, inc::self_management, inc::disable_copy),
                xpro(pend, node *),
                xhas(node)
            ){
                // TODO:====================================================================
                return inc::routing_result();
            }

        public:
            queue_t():
                pend(nullptr) {
            }
        protected:
            ~queue_t() {
                clear();
            }
        public:
            void clear() {
                nodep cur  = inc::atom_swap(& the.pend, nodep(nullptr));
                nodep head = cur;
                nodep tmp;

                if (cur == nullptr) {
                    return;
                }

                do {
                    tmp  = head;
                     head = head->next;
                    inc::free_with_destroy<node>(tmp);
                } while (head != cur);
            }

            void push(item_t const & value) {
                auto new_end = inc::alloc_with_initial<node>(value);
                auto old_end = inc::atom_swap(& pend, new_end);

                if (old_end != nullptr) {
                    new_end->next = old_end->next; // old_end->next 指向 head
                    old_end->next = new_end;
                }
                else {
                    new_end->next = new_end;
                }
            }

            inc::transmitter<item_t> pop() {
                auto head                  = pend->next;
                auto next                  = head->next;
                inc::transmitter<item_t> r = (item_t &)head[0];
                
                if (next != head) {
                    pend->next = next;
                }
                else {
                    pend = nullptr;
                }
                inc::free_with_destroy<node>(head);
                return r;
            }

            item_t & head() {
                return pend->next[0];
            }

            item_t const & head() const {
                return pend->next[0];
            }

            bool is_empty() const {
                return pend == nullptr;
            }
        };

        template<class final, class item_t>
        using queue = inc::pushpop_t<final, queue_t<item_t>, item_t>;
    }
#endif

#define xusing_docker_queue  ::mixc::docker_queue
