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
    #include"macro/xstruct.hpp"
    #include"math/align.hpp"
    #include"memop/addressof.hpp"
    #include"memop/copy.hpp"
    #include"memop/swap.hpp"
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
        xstruct(
            xtmpl(node, key_t, val_t),
            xpubf(key, key_t),
            xpubf(val, val_t)
        )
            // 避免误导 GC 判断
            node * next;

            node() : next(this) {}
            node(key_t const & key, val_t const & val) : 
                key(key), val(val), next(nullptr) {
            }

            hashmap_set_result set(key_t const & key, val_t const & value){
                // 约定 next == this 表示的是空节点，首元 next == nullptr 表示只有首元一个节点
                if (is_empty()){
                    new (this) node(key, value);
                    return hashmap_set_result::success;
                }

                for(auto cur = this; ; cur = cur->next){
                    if (cur->key == key){
                        cur->val    = value;
                        return hashmap_set_result::override;
                    }
                    if (cur->next == nullptr){
                        cur->next   = inc::alloc_with_initial<the_t>(key, value);
                        return hashmap_set_result::success;
                    }
                }
            }

            val_t & get(key_t const & key){
                if (not is_empty()){
                    for(auto cur = this; cur != nullptr; cur = cur->next){
                        if (cur->key == key){
                            return cur->val;
                        }
                    }
                }
                return inc::nullref;
            }

            the_t & take_out(key_t const & key, bool * can_release){
                if (can_release[0] = false; is_empty()){
                    return inc::nullref;
                }

                auto cur = this;
                auto pre = this;

                while(true){
                    if (cur == nullptr){
                        return inc::nullref;
                    }
                    if (cur->key == key){
                        break;
                    }
                    pre = cur;
                    cur = cur->next;
                }

                // cur 不是首元
                // 直接从链表中移除
                if (auto cur_next = cur->next; cur != this){
                    pre->next      = cur_next;
                    can_release[0] = true;
                }
                // cur 是首元但存在后继元素
                // 由于首元是数组中的一个元素，是不能单独释放的
                // 所以让 cur 和 cur_next 中的内容交换一下
                else if (cur_next != nullptr){
                    inc::swap(xref cur[0], xref cur_next[0]);
                    cur            = cur_next;
                    can_release[0] = true;
                }
                // cur 是首元且无后继
                // 标记此节点为空
                else{
                    cur->next      = this;
                }
                return cur[0];
            }

            void free(){
                if (is_empty()){
                    return;
                }
                while(next != nullptr){
                    auto temp = next;
                    temp->~node();
                    next      = next->next;
                    inc::free(temp);
                }
                the.~node();
                the.next = this;
            }

            bool is_empty() const {
                return next == this;
            }
        $

        template<class key_t, class val_t>
        xstruct(
            xtmpl(hashmap_t, key_t, val_t),
            xpubb(inc::self_management),
            xpubb(inc::disable_copy),
            xprif(lines,  uxx),
            xprif(count,  uxx),
            xprif(nodes,  node<key_t, val_t> *)
        )
        private:
            using pair_t   = node<key_t, val_t>;
            static constexpr uxx multi         = 4;
            static constexpr uxx start_capcity = 16;

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
                uxx index = addressing(key);
                xdebug(im_docker_hashmap_get, index);
                return nodes[index].get(key);
            }

            inc::transmitter<val_t> take_out(key_t const & key) {
                auto index      = addressing(key);
                auto can_relase = false;
                
                if (auto & pair = nodes[index].take_out(key, xref can_relase); pair == inc::nullref){
                    return inc::transmitter<val_t>();
                }
                else if (inc::transmitter<val_t> r = pair.val; can_relase){
                    inc::free_with_destroy(xref pair);
                    return r;
                }
                else{
                    return r;
                }
            }

            the_t & clear() {
                if (nodes != nullptr){
                    for(uxx i = 0; i < lines; i++){
                        nodes[i].free();
                    }
                }
                the.count = 0;
                return the;
            }

            the_t & remove(key_t const & key, hashmap_remove_result * state = nullptr) {
                if (auto && r = tale_out(key); state != nullptr){
                    state[0] = r.has_hold_value() ? 
                        hashmap_remove_result::success :
                        hashmap_remove_result::item_not_exist;
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
                hashmap_set_result * state = nullptr){

                if (lines == ++count){
                    resize(lines * multi);
                }

                auto index = addressing(key);
                xdebug(im_docker_hashmap_set, index, key, value);

                if (auto sta = nodes[index].set(key, value); state != nullptr){
                    state[0] = sta;
                }
                return the;
            }

            the_t & take_out(key_t const & key, val_t * value, hashmap_take_out_result * state = nullptr) {
                auto sta = hashmap_take_out_result::item_not_exist;

                if (auto && r = take_out(key); r.has_hold_value()){
                    value[0]  = r;
                    sta       = hashmap_take_out_result::success;
                }
                if (state){
                    state[0]  = sta;
                }
                return the;
            }

            /* 属性区 */
        public:
            bool is_empty() const {
                return length() == 0;
            }

            bool is_contains(key_t const & key) const {
                return get(key) != inc::nullref;
            }

            uxx length() const {
                return count;
            }

            /*私有区*/
        private:
            uxx addressing(key_t const & key) const {
                auto hash  = inc::hash(key);
                auto index = hash & mask();
                xdebug(im_docker_hashmap_addressing, hash, index);
                return index;
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
                for(uxx i = 0; i < lines; i++){
                    if (nodes[i].is_empty()){
                        continue;
                    }

                    auto & header = nodes[i];
                    val_t * dummy;

                    map.nodes[
                        map.addressing(header.key)
                    ].set(header.key, header.val);

                    for(auto cur = header.next; cur != nullptr;){
                        auto new_hash   = map.addressing(cur->key);
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
                the.free();
                return map;
            }

            uxx mask() const {
                return lines - 1;
            }

            void free() {
                inc::free(nodes, inc::memory_size(
                    lines * sizeof(node<key_t, val_t>)
                ));
                nodes = nullptr;
            }

            static node<key_t, val_t> * alloc(uxx counts){
                auto bytes = inc::memory_size(
                    counts * sizeof(node<key_t, val_t>)
                );
                auto nodes = inc::alloc<node<key_t, val_t>>(bytes);

                for (uxx i = 0; i < counts; i++){
                    new (nodes + i) node<key_t, val_t>();
                }
                return nodes;
            }
        $

        template<class final, class key_t, class val_t>
        struct hashmap : hashmap_t<key_t, val_t> {
            using the_t = hashmap_t<key_t, val_t>;
            using the_t::the_t;
            using the_t::take_out;

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
                key_t const &               key, 
                val_t const &               value, 
                hashmap_set_result *        state = nullptr) {
                return (final &)the.set(key, value, state);
            }

            final & take_out(
                key_t const &               key, 
                val_t *                     value, 
                hashmap_take_out_result *   state = nullptr) {
                return (final &)the.take_out(key, value, state);
            }
        };
    }

    namespace mixc::docker_hashmap::origin{
        using ::mixc::docker_hashmap::hashmap_remove_result;
        using ::mixc::docker_hashmap::hashmap_take_out_result;
        using ::mixc::docker_hashmap::hashmap_set_result;
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::docker_hashmap::origin;
}

#define xusing_docker_hashmap     ::mixc::docker_hashmap
