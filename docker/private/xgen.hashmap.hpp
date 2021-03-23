#ifdef xarg_has_val_t
template<class key_t, class val_t>
xstruct(
    xtmpl(node, key_t, val_t),
    xpubf(key , inc::mirror<key_t>),
    xpubf(val , inc::mirror<val_t>),
    xpubf(next, node<key_t, val_t> *),
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
    xpubf(key , inc::mirror<key_t>),
    xpubf(next, node<key_t, void> *),
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

    node() : next(this) {}

    hashmap_set_result_t set(key_t const & key xarg_val_t_decl){
        // 约定 next == this 表示的是空节点，首元 next == nullptr 表示只有首元一个节点
        if (is_empty()){
            xnew (this) node(key xarg_val, inc::construction_t::execute);
            return hashmap_set_result_t::success;
        }

        for(auto cur = this; ; cur = cur->next){
            if ((key_t &)cur->key == (key_t &)key){
                cur->xarg_item.assign_with_operator(xarg_item);
                return hashmap_set_result_t::override;
            }
            if (cur->next == nullptr){
                cur->next   = inc::alloc_with_initial<the_t>(key xarg_val, inc::construction_t::execute);
                return hashmap_set_result_t::success;
            }
        }
    }

    xarg_item_t & get(key_t const & key){
        if (not is_empty()){
            for(auto cur = this; cur != nullptr; cur = cur->next){
                if ((key_t &)cur->key == (key_t &)key){
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
            if ((key_t &)cur->key == (key_t &)key){
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
            key_t * k = temp->key.operator->();
            k->~key_t();

            #ifdef xarg_has_val_t
            val_t * v = temp->val.operator->();
            v->~val_t();
            #endif

            next      = next->next;
            inc::free(temp);
        }

        key_t * k = the.key.operator->();
        k->~key_t();

        #ifdef xarg_has_val_t
        val_t * v = the.val.operator->();
        v->~val_t();
        #endif

        the.next = this;
    }

    bool is_empty() const {
        return next == this;
    }
$

#ifdef  xarg_has_val_t
#define xarg_name  kvhashmap

template<class final_t, inc::has_cmp_equal key_t, class val_t>
xstruct(
    xtmpl(kvhashmap, final_t, key_t, val_t),
    xpubb(inc::disable_copy),
    xpubb(inc::self_management),
    xprif(lines,  uxx),
    xprif(count,  uxx),
    xprif(seed ,  uxx),
    xprif(bmp  ,  inc::bits_indicator<>),
    xprif(nodes,  node<key_t, val_t> *)
)
    using node_t   = node<key_t, val_t>;
#else
#define xarg_name  khashmap

template<class final_t, inc::has_cmp_equal key_t>
xstruct(
    xtmpl(khashmap, final_t, key_t),
    xpubb(inc::disable_copy),
    xpubb(inc::self_management),
    xprif(lines,  uxx),
    xprif(count,  uxx),
    xprif(seed ,  uxx),
    xprif(bmp  ,  inc::bits_indicator<>),
    xprif(nodes,  node<key_t, void> *)
)
    using node_t   = node<key_t, void>;
#endif

private:
    static constexpr uxx multi         = 4;
    static constexpr uxx start_capcity = 16;

    // 构造/析构区
public:
    xarg_name() : xarg_name(start_capcity){}
    xarg_name(uxx start_capcity) : xarg_name(start_capcity, inc::random<uxx>()){}
    xarg_name(uxx start_capcity, uxx seed) : 
        lines(inc::align(start_capcity)), 
        count(0), 
        seed(seed),
        nodes(the_t::alloc(xref bmp, lines)) {
    }
protected:
    ~xarg_name(){
        the.clear();
        the.free();
    }

    template<auto mode_v, class invoke_t>
    void foreach_template(invoke_t const & invoke) const {
        loop_t state = loop_t::go_on;

        for(uxx i = uxx(-1), index = 0; not_exist != (i = bmp.index_of_first_set(i + 1));){
            for(auto cur = xref nodes[i]; cur != nullptr; cur = cur->next){
                #ifdef xarg_has_val_t
                    state = inc::itr_switch<mode_v>(xref index, invoke, (key_t &)cur->key, (val_t &)cur->val);
                #else
                    state = inc::itr_switch<mode_v>(xref index, invoke, (key_t &)cur->key);
                #endif

                if (state == loop_t::finish){
                    return;
                }
            }
        }
    }

    // 接口区
public:
    #ifdef xarg_has_val_t
        xitr_foreach(key_t, val_t)
    #else
        xitr_foreach(key_t)
    #endif

    xarg_item_t & get(key_t const & key) const {
        uxx index = addressing(key);
        xdebug(im_docker_hashmap_get, index);
        return nodes[index].get(key);
    }

    inc::transmitter<xarg_item_t> take_out(key_t const & key, hashmap_take_out_result_t * state = nullptr) {
        auto   tmp_state  = hashmap_take_out_result_t::item_not_exist;
        auto   index      = addressing(key);
        auto   can_relase = false;
        auto & node       = nodes[index];
        auto & item       = node.take_out(key, xref can_relase);

        xdefer{
            if (state != nullptr) {
                state[0] = tmp_state;
            }
        };

        if (item == inc::nullref){
            return inc::transmitter<xarg_item_t>();
        }
        else{
            tmp_state     = hashmap_take_out_result_t::success;
            count        -= 1; // 需要计数
        }

        inc::transmitter<xarg_item_t> r = (xarg_item_t &)item.xarg_item;

        if (can_relase){
            inc::free_with_destroy(xref item);
        }
        if (node.is_empty()){
            bmp.reset(index);
        }
        return r;
    }

    void clear() {
        if (nodes != nullptr){
            for(uxx i = 0; not_exist != (i = bmp.pop_first());){
                nodes[i].free();
            }
            count = 0;
        }
    }

    void remove(key_t const & key, hashmap_remove_result_t * state = nullptr) {
        if (auto && r = tale_out(key); state != nullptr){
            state[0] = r.has_hold_value() ? 
                hashmap_remove_result_t::success :
                hashmap_remove_result_t::item_not_exist;
        }
    }

    void resize() {
        if (lines >= count * multi * 2 and lines > start_capcity){
            resize(lines / multi);
        }
    }

    void set(key_t const & key xarg_val_t_decl, hashmap_set_result_t * state = nullptr){
        auto   index = addressing(key);
        auto & node = nodes[index];

        #ifdef xarg_has_val_t
            xdebug(im_docker_hashmap_set, index, key, val);
        #else
            xdebug(im_docker_hashmap_set, index, key);
        #endif

        if (node.is_empty()){
            bmp.set(index);
        }

        auto sta = node.set(key xarg_val);

        if (state != nullptr){
            state[0] = sta;
        }
        if (sta == hashmap_set_result_t::success){
            count   += 1;
        }
        if (lines == count){
            resize(lines * multi);
        }
    }

    void take_out(key_t const & key, xarg_item_t * value, hashmap_take_out_result_t * state = nullptr) {
        auto sta = hashmap_take_out_result_t::item_not_exist;

        // count -= 1; 注意计数问题
        // 这里间接调用了 count -= 1 的 take_out 则无需考虑此问题
        if (auto && r = take_out(key); r.has_hold_value()){
            value[0]  = r;
            sta       = hashmap_take_out_result_t::success;
        }
        if (state){
            state[0]  = sta;
        }
    }

    bool is_contains(key_t const & key) const {
        return get(key) != inc::nullref;
    }

    // 属性区
public:
    xpubgetx(is_empty, bool){
        return length() == 0;
    }

    xpubgetx(length, uxx) {
        return count;
    }

    // 私有区
private:
    uxx addressing(key_t const & key) const {
        auto hash  = inc::hash(key, seed);
        auto index = hash & mask();
        xdebug(im_docker_hashmap_addressing, voidp(hash), index);
        return index;
    }

    void resize(uxx capcity){
        the_t new_hash_map{ capcity, seed };
        the.resize_to(new_hash_map);
        inc::swap(xref new_hash_map, xref the);
    }

    // 该函数用于 hashmap 内部扩容和压缩
    // 要求 map 是新分配的空间，并且内部无元素
    void resize_to(the_t & map) {
        if (nodes == nullptr){
            return;
        }

        for(uxx i; not_exist != (i = bmp.pop_first());){
            auto   old_head    = xref nodes[i];
            auto   cur         = old_head->next;
            auto   index       = map.addressing(old_head->key);
            auto & new_head    = map.nodes[index];

            // 旧的 hashmap 首元不可以当作普通节点一样挂到新的 hashmap 中
            // 因为它是数组中的一个元素，而不是通过 inc::alloc 分配得到的独立节点
            if (new_head.is_empty()){
                new_head       = old_head[0];
                new_head.next  = nullptr;
                map.bmp.set(index);
            }
            // 只有首元
            else if (auto next = inc::alloc_with_initial<node_t>(*old_head); new_head.next == nullptr){
                new_head.next  = next;
                next->next     = nullptr;
            }
            // 插入到首元后一个
            else{
                next->next     = new_head.next;
                new_head.next  = next;
            }

            // 逻辑清空，next 指向首节点
            for(old_head->next = old_head; nullptr != cur;){
                auto   next    = cur->next; 
                auto   index   = map.addressing(cur->key);
                auto & node    = map.nodes[index];

                if (node.is_empty()){
                    node       = cur[0];
                    node.next  = nullptr;
                    map.bmp.set(index);
                    inc::free(cur);
                }
                else if (node.next == nullptr){
                    node.next  = cur;
                    cur->next  = nullptr;
                }
                else{
                    cur->next  = node.next;
                    node.next  = cur;
                }
                cur            = next;
            }
        }
        map.count = count;
    }

    uxx mask() const {
        return lines - 1;
    }

    void free() {
        inc::free(nodes, inc::memory_size(
            lines * sizeof(node_t) + bmp.cost_bytes()
        ));
        nodes = nullptr;
    }

    static node_t * alloc(inc::bits_indicator<> * bmp, uxx node_count){
        node_t * nodes = nullptr;

        xnew (bmp) inc::bits_indicator<>(node_count,
            [&](uxx length) -> uxx * {
                auto bytes = inc::memory_size(
                    node_count * sizeof(node_t) + sizeof(uxx) * length
                );

                nodes = inc::alloc<node_t>(bytes);

                for (uxx i = 0; i < node_count; i++){
                    xnew (xref nodes[i]) node_t();
                }
                return uxxp(nodes + node_count);
            }
        );
        return nodes;
    }
$

#ifdef xarg_has_val_t
    #undef xarg_has_val_t
#endif

#undef  xarg_val_t_decl
#undef  xarg_val
#undef  xarg_item_t
#undef  xarg_item
#undef  xarg_name
