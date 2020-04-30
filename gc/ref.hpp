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
        #include"macro/xgc.hpp"
        #include"macro/xis_nullptr.hpp"
        #include"memory/allocator.hpp"
        #include"memop/addressof.hpp"
        #include"memop/cast.hpp"
        #include"meta/is_same.hpp"
        #include"meta_ctr/cif.hpp"
        #include"meta_seq/tlist.hpp"
        #include"meta_seq/tin.hpp"
        #include"meta_seq/vlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::gc_ref{
        using namespace inc;
        using visited_ptr_t = voidp;

        struct info_t {
            xgc_fields(
                xiam(info_t)
            );
        public:
            uxx can_arrive_root : 1;
            uxx visited         : sizeof(uxx) * 8 - 1;

            info_t() : 
                can_arrive_root(0), visited(0){}

            operator uxx (){
                return uxxp(this)[0];
            }
        };

        struct empty_t : token_plus{
            byte empty[48] = { 0 };

            empty_t() : token_plus(0) {}

            template<class type>
            operator type const & () const {
                auto self = this;
                return inc::cast<type>(self);
            }
        };

        inline hashmap<visited_ptr_t, info_t> gc_map;
        inline empty_t const empty_array;

        template<class impl, class item, class attribute = dummy_t, bool is_array = false> struct meta;
        template<class impl, class item, class attribute, bool is_array>
        struct meta : self_management {
            using the_length  = typename cif<is_array, token_plus, token>::result;
            using token_mix_t = token_mix<item, attribute, the_length>;

            xgc_fields(
                xiam(meta<impl, item, attribute, is_array>),
                xhas(attribute),
                xhas(item)
            ) {
                using tuplep = tuple<attribute, typename attribute::member_list> *;

                if (mem == nullptr){
                    return { 0 }; // no way
                }
                if (auto & info = gc_map.get(mem); info == nullref){
                    routing_result r;
                    attribute *    ptr = mem;
                    info_t    *    i = nullptr;

                    xdebug(im_gc_meta_routing, 
                        mem,
                        xtypeid(attribute).name,
                        "not visited"
                    );

                    gc_map.set(mem, info_t(), xref i);

                    if (r = tuplep(ptr)->template routing<guide>(); r.can_arrive_root){
                        i->can_arrive_root = true;
                        r.degree_dvalue   += mem->owners() - i->visited;
                    }
                    return r;
                }
                else if (info.can_arrive_root){
                    xdebug(im_gc_meta_routing, 
                        mem,
                        xtypeid(attribute).name,
                        info.can_arrive_root,
                        info.visited
                    );
                    return { 1 }; // has a way can arrive root
                }
                else{
                    info.visited += 1;
                    xdebug(im_gc_meta_routing, 
                        mem,
                        xtypeid(attribute).name,
                        info.can_arrive_root,
                        info.visited
                    );
                    return { 0 };
                }
            }
        public:
            meta() : mem(nullptr) { }

            template<class ... args>
            meta(inc::length length, args const & ... list) {
                mem = alloc(length);

                for (uxx i = 0; i < length; i++) {
                    new (mem->ptr(i)) item(list...);
                }
            }

            template<class ... args>
            meta(ini, args const & ... list) {
                mem = alloc(inc::length(0), list...);
            }

            meta(the_t const & value){
                if (mem = value.mem; mem != nullptr) {
                    mem->owners_inc();
                }
            }
        protected:
            ~meta(){
                using guide = decltype(make_guide<impl>());
                constexpr bool need_gc = not is_same<guide, tlist<>>;
                token_mix_t *  tmp = nullptr;
                uxx            cnt;

                if (tmp = atom_swap(& mem, tmp); tmp == nullptr) { // enter only once
                    return;
                }

                auto & old = cast<the_t>(tmp);

                if constexpr (not need_gc){
                    if (cnt = tmp->owners_dec(); cnt == 0){
                        old.free();
                    }
                    xdebug(im_gc_$meta, xtypeid(attribute).name, cnt, tmp);
                }
                else if (old.mem->is_under_free() == false){
                    old.template routing_entry<guide>();
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

            template<class guide> void routing_entry(){
                using tuplep = tuple<attribute, typename attribute::member_list> *;

                if (mem->owners_dec() == 0){
                    the.free();
                    return;
                }

                attribute *    ptr = mem; // lis convert
                routing_result r;
                info_t         info;

                info.can_arrive_root = true;
                gc_map.set(mem, info);
                r                = tuplep(ptr)->template routing<guide>();
                r.degree_dvalue += mem->owners();

                xdebug(im_gc_meta_routing_entry, 
                    xtypeid(attribute).name, 
                    r.degree_dvalue
                );

                gc_map.clear();
                gc_map.resize();

                if (r.degree_dvalue <= 0){
                    the.free();
                }
                else if constexpr (tin<guide, item>){
                    using tuplep = tuple<item, typename item::member_list> *;

                    for(uxx i = 0; i < length(); i++){
                        gc_map.set(mem, info);
                        r                = tuplep(xref the[i])->template routing<guide>();
                        r.degree_dvalue += mem->owners();

                        if (gc_map.clear(); r.degree_dvalue <= 0){
                            the.free();
                            break;
                        }
                    }
                }
            }

            template<class ... args> auto alloc(uxx length, args const & ... list) {
                return alloc_with_initial<token_mix_t>(
                    memory_size(
                        sizeof(token_mix_t) + length * sizeof(item)
                    ),
                    length, list...
                );
            }
            void free(){
                mem->mark_under_free();
                free_with_destroy(mem,
                    memory_size(
                        sizeof(token_mix_t) + mem->this_length() * sizeof(item)
                    )
                );
            }
        };

        template<class impl, class type>
        using ref_ptr = meta<impl, dummy_t, struct_t<type>, false>;

        template<class impl, class item, class attribute = void>
        using ref_array = meta<
            impl, 
            item, 
            struct_t<
                typename cif<
                    is_same<attribute, void>,
                    dummy_t,
                    attribute
                >::result
            >,
            true
        >;
    }

#endif

namespace xuser::inc{
    using ::mixc::gc_ref::ref_ptr;
    using ::mixc::gc_ref::ref_array;
    using ::mixc::gc_ref::empty_array;
}
