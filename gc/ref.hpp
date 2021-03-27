#ifndef xpack_gc_ref
#define xpack_gc_ref
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_ref::inc
#include"concurrency/lock/atom_swap.hpp"
#include"configure/switch.hpp"
#include"define/nullref.hpp"
#include"dumb/dummy_type.hpp"
#include"dumb/init_by.hpp"
#include"gc/self_management.hpp"
#include"gc/private/background.hpp"
#include"gc/private/make_guide.hpp"
#include"gc/private/token.hpp"
#include"gc/private/tuple.hpp"
#include"instruction/index_of_last_set.hpp"
#include"interface/can_callback.hpp"
#include"macro/xdebug.hpp"
#include"macro/xnew.hpp"
#include"macro/xis_nullptr.hpp"
#include"memop/addressof.hpp"
#include"meta/has_cast.hpp"
#include"meta/is_same.hpp"
#include"meta_seq/tin.hpp"
#include"meta_seq/tkv.hpp"
#include"mixc.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_ref{
    using namespace inc;

    // 代替 nullptr 作为空对象
    inline uxx                 empty_mem[32];
    inline voidp               empty_mem_ptr       = empty_mem;

    template<
        class final_t, 
        class item_t, 
        class attribute_t, 
        bool  is_array_v, 
        bool  is_binary_aligned_alloc_v
    >
    xstruct(
        xtmpl(meta, final_t, item_t, attribute_t, is_array_v, is_binary_aligned_alloc_v),
        xpubb(self_management),
        xasso(attribute_t),
        xasso(item_t)
    )
    private:
        static auto configure(){
            if constexpr (is_array_v){
                return inc::tkv<inc::token_plus, item_t>{};
            }
            else{
                return inc::tkv<inc::token, inc::dummy_type>{};
            }
        }

        using length_t                      = typename decltype(configure())::key;
        using converted_item_t              = typename decltype(configure())::val;
        using token_mix_t                   = inc::token_mix<item_t, attribute_t, length_t>;

        static constexpr bool has_attribute = not is_same<void, attribute_t>;
        static constexpr bool has_array     = not is_same<void, item_t>;

        static token_mix_t * null(){
            return (token_mix_t *)empty_mem_ptr;
        }

        template<class guide_t>
        bool routing() {
            using tuplep = inc::tuple<attribute_t> *;

            auto ptr                = the.mem;
            auto can_arrive_root    = false;

            // 无法通向 root
            if (ptr == null()){
                return false;
            }

            xdebug(im_gc_meta_routing, ptr, xtypeid(attribute_t).name, ptr->owners(), the.length());

            // 我们把经过的节点存下来
            // 如果是首次经过就让它遍历成员节点
            // 如果该节点可以通往 root，那么曾经拜访过该节点的附和节点也可以通往根节点
            // 实际过程如下：
            // root --> 此节点 --> 附和节点 -> 此节点
            //             |
            //             +-----> 其他节点 -> root
            // 在此时附和节点不能确定此节点是否还存在可以通往 root 的路径，所以指示暂时的让此节点的 visited 访问计数器加一
            // 而计数汇总的工作则是交给此节点来完成
            inc::gc_info_t & info   = inc::gc_map.get(ptr); 
            inc::gc_info_t   gi;

            if (info != nullref){
                if (info.can_arrive_root){
                    xdebug(im_gc_meta_routing, "can_arrive_root");

                    // 这里必须设置 degree_dvalue -= 1 而不是 visited += 1
                    // 因为后续可能不一定会有关于该节点的 owners - visited
                    degree_dvalue -= 1;
                    return true;
                }
                else{
                    info.visited   += 1;
                    xdebug(im_gc_meta_routing, info.visited);
                    return false;
                }
            }

            // 从非根节点开始计算
            gi.can_arrive_root      = ptr == root;
            gi.visited              = ptr != root;
            ptr->in_gc_queue(false);
            inc::gc_map.set(ptr, gi);
            xdebug(im_gc_meta_routing, ptr, "set to gc_map");

            // 如果数组元素 item_t 属于 guide_t 类型集合，那么就遍历数组元素
            if constexpr (tin<guide_t, item_t>){
                can_arrive_root    |= tuple<void>::template routing<guide_t>(the, the.length());
            }
            
            // 如果 attribute_t 属于 guide_t 类型集合，就遍历该对象的字段
            if constexpr (tin<guide_t, attribute_t>){
                attribute_t * attr  = ptr; // 里氏转换
                can_arrive_root    |= tuplep(attr)->template routing<guide_t>();
            }

            if (can_arrive_root){
                inc::gc_info_t & gi = inc::gc_map.get(ptr); 
                inc::degree_dvalue += ptr->owners() - gi.visited;
                gi.can_arrive_root  = true;
            }
            return can_arrive_root;
        }

        template<class guide_t>
        bool can_release(){
            inc::root               = mem;
            inc::degree_dvalue      = 0;

            // 只有可以再次回到根节点 degree_dvalue 的值才有意义
            if (not the.template routing<guide_t>()){
                return false;
            }
            return inc::degree_dvalue == 0;
        }
    public:
        meta() : mem(null()) { }

        meta(the_t const & object){
            auto m                  = object.mem;
            mem                     = m;
            m->owners_increase();
        }

        meta(the_t && object) : 
            mem(
                atom_swap(xref object.mem, null())
            ){
        }
    protected:
        // 对外隐藏析构函数
        ~meta(){
            the                     = nullptr;
        }

        using item_initial_invoke   = icallback<void(converted_item_t * item_ptr)>;
        using item_initial_invokex  = icallback<void(uxx i, converted_item_t * item_ptr)>;

        template<class initial_invoke_t, class ... args_t>
        void init(::length length, init_by<args_t...> const & init_attr, initial_invoke_t const & init_ary) {
            mem                     = the_t::alloc(length, init_attr);

            for(uxx i = 0; i < length; i++) {
                if constexpr (inc::has_cast<item_initial_invoke, initial_invoke_t>){
                    init_ary(mem->item_ptr(i));
                }
                else{
                    init_ary(i, mem->item_ptr(i));
                }
            }
        }

        template<class ... args_t, class ... argsx_t>
        void init_one_by_one(
            init_by<args_t...>  const &     init_attr, 
            converted_item_t    const &     first, 
            argsx_t             const & ... rest){

            struct item_ref{
                converted_item_t const & value;
                item_ref(converted_item_t const & value) : value(value){}
            } items[] = {first, rest...};

            mem                     = the_t::alloc(1 + sizeof...(rest), init_attr);

            for(uxx i = 0, count = 1 + sizeof...(rest); i < count; i++) {
                xnew(mem->item_ptr(i)) converted_item_t(items[i].value);
            }
        }

        template<class ... args_t>
        requires(can_init<attribute_t, init_by<args_t...>> == true)
        void init_ptr(init_by<args_t...> const & init) {
            mem                     = the_t::alloc(::length(0), init);
        }

        template<class guide_t, bool need_gc_v>
        static void release(voidp ptr){
            auto mem                = (token_mix_t *)ptr;
            auto old                = (the_t *)& mem;
            auto owners             = (mem->owners());

            // 只要计数器为 0 就可以直接释放
            // 有些[潜质类型]可能不存在环，但是此时计数器为 0
            if (owners == 0){
                inc::in_release     = true;
                the_t::free_with_destroy(mem);
                inc::in_release     = false;
                return;
            }

            if constexpr (not need_gc_v){
                ; // pass
            }
            // 如果可以释放
            // 设置 thread_local 变量 in_release 
            // 让成员变量析构操作不再推送到 gc_thread
            else if (old->template can_release<guide_t>()){
                auto & gi           = inc::gc_map.get(mem);
                inc::in_release     = true;
                gi.can_arrive_root  = false;
                the_t::free_with_destroy(mem);
                inc::in_release     = false;
            }
        }
    public:
        xis_nullptr(
            mem == the_t::null()
        );

        final_t & operator= (the_t const & object){
            /* 当 object == nullptr 时，实际上指向的是 the_t::null()
             * 对外而言依旧与 nullptr 比较判断是否是空对象
             * the_t::null() 是一块不为 nullptr 可用的内存，只是我们不应该访问它里面的内容
             * 具体情况：
             * 对于该函数 operator=(the_t const & object)
             * 假如 object.mem 为空时指向的是 nullptr
             * 那么在增加计数器时我们需要额外判断它是否为 nullptr
             * 此外下列步骤在多线程环境并不安全：
             * 
             * // 前一刻不为 nullptr
             * if (object.mem != nullptr){
             *     // 后一刻在其他线程释放了
             *     // 由于 cache 的存在，当前 cpu 可能看不见其他 cpu 对它的改动
             *     object.mem->owners_increase();   
             * }
             * 
             * 所以我们用静态的空对象 null() 代替 nullptr
             * 这样我们我们可以放心的直接增加计数器并且消除了线程不一致问题
             */

            // 避免 debug 模式多次从内存中访问 object.mem
            // object.mem 前后可能不一致
            using guide             = decltype(make_guide<final_t>());
            constexpr bool need_gc  = guide::length != 0;
            token_mix_t * om        = object.mem;

            if (om == mem){
                return thex;
            }

            om->owners_increase();

            // 当有多个线程对该对象赋值时，该原子操作可以保证正确性
            the_t         old{};
            token_mix_t * m         = inc::atom_swap(& the.mem, om);

            // 把原先的 the.mem 交给 old 进行析构操作
            old.mem                 = m;
            return thex;
        }

        final_t & operator=(the_t && object){
            the_t         old{};
            token_mix_t * m;

            // 先夺取 object.mem
            m                       = inc::atom_swap(xref object.mem, the_t::null());

            // 即使其他线程也想与 the.mem 交换
            // 后交换的线程会把先交换的内容挤出来放到 m 中
            // 由 old 析构
            m                       = inc::atom_swap(xref the.mem, m);
            old.mem                 = m;
            return thex;
        }

        final_t & operator=(decltype(nullptr)){
            using guide             = decltype(make_guide<final_t>());
            using thep              = the_t *;
            constexpr bool need_gc  = guide::length != 0;
            token_mix_t *  ptr      = the_t::null();
            the_t       *  old;
            uxx            owners;

            if (ptr = atom_swap(& mem, ptr); ptr == the_t::null()) {
                return thex;
            }
            else{
                old                 = thep(& ptr);
            }

            // in_release 是一个 thread_local 变量
            // 非 gc_thread 线程 in_release 永远是 false
            // 前台[外析构]的剩余步骤会推送到 gc_thread 后台线程执行
            // 后台 gc_thread 线程执行[内析构]前会将 in_release 置 true
            // 表示[内析构]直接执行而不推送到自身的 gc_que 队列中
            // 这么做保证了释放的顺序，同时避免了 gc_que 存满时的死锁
            if (not in_release){
                gc_push(ptr, xref the_t::release<guide, need_gc>);
                return thex;
            }

            // 对于[平凡类型]只要计数器为 0 就可以[内析构]
            if constexpr (not need_gc){
                if (owners = ptr->owners_decrease(); owners == 0){
                    the_t::free_with_destroy(ptr);
                }
            }
            // 此节点可能不属于当前[类型环]
            // 遍历子节点，如果可以释放就执行[内析构]操作
            else if (auto & i = inc::gc_map.get(ptr); i == nullref){
                if (owners = ptr->owners_decrease(); owners == 0){
                    the_t::free_with_destroy(ptr);
                }
                else if (old->template can_release<guide>()){
                    auto  & gi          = inc::gc_map.get(ptr);
                    gi.can_arrive_root  = false; // 避免重入
                    the_t::free_with_destroy(ptr);
                }
            }
            // 如果 gc 经过了该节点，且它可以抵达根节点
            else {
                if (i.can_arrive_root){
                    i.can_arrive_root   = false; // 避免重入
                    the_t::free_with_destroy(ptr);
                }
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
            object->mem = the_t::atom_swap(& mem, object->mem);
        }

    protected:
        operator item_t * () const {
            return mem->item_ptr(0);
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

        uxx capacity() const {
            return the_t::real(mem->this_length());
        }
    private:
        mutable token_mix_t * mem;

        static uxx real(uxx length) {
            if constexpr (is_binary_aligned_alloc_v){
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

        static inc::memory_size size(uxx length) {
            if constexpr (has_array){
                return inc::memory_size(
                    sizeof(token_mix_t) + length * sizeof(item_t)
                );
            }
            else{
                return inc::memory_size(
                    sizeof(token_mix_t)
                );
            }
        }

        template<class ... args_t>
        static auto alloc(uxx length, init_by<args_t...> const & init) {
            // 这里只初始化不带属性的部分（引用计数器（*必选）、数组长度（*可选））
            auto real_length    = (the_t::real(length));
            auto mem            = (token_mix_t *)inc::alloc_with_initial<typename token_mix_t::base_t>(
                the_t::size(real_length), length
            );

            // 再根据实际情况初始化属性部分
            if constexpr (has_attribute){
                init.make(mem->attribute_ptr());
            }
            return mem;
        }

        static void free_with_destroy(token_mix_t * mem) {
            // 先析构
            mem->~token_mix_t();

            // 添加到释放列表，在完成一轮 gc 后再释放
            // 避免在对下一个和 mem 指向相同元素 gc 的时候，访问了释放的内存
            // 注意：这里修改该 mem 中的内容，也相当于间接调用了 in_gc_queue(false)
            auto node       = free_nodep(mem);
            auto length     = the_t::real(mem->this_length());
            node->bytes     = the_t::size(length);
            node->previous  = free_list;
            free_list       = node;
        }
    $

    #define xgen(name)                                          \
        name()                              = default;          \
        name(the_t const &)                 = default;          \
        name(the_t &&)                      = default;          \
        the_t & operator=(the_t const &)    = default;          \
        the_t & operator=(the_t &&)         = default;          \

    template<class final_t, class attribute_t>
    xstruct(
        xtmpl(ref_ptr, final_t, attribute_t),
        xpubb(meta<final_t, void, attribute_t, false, false>)
    )
        using base_t = meta<final_t, void, attribute_t, false, false>;

        xgen(ref_ptr)

        template<class ... args_t>
        requires(can_init<attribute_t, init_by<args_t...>> == true)
        ref_ptr(init_by<args_t...> const & init){
            base_t::init_ptr(init);
        }
    $

    template<
        class   final_t, 
        class   item_t, 
        class   attribute_t, 
        bool    is_binary_aligned_alloc_v
    >
    xstruct(
        xtmpl(ref_array, final_t, item_t, attribute_t, is_binary_aligned_alloc_v),
        xpubb(meta<final_t, item_t, attribute_t, true, is_binary_aligned_alloc_v>)
    )
    public:
        using base_t = meta<final_t, item_t, attribute_t, true, is_binary_aligned_alloc_v>;
        using typename base_t::item_initial_invoke;
        using typename base_t::item_initial_invokex;
    private:
        template<class invoke_t>
        static constexpr bool is_match = 
            has_cast<item_initial_invoke, invoke_t> or 
            has_cast<item_initial_invokex, invoke_t>;
    public:
        xgen(ref_array)

        template<class initial_invoke_t, class ... args_t>
        requires(
            (is_match<initial_invoke_t>) and
            (can_init<attribute_t, init_by<args_t...>>)
        )
        ref_array(::length length, init_by<args_t...> const & init_attr, initial_invoke_t const & init_ary){
            base_t::init(length, init_attr, init_ary);
        }

        template<class ... args_t, class ... argsx>
        requires(
            (... and has_cast<item_t, argsx>) and
            (can_init<attribute_t, init_by<args_t...>>)
        )
        ref_array(init_by<args_t...> const & init_attr, item_t const & first, argsx const & ... rest){
            base_t::init_one_by_one(init_attr, first, rest...);
        }
    $

    template<
        class   final_t, 
        class   item_t, 
        bool    is_binary_aligned_alloc_v
    >
    xstruct(
        xspec(ref_array, final_t, item_t, void, is_binary_aligned_alloc_v),
        xpubb(meta<final_t, item_t, void, true, is_binary_aligned_alloc_v>)
    )
        using base_t = meta<final_t, item_t, void, true, is_binary_aligned_alloc_v>;
        using typename base_t::item_initial_invoke;
        using typename base_t::item_initial_invokex;

        xgen(ref_array)

        template<class initial_invoke_t, class ... args_t>
        requires(
            has_cast<item_initial_invoke , initial_invoke_t> or 
            has_cast<item_initial_invokex, initial_invoke_t>
        )
        ref_array(::length length, initial_invoke_t const & init_ary){
            base_t::init(length, default_init_by, init_ary);
        }

        template<class ... args_t>
        requires(... and has_cast<item_t, args_t>)
        ref_array(item_t const & first, args_t const & ... rest){
            base_t::init_one_by_one(default_init_by, first, rest...);
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