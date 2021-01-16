#ifndef xpack_gc_ref
#define xpack_gc_ref
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_ref::inc
#include"define/nullref.hpp"
#include"docker/hashmap.hpp"
#include"dumb/dummy_type.hpp"
#include"dumb/init_by.hpp"
#include"dumb/struct_type.hpp"
#include"gc/self_management.hpp"
#include"gc/private/make_guide.hpp"
#include"gc/private/token.hpp"
#include"gc/private/tuple.hpp"
#include"instruction/index_of_last_set.hpp"
#include"interface/can_callback.hpp"
#include"lock/atom_swap.hpp"
#include"macro/xdebug.hpp"
#include"macro/xnew.hpp"
#include"macro/xis_nullptr.hpp"
#include"memory/allocator.hpp"
#include"memop/addressof.hpp"
#include"memop/cast.hpp"
#include"meta/is_based_on.hpp"
#include"meta/is_same.hpp"
#include"meta/has_cast.hpp"
#include"meta/has_constructor.hpp"
#include"meta_ctr/cif.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tin.hpp"
#include"meta_seq/vlist.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_ref{
    using namespace inc;
    using visited_ptr_t = voidp;

    xstruct(
        xname(info_t)
    )
        uxx can_arrive_root : 1;
        uxx visited         : sizeof(uxx) * 8 - 1;

        info_t() : 
            can_arrive_root(0), visited(0){}

        operator uxx (){
            return uxxp(this)[0];
        }
    $

    extern hashmap<visited_ptr_t, info_t>    gc_map;
    extern uxx                               degree_dvalue;
    extern visited_ptr_t                     root;
    extern bool                              can_free_whole_ring;
    static inline uxx                        empty_mem[32];
    static inline voidp                      empty_mem_ptr   = empty_mem;

    template<class final, class item_t, class attribute_t, bool is_array, bool is_binary_aligned_alloc>
    xstruct(
        xtmpl(meta, final, item_t, attribute_t, is_array, is_binary_aligned_alloc),
        xpubb(self_management),
        xasso(attribute_t),
        xasso(item_t)
    )
    private:
        using the_length =
            typename cif<
                is_array
            >::template select<
                token_plus
            >::template ces<
                token
            >;

        using the_item_t = 
            typename cif<
                is_array
            >::template select<
                item_t
            >::template ces<
                dummy_type
            >;

        using token_mix_t = token_mix<item_t, attribute_t, the_length>;

        static constexpr bool has_attribute = not inc::is_same<void, attribute_t>;
        static constexpr bool has_array     = not inc::is_same<void, item_t>;

        static token_mix_t * null(){
            return (token_mix_t *)empty_mem_ptr;
        }

        template<class guide>
        bool routing() {
            using tuplep = tuple<attribute_t> *;

            bool can_arrive_root = false;

            // 无法通向 root 的死路
            if (mem == null()){
                return false;
            }

            xdebug(im_gc_meta_routing, mem, xtypeid(attribute_t).name, mem->owners(), the.length());

            // 我们把经过的节点存下来
            // 如果是首次经过就让它遍历成员节点
            // 如果该节点可以通往 root，那么曾经拜访过该节点的附和节点也可以通往根节点
            // 实际过程如下：
            // root --> 此节点 --> 附和节点 -> 此节点
            //             |   
            //             +-----> 其他节点 -> root
            // 在此时附和节点不能确定此节点是否还存在可以通往 root 的路径，所以指示暂时的让此节点的 visited 访问计数器加一
            // 而计数汇总的工作则是交给此节点来完成
            if (info_t & info = gc_map.get(mem); info == nullref){
                info_t        this_node;
                this_node.can_arrive_root       = mem == root;
                this_node.visited               = mem != root; // 除了根节点，其他节点都有直接的入边（表示从前一个节点到此节点）
                gc_map.set(mem, this_node);
                xdebug(im_gc_meta_routing, mem, "set to gc_map");

                // 如果数组元素 item_t 属于 guide 类型集合，那么就遍历数组元素
                if constexpr (tin<guide, item_t>){
                    can_arrive_root            |= tuple<void>::template routing<guide>(the, the.length());
                }
                
                // 如果 attribute_t 属于 guide 类型集合，就遍历该对象的字段
                if constexpr (tin<guide, attribute_t>){
                    attribute_t * ptr           = mem; // 李氏转换
                    can_arrive_root            |= tuplep(ptr)->template routing<guide>();
                }

                if (can_arrive_root){
                    info_t & this_node          = gc_map.get(mem);
                    this_node.can_arrive_root   = true;
                    degree_dvalue              += mem->owners() - this_node.visited;
                }
            }
            else if (info.can_arrive_root){
                xdebug(im_gc_meta_routing, "can_arrive_root");
                degree_dvalue                  -= 1;
                can_arrive_root                 = true;
            }
            else{
                info.visited                   += 1;
                xdebug(im_gc_meta_routing, info.visited);
            }
            return can_arrive_root;
        }

        template<class guide>
        bool can_release(){
            bool    state;
            root            = mem;

            // not can_arrive_root
            if (not the.template routing<guide>()){
                return false;
            }

            state           = degree_dvalue == 0;
            degree_dvalue   = 0;
            return state;
        }
    public:
        meta() : mem(null()) { }

        meta(the_t const & object){
            object.mem->owners_inc();
            mem     = object.mem;
        }

        meta(the_t && object){
            the.mem = atom_swap(& object.mem, null());
        }
    protected:
        using item_initial_invoke   = icallback<void(the_item_t * item_ptr)>;
        using item_initial_invokex  = icallback<void(uxx i, the_item_t * item_ptr)>;

        template<class initial_invoke, class ... args>
        void init(::length length, init_by<args...> const & init_attr, initial_invoke const & init_ary) {
            mem     = alloc(length, init_attr);

            for(uxx i = 0; i < length; i++) {
                if constexpr (inc::has_cast<item_initial_invoke, initial_invoke>){
                    init_ary(mem->item_ptr(i));
                }
                else{
                    init_ary(i, mem->item_ptr(i));
                }
            }
        }

        template<class ... args, class ... argsx>
        void init_one_by_one(init_by<args...> const & init_attr, the_item_t const & first, argsx const & ... rest){
            struct item_ref{
                the_item_t const & value;
                item_ref(the_item_t const & value) : value(value){}
            } items[] = {first, rest...};

            mem     = alloc(1 + sizeof...(rest), init_attr);

            for(uxx i = 0, count = 1 + sizeof...(rest); i < count; i++) {
                xnew(mem->item_ptr(i)) the_item_t(items[i].value);
            }
        }

        template<class ... args>
        requires(can_init<attribute_t, init_by<args...>> == true)
        void init_ptr(init_by<args...> const & init) {
            mem     = alloc(::length(0), init);
        }

        ~meta(){
            the = nullptr;
        }
    public:
        final & operator = (the_t const & object){
            /* 当 object == nullptr 时，实际上指向的是 the_t::null()
             * 对外而言依旧与 nullptr 比较判断是否是空对象
             * the_t::null() 是一块不为 nullptr 可用的内存，只是它不能当作对象实例使用
             * 具体情况：
             * 对于该函数 operator=(the_t const & object)
             * 假如 object.mem 为空时指向的是 nullptr
             * 那么在增加计数器时我们需要额外判断它是否为 nullptr
             * 此外下列步骤在多线程环境并不安全：
             * if (object.mem != nullptr){      // 前一刻不为 nullptr
             *     object.mem->owners_inc();    // 后一刻为 nullptr
             * }
             * 
             * 所以我们用静态的空对象 null() 代替 nullptr
             * 这样我们我们可以放心的直接增加计数器并且消除了线程不一致问题
             */
            object.mem->owners_inc();

            // 当有多个线程对该对象赋值时，该原子操作可以保证正确性
            the_t         old;
            token_mix_t * m     = atom_swap(& the.mem, object.mem);
            old.mem             = m;
            return thex;
        }

        final & operator = (the_t && object){
            the_t         old;
            token_mix_t * m     = atom_swap(& the.mem, object.mem);
            old.mem             = m;

            // 移除 object 对当前内存的所有权(右值移动)
            atom_swap(& object.mem, null());
            return thex;
        }

        xis_nullptr(mem == null());

        final & operator=(decltype(nullptr)){
            using guide = decltype(make_guide<final>());
            constexpr bool need_gc = guide::length != 0;
            token_mix_t *  ptr = null();
            uxx            cnt;

            if (ptr = atom_swap(& mem, ptr); ptr == null()) { // enter only once
                return thex;
            }

            // 后面的代码可以推送给后台 gc 线程
            auto old = (the_t *)& ptr;

            if (can_free_whole_ring){
                if constexpr (not need_gc){
                    if (ptr->owners_dec() == 0){
                        old->free();
                    }
                }
                else if (auto && i = gc_map.take_out(ptr); i.has_hold_value()){
                    if (i.can_arrive_root){
                        old->free();
                    }
                }
                return thex;
            }

            cnt = ptr->owners_dec();
            xdebug(im_gc__meta, xtypeid(attribute_t).name, cnt, ptr);

            if (cnt == 0){
                old->free();
            }
            else if constexpr (need_gc){
                if (old->template can_release<guide>()){
                    can_free_whole_ring = true;
                    gc_map.take_out(ptr);
                    old->free();
                    can_free_whole_ring = false;
                }
                gc_map.clear();
            }
            return thex;
        }

        bool operator == (the_t const & object) const {
            return mem == object.mem;
        }

        bool operator != (the_t const & object) const {
            return mem != object.mem;
        }

        void swap(the_t * object) {
            object->mem = atom_swap(& mem, object->mem);
        }

    protected:
        operator item_t * () const {
            return mem[0].item_ptr(0);
        }

        attribute_t * operator->() const {
            if constexpr (has_attribute){
                return mem->attribute_ptr();
            }
            else{
                return nullptr;
            }
        }

        uxx length() const {
            return mem->this_length();
        }

        void length(uxx value) const {
            mem->this_length(value);
        }

        constexpr static uxx header_size(){
            return sizeof(token_mix_t);
        }

        uxx capacity() const {
            return real(mem->this_length());
        }
    private:
        mutable token_mix_t * mem;

        static uxx real(uxx length) {
            if constexpr (is_binary_aligned_alloc){
                // length       capacity
                // 0        ->  1 
                // 1        ->  2
                // 2        ->  4
                // 3        ->  4
                // 4        ->  8
                // 5        ->  8
                // 6        ->  8
                // ...
                length    = uxx(1) << (index_of_last_set(length) + 1);
            }
            return length;
        }

        static memory_size size(uxx length) {
            if constexpr (has_array){
                return memory_size(
                    sizeof(token_mix_t) + length * sizeof(item_t)
                );
            }
            else{
                return memory_size(
                    sizeof(token_mix_t)
                );
            }
        }

        template<class ... args>
        static auto alloc(uxx length, init_by<args...> const & init) {
            // 这里只初始化不带属性的部分（引用计数器（*必选）、数组长度（*可选））
            auto real_length    = real(length);
            auto mem            = (token_mix_t *)alloc_with_initial<typename token_mix_t::base_t>(size(real_length), length);

            // 再根据实际情况初始化属性部分
            if constexpr (has_attribute){
                init.make(mem->attribute_ptr());
            }
            return mem;
        }

        void free() const {
            uxx length = capacity();

            // 先析构
            mem->~token_mix_t();

            // 再根据实际内存大小释放
            inc::free(mem, size(length));
        }
    $

    #define xgen(name)                                          \
        name()                              = default;          \
        name(the_t const &)                 = default;          \
        name(the_t &&)                      = default;          \
        the_t & operator=(the_t const &)    = default;          \
        the_t & operator=(the_t &&)         = default;          \

    template<class final, class attribute_t>
    xstruct(
        xtmpl(ref_ptr, final, attribute_t),
        xpubb(meta<final, void, attribute_t, false, false>)
    )
        using base_t = meta<final, void, attribute_t, false, false>;

        xgen(ref_ptr)

        template<class ... args>
        requires(can_init<attribute_t, init_by<args...>> == true)
        ref_ptr(init_by<args...> const & init){
            base_t::init_ptr(init);
        }
    $

    template<
        class   final, 
        class   item_t, 
        class   attribute_t, 
        bool    is_binary_aligned_alloc
    >
    xstruct(
        xtmpl(ref_array, final, item_t, attribute_t, is_binary_aligned_alloc),
        xpubb(meta<final, item_t, attribute_t, true, is_binary_aligned_alloc>)
    )
        using base_t = meta<final, item_t, attribute_t, true, is_binary_aligned_alloc>;
        using typename base_t::item_initial_invoke;
        using typename base_t::item_initial_invokex;

        xgen(ref_array)

        template<class initial_invoke, class ... args>
        requires(
            (
                has_cast<item_initial_invoke , initial_invoke> or 
                has_cast<item_initial_invokex, initial_invoke>
            )   and
            (
                can_init<attribute_t, init_by<args...>>
            )
        )
        ref_array(::length length, init_by<args...> const & init_attr, initial_invoke const & init_ary){
            base_t::init(length, init_attr, init_ary);
        }

        template<class ... args, class ... argsx>
        requires(
            can_init<attribute_t, init_by<args...>> and ... and 
            has_cast<item_t, argsx>
        )
        ref_array(init_by<args...> const & init_attr, item_t const & first, argsx const & ... rest){
            base_t::init_one_by_one(init_attr, first, rest...);
        }
    $

    template<
        class   final, 
        class   item_t, 
        bool    is_binary_aligned_alloc
    >
    xstruct(
        xspec(ref_array, final, item_t, void, is_binary_aligned_alloc),
        xpubb(meta<final, item_t, void, true, is_binary_aligned_alloc>)
    )
        using base_t = meta<final, item_t, void, true, is_binary_aligned_alloc>;
        using typename base_t::item_initial_invoke;
        using typename base_t::item_initial_invokex;

        xgen(ref_array)

        template<class initial_invoke, class ... args>
        requires(
            has_cast<item_initial_invoke , initial_invoke> or 
            has_cast<item_initial_invokex, initial_invoke>
        )
        ref_array(::length length, initial_invoke const & init_ary){
            base_t::init(length, init_by_default, init_ary);
        }

        template<class ... args>
        requires(... and has_cast<item_t, args>)
        ref_array(item_t const & first, args const & ... rest){
            base_t::init_one_by_one(init_by_default, first, rest...);
        }
    $

    #undef  xgen
}

namespace mixc::gc_ref::origin{
    using inc::init_by;
    using mixc::gc_ref::ref_ptr;
    using mixc::gc_ref::ref_array;
}

#endif

xexport_space(mixc::gc_ref::origin)