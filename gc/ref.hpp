#ifndef xpack_gc_ref
#define xpack_gc_ref
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::gc_ref
    #include"define/base_type.hpp"
    #include"define/nullref.hpp"
    #include"docker/hashmap.hpp"
    #include"dumb/dummy_t.hpp"
    #include"dumb/struct_t.hpp"
    #include"gc/self_management.hpp"
    #include"gc/private/make_guide.hpp"
    #include"gc/private/token.hpp"
    #include"gc/private/tuple.hpp"
    #include"lock/atom_swap.hpp"
    #include"macro/xdebug.hpp"
    #include"macro/xis_nullptr.hpp"
    #include"macro/xstruct.hpp"
    #include"memory/allocator.hpp"
    #include"memop/addressof.hpp"
    #include"memop/cast.hpp"
    #include"meta/is_class.hpp"
    #include"meta_ctr/cif.hpp"
    #include"meta_seq/tlist.hpp"
    #include"meta_seq/tin.hpp"
    #include"meta_seq/vlist.hpp"
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

        inline static hashmap<visited_ptr_t, info_t>    gc_map;
        inline static uxx                               degree_dvalue;
        inline static visited_ptr_t                     root;
        inline static bool                              need_free_whole_ring;

        template<class impl, class item, class attribute, bool is_array>
        xstruct(
            xtmpl(meta, impl, item, attribute, is_array),
            xpubb(self_management),
            xasso(attribute),
            xasso(item)
        )
            using the_length  = typename cif<is_array, token_plus, token>::result;
            using token_mix_t = token_mix<item, attribute, the_length>;

            template<class guide>
            bool routing() {
                using tuplep = tuple<attribute> *;

                bool can_arrive_root = false;

                // 无法通向 root 的死路
                if (mem == nullptr){
                    return false;
                }

                xdebug(im_gc_meta_routing, mem, xtypeid(attribute).name, mem->owners(), the.length());

                if constexpr (tin<guide, item>){
                    using tuplep = tuple<item> *;

                    for(uxx i = 0; i < the.length(); i++){
                        can_arrive_root |= tuplep(xref the[i])->template routing<guide>();
                    }
                }

                // 我们把经过的节点存下来
                // 如果是首次经过就让它遍历成员节点
                // 如果该节点可以通往 root，那么曾经拜访过该节点的附和节点也可以通往根节点
                // 实际过程如下：
                // root --> 此节点 --> 附和节点 -> 此节点
                //             |   
                //             +-----> 其他节点 -> root
                // 在此时附和节点不能确定此节点是否还存在可以通往 root 的路径，所以指示暂时的让此节点的 visited 访问计数器加一
                // 而计数汇总的工作则是交给此节点来完成
                if (auto & info = gc_map.get(mem); info == nullref){
                    info_t      this_node;
                    attribute * ptr             = mem; // 李氏转换
                    this_node.can_arrive_root   = mem == root;
                    this_node.visited           = mem != root; // 除了根节点，其他节点都有直接的入边（表示从前一个节点到此节点）
                    gc_map.set(mem, this_node);
                    xdebug(im_gc_meta_routing, mem, "set to gc_map");
                    can_arrive_root |= tuplep(ptr)->template routing<guide>();

                    if (can_arrive_root){
                        auto & this_node            = gc_map.get(mem);
                        this_node.can_arrive_root   = true;
                        degree_dvalue              += mem->owners() - this_node.visited;
                    }
                }
                else if (info.can_arrive_root){
                    xdebug(im_gc_meta_routing, "can_arrive_root");
                    degree_dvalue  -= 1;
                    can_arrive_root = true;
                }
                else{
                    info.visited   += 1;
                    xdebug(im_gc_meta_routing, info.visited);
                }
                return can_arrive_root;
            }

            template<class guide>
            bool can_release(){
                bool    state;
                root                    = mem;

                // not can_arrive_root
                if (not the.template routing<guide>()){
                    return false;
                }

                state                   = degree_dvalue == 0;
                degree_dvalue           = 0;
                return state;
            }
        public:
            meta() : mem(nullptr) { }

            template<class ... args>
            meta(::length length, args const & ... list) {
                mem = alloc(length);

                for (uxx i = 0; i < length; i++) {
                    new (mem->ptr(i)) item(list...);
                }
            }

            template<class ... args>
            meta(ini, args const & ... list) {
                mem = alloc(::length(0), list...);
            }

            meta(the_t const & value){
                if (mem = value.mem; mem != nullptr) {
                    mem->owners_inc();
                }
            }
        protected:
            ~meta(){
                using guide = decltype(make_guide<impl>());
                constexpr bool need_gc = guide::length != 0;
                token_mix_t *  ptr = nullptr;
                uxx            cnt;

                if (ptr = atom_swap(& mem, ptr); ptr == nullptr) { // enter only once
                    return;
                }

                auto & old = cast<the_t>(ptr);
                cnt        = ptr->owners_dec();
                xdebug(im_gc__meta, xtypeid(attribute).name, cnt, ptr);

                if constexpr (not need_gc){
                    if (cnt == 0){
                        old.free();
                    }
                }
                else if (need_free_whole_ring){
                    if (auto && i = gc_map.take_out(ptr); i.has_hold_value() and i.can_arrive_root){
                        old.free();
                    }
                }
                else if (cnt == 0){
                    old.free();
                }
                else if (old.template can_release<guide>()){
                    need_free_whole_ring = true;
                    gc_map.take_out(ptr);
                    old.free();
                    gc_map.clear();
                    need_free_whole_ring = false;
                }
                else{
                    gc_map.clear();
                }
            }

            auto operator -> () const{
                return mem;
            }
        public:
            impl & operator = (the_t const & value){
                if (value.mem){ 
                    value->owners_inc();
                }
                cast<the_t>(
                    atom_swap(& mem, value.mem)
                ).~meta();
                return (impl &)this[0];
            }

            xis_nullptr(mem == nullptr);

            bool operator == (the_t const & value) const {
                return mem == value.mem;
            }

            bool operator != (the_t const & value) const {
                return mem != value.mem;
            }

            void swap(the_t * values) {
                values->mem = atom_swap(& mem, values->mem);
            }

        protected:
            operator item * () {
                return xref mem[0][0];
            }

            operator item const * () const {
                return xref mem[0][0];
            }

            item & operator [] (uxx index){
                return mem[0][index];
            }

            const item & operator [] (uxx index) const {
                return mem[0][index];
            }

            uxx length() const {
                return mem->this_length();
            }

            constexpr static uxx header_size(){
                return sizeof(token_mix_t);
            }
        private:
            token_mix_t * mem;

            template<class ... args> auto alloc(uxx length, args const & ... list) {
                return alloc_with_initial<token_mix_t>(
                    memory_size(
                        sizeof(token_mix_t) + length * sizeof(item)
                    ),
                    length, list...
                );
            }

            void free(){
                free_with_destroy(mem,
                    memory_size(
                        sizeof(token_mix_t) + mem->this_length() * sizeof(item)
                    )
                );
            }
        $
    }

    namespace mixc::gc_ref::origin{
        template<class impl, class type>
        using ref_ptr = meta<
            impl, 
            dummy_t, 
            typename cif<
                is_class<type>,
                type,
                struct_t<type>
            >::result,
            false
        >;

        template<class impl, class item, class attribute = void>
        using ref_array = meta<
            impl, 
            item, 
            typename cif<
                is_class<attribute>,
                attribute,
                struct_t<attribute>
            >::result,
            true
        >;
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::gc_ref::origin;
}
