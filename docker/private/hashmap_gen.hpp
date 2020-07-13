#ifdef xarg_has_val_t
template<class key_t, class val_t>
xstruct(
    xtmpl(node, key_t, val_t),
    xpubf(key, inc::mirror<key_t>),
    xpubf(val, inc::mirror<val_t>),
    xasso(key_t),
    xasso(val_t)
)
    node(key_t const & key, val_t const & val, inc::construction_t mode) : 
        key(key, mode), val(val, mode), next(nullptr) {
    }

    #define xarg_val_t_decl     , val_t const & val
    #define xarg_val            , val
    #define xarg_item_t         val_t
    #define xarg_item           val
#else
template<class key_t>
xstruct(
    xspec(node, key_t, void),
    xpubf(key, inc::mirror<key_t>),
    xasso(key_t)
)
    node(key_t const & key, inc::construction_t mode) : 
        key(key, mode), next(nullptr) {
    }

    #define xarg_val_t_decl
    #define xarg_val
    #define xarg_item_t         key_t
    #define xarg_item           key
#endif
    // 结构嵌套，避免误导 GC 判断
    node * next;

    node() : next(this) {}

    hashmap_set_result set(key_t const & key xarg_val_t_decl){
        // 约定 next == this 表示的是空节点，首元 next == nullptr 表示只有首元一个节点
        if (is_empty()){
            new (this) node(key xarg_val, inc::construction_t::execute);
            return hashmap_set_result::success;
        }

        for(auto cur = this; ; cur = cur->next){
            if (cur->key == key){
                cur->xarg_item.assign_with_operator(xarg_item);
                return hashmap_set_result::override;
            }
            if (cur->next == nullptr){
                cur->next   = inc::alloc_with_initial<the_t>(key xarg_val, inc::construction_t::execute);
                return hashmap_set_result::success;
            }
        }
    }

    xarg_item_t & get(key_t const & key){
        if (not is_empty()){
            for(auto cur = this; cur != nullptr; cur = cur->next){
                if (cur->key == key){
                    return cur->xarg_item;
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
            temp->val->~val_t();

            #ifdef xarg_has_val_t
                temp->key->~key_t();
            #endif

            next      = next->next;
            inc::free(temp);
        }

        the.val->~val_t();

        #ifdef xarg_has_val_t
            the.key->~key_t();
        #endif

        the.next = this;
    }

    bool is_empty() const {
        return next == this;
    }
$

#ifdef xarg_has_val_t
template<class key_t, class val_t>
xstruct(
    xtmpl(hashmap_t, key_t, val_t),
    xpubb(inc::self_management),
    xpubb(inc::disable_copy),
    xprif(lines,  uxx),
    xprif(count,  uxx),
    xprif(nodes,  node<key_t, val_t> *)
)
    using node_t   = node<key_t, val_t>;
#else
template<class key_t>
xstruct(
    xspec(hashmap_t, key_t),
    xpubb(inc::self_management),
    xpubb(inc::disable_copy),
    xprif(lines,  uxx),
    xprif(count,  uxx),
    xprif(nodes,  node<key_t> *)
)
    using node_t   = node<key_t>;
#endif
private:
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
    xarg_item_t & get(key_t const & key) const {
        uxx index = addressing(key);
        xdebug(im_docker_hashmap_get, index);
        return nodes[index].get(key);
    }

    inc::transmitter<xarg_item_t> take_out(key_t const & key) {
        auto   index      = addressing(key);
        auto   can_relase = false;
        auto & item       = nodes[index].take_out(key, xref can_relase);

        if (item == inc::nullref){
            return inc::transmitter<xarg_item_t>();
        }
        else{
            count -= 1; // 需要计数========================================
        }

        if (inc::transmitter<xarg_item_t> r = (xarg_item_t &)item.xarg_item; can_relase){
            inc::free_with_destroy(xref item);
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

    the_t & set(key_t const & key xarg_val_t_decl, hashmap_set_result * state = nullptr){
        if (lines == ++count){
            resize(lines * multi);
        }

        auto index = addressing(key);
        xdebug(im_docker_hashmap_set, index, key xarg_val);

        if (auto sta = nodes[index].set(key xarg_val); state != nullptr){
            state[0] = sta;
        }
        return the;
    }

    the_t & take_out(key_t const & key, xarg_item_t * value, hashmap_take_out_result * state = nullptr) {
        auto sta = hashmap_take_out_result::item_not_exist;

        // count -= 1; 注意计数问题========================================
        // 这里间接调用了 count -= 1 的 take_out 则无需考虑此问题
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
        the_t new_hash_map{ capcity };
        the.resize_to(new_hash_map);
        inc::swap(xref new_hash_map, xref the);
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

            #ifdef xarg_has_val_t
            map.nodes[
                map.addressing(header.key)
            ].set(header.key, header.val);
            #else
            map.nodes[
                map.addressing(header.key)
            ].set(header.key);
            #endif

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
            lines * sizeof(node_t)
        ));
        nodes = nullptr;
    }

    static node_t * alloc(uxx counts){
        auto bytes = inc::memory_size(
            counts * sizeof(node_t)
        );
        auto nodes = inc::alloc<node_t>(bytes);

        for (uxx i = 0; i < counts; i++){
            new (nodes + i) node_t();
        }
        return nodes;
    }
$

#ifdef xarg_has_val_t
template<class final, class key_t, class val_t>
struct hashmap : hashmap_t<key_t, val_t> {
    using the_t = hashmap_t<key_t, val_t>;
#else
template<class final, class key_t>
struct hashmap<final, key_t> : hashmap_t<key_t> {
    using the_t = hashmap_t<key_t>;
#endif
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

    final & set(key_t const & key xarg_val_t_decl, hashmap_set_result * state = nullptr) {
        return (final &)the.set(key xarg_val, state);
    }

    final & take_out(key_t const & key, xarg_item_t * val, hashmap_take_out_result * state = nullptr) {
        return (final &)the.take_out(key, val, state);
    }
};

#ifdef xarg_has_val_t
    #undef xarg_has_val_t
#endif
#undef  xarg_val_t_decl
#undef  xarg_val
#undef  xarg_item_t
#undef  xarg_item