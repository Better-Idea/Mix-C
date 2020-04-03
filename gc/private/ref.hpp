#ifndef xpack_gc_ref
#define xpack_gc_ref
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::gc_ref
        #include"define/base_type.hpp"
        #include"gc/private/make_guide.hpp"
        #include"gc/private/routing_result.hpp"
        #include"gc/private/self_management.hpp"
        #include"gc/private/token.hpp"
        #include"gc/private/tuple.hpp"
        #include"lock/atom_swap.hpp"
        #include"macro/xdebug.hpp"
        #include"memory/allocator.hpp"
        #include"memop/cast.hpp"
        #include"memop/copy.hpp"
        #include"meta/is_same.hpp"
        #include"meta_ctr/cif.hpp"
        #include"meta_seq/tlist.hpp"
        #include"meta_seq/vlist.hpp"
        #include"dumb/dummy_t.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::gc_ref{
        using namespace inc;
        constexpr bool releaseable = true;

        template<class impl, class item, class attribute = dummy_t, bool is_array = false> struct meta;
        template<class impl, class item, class attribute, bool is_array>
        struct meta : self_management {
            using self        = meta<impl, item, attribute, is_array>;
            using the_length  = typename cif<is_array, token_plus, token>::result;
            using token_mix_t = token_mix<item, attribute, the_length>;

            // TO BE CONTINUE ========================================================================================
            using member_list = vlist<(attribute self::*)0>;

            meta() : mem(nullptr) { }

            template<class ... args>
            meta(length length, args const & ... list) {
                mem = alloc(length, list...);

                for (uxx i = 0; i < length; i++) {
                    new (mem->ptr(i)) item();
                }
            }


            meta(self const & value){
                if (mem = value.mem; mem != nullptr) {
                    mem->owners_inc();
                }
            }

            ~meta(){
                using guide = decltype(make_guide<impl>());
                constexpr bool      need_gc = not is_same<guide, tlist<>>;
                token_mix_t *       tmp = nullptr;
                guide               gui;
                uxx                 cnt;
                
                if (tmp = atom_swap(& mem, tmp); tmp == nullptr) { // enter only once
                    return;
                }
                else{
                    cnt = tmp->owners_dec();
                }

                xdebug(im_gc_$meta, xtypeid(attribute).name, cnt, tmp);
                auto & old = cast<self>(tmp);

                if constexpr (not need_gc){
                    if (cnt == 0){
                        old.free();
                    }
                }
                else if (old->in_free()){
                    return;
                }
                else if (old.routing_entry(gui) == releaseable){
                    old.clear_footmark(gui);
                    old->set_in_free();
                    old.free();
                }
                else{
                    old.clear_footmark(gui);
                }
            }

            operator item & () const{
                return mem[0];
            }

            auto operator -> () const{
                return mem;
            }

            auto & operator = (self const & value){
                if (value.mem){ 
                    value->owners_inc();
                }
                cast<self>(
                    atom_swap(& mem, value.mem)
                ).~meta();
                return this[0];
            }

            friend bool operator == (self const & value, decltype(nullptr)){
            return value.mem == nullptr;
            }

        protected:
            auto & operator [] (uxx index) const{
                return mem[0][index];
            }

            uxx length() const {
                return mem->this_length();
            }
        private:
            token_mix_t * mem;

            template<class root_t, class list> friend union mixc::gc_tuple::tuple;

            template<class guide> bool routing_entry(guide gui){
                using tuplep = tuple<attribute, typename attribute::member_list> *;
                attribute *         ptr = mem; // lis convert
                routing_result r;
                mem->set_visit();
                mem->set_can_arrive_root();

                if (r = tuplep(ptr)->routing(gui); r.can_arrive_root){
                    r.degree_dvalue += mem->owners();
                }

                xdebug(im_gc_meta_routing_entry, 
                    xtypeid(attribute).name, 
                    r.degree_dvalue
                );

                auto result = r.degree_dvalue <= 0 ? releaseable : not releaseable;

                if (result != releaseable){
                    mem->reset_can_arrive_root();
                }
                return result;
            }

            template<class guide> routing_result
            routing(guide gui){
                routing_result r;

                if (mem == nullptr){
                    return { 0 }; // no way
                }

                xdebug(im_gc_meta_routing, 
                    xtypeid(attribute).name,
                    mem->visited(),
                    mem->can_arrive_root(),
                    mem->owners(),
                    mem
                );

                if (not mem->visited()){
                    if (r = subrouting(gui); r.can_arrive_root){
                        r.degree_dvalue += mem->owners();
                    }
                    return r;
                }
                if (mem->can_arrive_root()){
                    return { 1 }; // has a way can arrive root
                }
                else{
                    mem->owners_dec(); // no way (maybe, but not absolutely)
                    xdebug(im_gc_meta_routing,
                        xtypeid(attribute).name, 
                        mem->owners()
                    );
                    return { 0 };
                }
            }

            template<class guide> routing_result subrouting(guide gui){
                using tuplep = tuple<attribute, typename attribute::member_list> *;
                attribute *     ptr = mem; // lis convert
                routing_result  r;
                mem->set_visit();

                if (r = tuplep(ptr)->routing(gui); r.can_arrive_root){
                    mem->set_can_arrive_root();
                }
                return r;
            }

            template<class guide> void clear_footmark(guide gui){
                using tuplep = tuple<attribute, typename attribute::member_list> *;
                xdebug(im_gc_meta_clear_footmark,
                    xtypeid(attribute).name, 
                    mem->visited(), 
                    mem->can_arrive_root(),
                    mem->owners(),
                    mem
                );
                if (mem->visited()){
                    mem->reset_visit();
                    attribute * ptr = mem; // lis convert
                    tuplep(ptr)->clear_footmark(gui, mem);
                    mem->reset_can_arrive_root();
                }
                else if (mem->can_arrive_root()){
                    mem->owners_inc();
                    xdebug(im_gc_meta_clear_footmark,
                        xtypeid(attribute).name, 
                        mem->owners()
                    );
                }
            }

            template<class guide> void clear_footmark(guide gui, voidp root){
                if (mem == nullptr){
                    return;
                }
                clear_footmark(gui);
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
                free_with_destroy(mem,
                    memory_size(
                        sizeof(token_mix_t) + mem->this_length() * sizeof(item)
                    )
                );
            }
        };
    }

#endif

namespace xuser::inc{
    template<class impl, class item>
    using ref_ptr = mixc::gc_ref::meta<impl, mixc::gc_ref::dummy_t, item, false>;

    template<class impl, class item, class attribute = mixc::gc_ref::dummy_t>
    using ref_array = mixc::gc_ref::meta<impl, item, attribute, true>;
}
