#ifndef xpack_docker_hashmap
#define xpack_docker_hashmap
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_hashmap
        #include"algo/hash.hpp"
        #include"define/base_type.hpp"
        #include"define/nullref.hpp"
        #include"docker/transmitter.hpp"
        #include"dumb/mirror.hpp"
        #include"dumb/disable_copy.hpp"
        #include"gc/self_management.hpp"
        #include"macro/xdebug.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xref.hpp"
        #include"math/align.hpp"
        #include"memop/copy.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_hashmap{
        enum class hashmap_remove_result{
            success,
            item_not_exist,
        };

        enum class hashmap_take_out_result{
            success,
            item_not_exist,
        };

        enum class hashmap_set_result{
            success,
            override,
        };

        template<class key_t, class val_t>
        struct hashmap_t : inc::self_management, inc::disable_copy {
        private:
            template<class guide> inc::routing_result routing(){
                return inc::routing_result();
                // TODO:================================================================
            }

            typedef struct pair{
                key_t    key;
                val_t    value;

                pair(key_t const & key, val_t const & value) : 
                    key(key), value(value){}
            } * pairp;

            struct node{
                using mirror_t = u08[sizeof(pair)];

                xgc_fields(
                    //xiam(node, key_t, val_t),
                    xiam(node),
                    xpub(next,      node *),
                    xpub(hash_code, uxx),
                    xpub(mirror,    mirror_t),
                    xhas(key_t),
                    xhas(val_t)
                );

                node() : next(this) {}
                node(uxx hash_code, key_t const & key, val_t const & value, bool with_copy_operator) : 
                    next(nullptr), hash_code(hash_code) {
                    if (with_copy_operator){
                        new (mirror) pair(key, value);
                    }
                    else{
                        inc::copy<key_t>(xref the->key, key);
                        inc::copy<val_t>(xref the->value, value);
                    }
                }

                hashmap_set_result set(
                    uxx             hash, 
                    key_t const &   key, 
                    val_t const &   value, 
                    val_t **        receive, 
                    bool            with_copy_operator = true){

                    // 约定 next == this 表示的是空节点
                    if (is_empty()){
                        new (this) node(hash, key, value, with_copy_operator);
                        receive[0] = xref the->value;
                        return hashmap_set_result::success;
                    }

                    for(auto cur = this; ; cur = cur->next){
                        if (cur->hash_code == hash and cur[0]->key == key){
                            cur[0]->value   = value;
                            receive[0]      = xref cur[0]->value;
                            return hashmap_set_result::override;
                        }
                        if (cur->next == nullptr){
                            cur->next       = inc::alloc_with_initial<node>(hash, key, value, with_copy_operator);
                            receive[0]      = xref cur[0]->value;
                            return hashmap_set_result::success;
                        }
                    }
                }

                val_t & get(uxx hash, key_t const & key) const {
                    if (is_empty() == false){
                        for(auto cur = this; cur != nullptr; cur = cur->next){
                            if (cur->hash_code == hash and cur[0]->key == key){
                                return cur[0]->value;
                            }
                        }
                    }
                    return inc::nullref;
                }

                void take_out(uxx hash, key_t const & key, inc::transmitter<pair> * receive){
                    if (is_empty()){
                        return;
                    }

                    auto cur = this, pre = this;
                    for(; cur != nullptr; pre = cur, cur = cur->next){
                        if (cur->hash_code == hash and cur[0]->key == key){
                            break;
                        }
                    }

                    // 未找到
                    if (cur == nullptr){
                        return;
                    }

                    receive[0] = *(pair *)cur->mirror;

                    // cur 不是首元
                    if (cur != this){
                        pre->next = cur->next;
                        inc::free(cur);
                    }
                    // cur 是首元但存在后继元素
                    else if (next != nullptr){
                        auto tmp = next;
                        the = next[0];
                        inc::free(tmp);
                    }
                    // cur 是首元且无后继
                    // 标记此节点为空
                    else{
                        next = this;
                    }
                }

                void free(){
                    if (is_empty()){
                        return;
                    }
                    while(next != nullptr){
                        auto temp = next;
                        temp[0]->~pair();
                        next      = next->next;
                        inc::free(temp);
                    }
                    the->~pair();
                    the.next = this;
                }

                bool is_empty() const {
                    return next == this;
                }

                pair * operator->() const {
                    return (pair *)mirror;
                }
            };

            static constexpr uxx multi         = 4;
            static constexpr uxx start_capcity = 16;

            xgc_fields(
                xiam(hashmap_t<key_t, val_t>, inc::self_management),
                xpro(lines,  uxx),
                xpro(count,  uxx),
                xpro(nodes,  node *),
                xhas(node)
            );
        public:

            /*构造/析构区*/
        public:
            hashmap_t() : hashmap_t(start_capcity){}
            hashmap_t(uxx start_capcity) : 
                lines(inc::align(start_capcity)), 
                count(0), 
                nodes(the_t::alloc(lines)) {
            }
        protected:
            ~hashmap_t(){
                the.clear();
                the.free();
            }

            /*接口区*/
        public:
            val_t & get(key_t const & key) const {
                auto hash  = inc::hash(key);
                auto index = hash & mask();
                xdebug(im_docker_hashmap_get, hash, index, key);
                return nodes[index].get(hash, key);
            }

            inc::transmitter<val_t> take_out(key_t const & key) {
                inc::transmitter<pair>  mem;
                inc::transmitter<val_t> r;
                auto a = addressing(key);
                
                if (nodes[a.index].take_out(a.hash, key, xref mem); mem != nullptr){
                    pair & p = mem; // 取消 mem 的析构操作，使用手动析构
                    r        = p.value;
                    p.key.~key_t(); // 只析构 key。value 的析构交给 r 托管
                }
                return r;
            }

            the_t & clear() {
                if (nodes != nullptr){
                    for (uxx i = 0; i < lines; i++){
                        nodes[i].free();
                    }
                }
                return the;
            }

            the_t & remove(key_t const & key, hashmap_remove_result * state = nullptr) {
                inc::transmitter<pair> mem; // 如果存在 key，则自动析构从 take_out 带出来的 pair
                auto r = addressing(key);
                
                if (nodes[r.index].take_out(r.hash, key, xref mem); state != nullptr){
                    state[0] = mem == nullptr ? 
                        hashmap_remove_result::item_not_exist :
                        hashmap_remove_result::success;
                }
                return the;
            }

            the_t & resize() {
                if (lines >= count * multi * 2 and lines > start_capcity){
                    resize(lines / multi);
                }
                return the;
            }

            the_t & set(
                key_t const &        key, 
                val_t const &        value, 
                val_t **             receive, 
                hashmap_set_result * state = nullptr){

                if (lines == ++count){
                    resize(lines * multi);
                }

                auto a = addressing(key);
                xdebug(im_docker_hashmap_set, a.hash, a.index, key, value);

                if (auto tmp = nodes[a.index].set(a.hash, key, value, receive); state != nullptr){
                    state[0] = tmp;
                }
                return the;
            }

            the_t & take_out(key_t const & key, val_t * value, hashmap_take_out_result * state = nullptr) {
                auto tmp = hashmap_take_out_result::item_not_exist;

                if (auto && r = take_out(key); r != nullptr){
                    value[0]  = r;
                    tmp       = hashmap_take_out_result::success;
                }
                if (state){
                    state[0]  = tmp;
                }
                return the;
            }

            /* 属性区 */
        public:
            bool is_empty() const {
                return length() == 0;
            }

            bool contains(key_t const & key) const {
                return get(key) != inc::nullref;
            }

            uxx length() const {
                return count;
            }

            /*私有区*/
        private:
            auto addressing(key_t const & key) const {
                struct { uxx index, hash; } r;
                r.hash  = inc::hash(key);
                r.index = r.hash & mask();
                return r;
            }

            the_t & resize(uxx capcity){
                inc::mirror<the_t> mem = the;
                new(& mem) the_t(capcity);
                resize_to(mem);
                inc::copy(this, mem);
                return the;
            }

            // 该函数用于 hashmap_t 内部扩容和压缩
            // 要求 map 是新分配的空间，并且内部无元素
            the_t & resize_to(the_t & map) {
                for (uxx i = 0; i < lines; i++){
                    if (nodes[i].is_empty()){
                        continue;
                    }

                    auto &  header = nodes[i];
                    val_t * dummy;

                    map.nodes[
                        header.hash_code & map.mask()
                    ].set(header.hash_code, header->key, header->value, xref dummy, false/*屏蔽 key/value 的复制构造和赋值重载*/);

                    for(auto cur = header.next; cur != nullptr;){
                        auto new_hash   = cur->hash_code & map.mask();
                        auto new_host   = map.nodes + new_hash;
                        auto host_next  = new_host->next;
                        auto cur_next   = cur->next;

                        if (new_host->is_empty()){
                            new_host[0]          = cur[0];
                            new_host->next       = nullptr;
                            inc::free(cur);
                        }
                        else{
                            new_host->next       = cur;
                            new_host->next->next = host_next;
                        }
                        cur             = cur_next;
                    }
                }

                map.count = count;
                this->free();
                return map;
            }

            uxx mask() const {
                return lines - 1;
            }

            void free() {
                inc::free(nodes, inc::memory_size(
                    lines * sizeof(node)
                ));
                nodes = nullptr;
            }

            static node * alloc(uxx counts){
                auto bytes = inc::memory_size(
                    counts * sizeof(node)
                );
                auto nodes = inc::alloc<node>(bytes);

                for (uxx i = 0; i < counts; i++){
                    new (nodes + i) node();
                }
                return nodes;
            }
        };

        template<class final, class key_t, class val_t>
        struct hashmap : hashmap_t<key_t, val_t> {
            using the_t = hashmap_t<key_t, val_t>;
            using the_t::the_t;

            final & clear() {
                return (final &)the.clear();
            }

            final & remove(key_t const & key, hashmap_remove_result * state = nullptr) {
                return (final &)the.remove(key, state);
            }

            final & resize(){
                return (final &)the.resize();
            }

            final & set(
                key_t const &        key, 
                val_t const &        value, 
                hashmap_set_result * state = nullptr) {
                val_t * receive;
                return (final &)the.set(key, value, xref receive, state);
            }

            final & set(
                key_t const &        key, 
                val_t const &        value, 
                val_t **             receive, 
                hashmap_set_result * state = nullptr) {
                return (final &)the.set(key, value, receive, state);
            }

            final & take_out(
                key_t const &           key, 
                val_t *                 value, 
                hashmap_remove_result * state = nullptr) {
                return (final &)the.take_out(key, value, state);
            }
        };
    }
#endif

#define xusing_docker_hashmap     mixc::docker_hashmap

namespace xuser::inc{
    using xusing_docker_hashmap::hashmap_remove_result;
    using xusing_docker_hashmap::hashmap_take_out_result;
    using xusing_docker_hashmap::hashmap_set_result;
}
