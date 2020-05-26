#ifndef xpack_docker_priority_queue
#define xpack_docker_priority_queue
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_priority_queue
        #include"algo/heap_root.hpp"
        #include"define/base_type.hpp"
        #include"docker/transmitter.hpp"
        #include"docker/darray/pushpop.hpp"
        #include"docker/darray.hpp"
        #include"docker/private/pushpop_t.hpp"
        #include"dumb/struct_t.hpp"
        #include"dumb/mirror.hpp"
        #include"gc/self_management.hpp"
        #include"macro/xgc.hpp"
        #include"memop/cast.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_priority_queue {
        //template<class item_t> struct priority_queue_t;
        //using item_t = int;
        //template<>
        //struct priority_queue_t<item_t> : inc::self_management {

        template<class item_t>
        struct priority_queue_t : inc::self_management {
            using mirror_item_t = inc::mirror<item_t>;
            using docker_t      = inc::darray<item_t>;

            xgc_fields(
                xiam(priority_queue_t<item_t>, inc::self_management),
                xpri(items, inc::darray<mirror_item_t>),
                xhas(inc::darray<item_t>)
            ) {
                // TODO:====================================================================
                return inc::routing_result();
            }
        private:
            docker_t & origin() const {
                return inc::cast<docker_t>(items);
            }
        public:
            priority_queue_t() = default;
            priority_queue_t(uxx start_capacity) : 
                items(::length(start_capacity)){
            }

            void clear() {
                origin().clear();
            }

            uxx length() const {
                return items.length();
            }

            item_t & root() {
                return origin()[0];
            }
            
            item_t const & root() const {
                return origin()[0];
            }

            void push(item_t const & value) {
                items.push(value);
                inc::heap_root<item_t>::push(origin(), items.length() - 1, value);
            }

            inc::transmitter<item_t> pop() {
                auto length = items.length();
                inc::transmitter<item_t> r = origin()[0];
                inc::heap_root<item_t>::pop(origin(), length, items[length - 1]);
                items.pop();
                return r;
            }
        };

        template<class final, class item_t>
        using priority_queue = inc::pushpop_t<final, priority_queue_t<item_t>, item_t>;
    }
#endif

#define xusing_docker_name  ::mixc::docker_priority_queue
