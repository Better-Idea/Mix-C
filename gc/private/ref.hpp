#ifndef xpack_gc_ref
#define xpack_gc_ref
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::gc_ref
        #include"define/base_type.hpp"
        #include"gc/private/make_guide.hpp"
        #include"gc/private/routing_result.hpp"
        #include"gc/private/self_management.hpp"
        #include"gc/private/tuple.hpp"
        #include"lock/atom_add.hpp"
        #include"lock/atom_and.hpp"
        #include"lock/atom_or.hpp"
        #include"lock/atom_sub.hpp"
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

        constexpr uxx  mark_visited         = uxx(1) << (sizeof(uxx) * 8 - 1);
        constexpr uxx  mark_can_arrive_root = uxx(1) << (sizeof(uxx) * 8 - 2);
        constexpr uxx  mark_in_free         = uxx(1) << (sizeof(uxx) * 8 - 3);
        constexpr uxx  mask_owners          = uxx(-1) >> 3;
        constexpr uxx  step                 = uxx(1);
        constexpr bool can_release          = true;

        template<class impl, class item, class attribute = dummy_t, bool is_array = false> struct meta;

        struct empty_length {
            empty_length(uxx length) : record(1) { }
        protected:
            constexpr auto this_length() { return uxx(0); }
            constexpr auto this_length(uxx value) { }
        private:
            template<class impl, class item, class attribute, bool is_array> friend struct meta;
            template<class root_t, class list> friend union mixc::gc_tuple::tuple;
            uxx record;
            uxx owners(){
                return record & mask_owners;
            }
            uxx owners_inc();
            uxx owners_dec();
            bool visited(){
                return (record & mark_visited) != 0;
            }
            bool can_arrive_root(){
                return (record & mark_can_arrive_root) != 0;
            }
            bool in_free(){
                return (record & mark_in_free) != 0;
            }
            void set_visit();
            void set_can_arrive_root();
            void set_in_free();
            void reset_can_arrive_root();
            void reset_visit();
            void reset_in_free();
        };

        struct plus_length : empty_length {
            plus_length(uxx length) : length(length), empty_length(0) { }
            const uxx length;
        protected:
            auto this_length() {
                return length;
            }
            auto this_length(uxx value) {
                return copy(& length, value);
            }
        };

        template<class type, class attribute = dummy_t, class addition = empty_length>
        struct header : addition, attribute {
            template<class ... args>
            header(uxx length, args const ... list) : 
                addition(length), attribute(list...) {}
        private:
            ~header() {
                xdebug(im_inner_gc_$header, "%s free\n", xtypeid(attribute).name());

                for (uxx i = 0; i < addition::this_length(); i++) {
                    (*this)[i].~type();
                }
            }
            auto * ptr(uxx index){
                return ((type *)(this + 1)) + index;
            }
            auto & operator [] (uxx index) {
                return ptr(index)[0];
            }
            template<class impl, class a, class b, bool is_array> friend struct meta;
            template<class t> friend void mixc::memory_alloctor::free_with_destroy(t *, memory_size);
        };


        template<class impl, class item, class attribute, bool is_array>
        struct meta : self_management {
            using self = meta<impl, item, attribute, is_array>;
            using the_length = typename cif<is_array, plus_length, empty_length>::result;
            using header_t = header<item, attribute, the_length>;
            // using header_mirror_t = header_mirror<item, attribute, the_length>;

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

            meta(self const & value);

            ~meta();

            self & operator = (self const & value);
            
            operator item & () const{
                return mem[0];
            }

            auto operator -> () const{
                return mem;
            }

            auto & operator [] (uxx index) const{
                return mem[0][index];
            }
        private:
            header_t * mem;

            template<class root_t, class list> friend union mixc::gc_tuple::tuple;
            template<class guide> void clear_footmark(guide);
            template<class guide> void clear_footmark(guide, voidp root);
            template<class guide> bool routing_entry(guide);
            template<class guide> routing_result routing(guide);
            template<class guide> routing_result subrouting(guide);

            template<class ... args>
            auto alloc(uxx length, args const & ... list) {
                return alloc_with_initial<header_t>(
                    memory_size(
                        sizeof(header_t) + length * sizeof(item)
                    ),
                    length, list...
                );
            }

            void free(){
                free_with_destroy(mem,
                    memory_size(
                        sizeof(header_t) + mem->this_length() * sizeof(item)
                    )
                );
            }
        };

        #define xtemp       template<class impl, class item, class attribute, bool is_array>
        #define xself       meta<impl, item, attribute, is_array>

        inline uxx empty_length::owners_inc(){
            return atom_add(& record, step) & mask_owners;
        }

        inline uxx empty_length::owners_dec(){
            return atom_sub(& record, step) & mask_owners;
        }

        inline void empty_length::set_visit(){
            atom_or(& record, mark_visited);
        }

        inline void empty_length::reset_visit(){
            atom_and(& record, ~mark_visited);
        }

        inline void empty_length::set_can_arrive_root(){
            atom_or(& record, mark_can_arrive_root);
        }

        inline void empty_length::reset_can_arrive_root(){
            atom_and(& record, ~mark_can_arrive_root);
        }

        inline void empty_length::set_in_free(){
            atom_or(& record, mark_in_free);
        }

        inline void empty_length::reset_in_free(){
            atom_and(& record, ~mark_in_free);
        }

        xtemp inline xself::meta(self const & value){
            if (mem = value.mem; mem != nullptr) {
                mem->owners_inc();
            }
        }

        xtemp inline xself & xself::operator = (self const & value){
            if (value.mem){ 
                value->owners_inc();
            }
            cast<self>(
                atom_swap(& mem, value.mem)
            ).~meta();
            return this[0];
        }

        xtemp inline xself::~meta(){
            using guide = decltype(make_guide<impl>());
            constexpr bool      need_gc = not is_same<guide, tlist<>>;
            header_t *          tmp = nullptr;
            guide               gui;
            uxx                 cnt;
            
            if (tmp = atom_swap(& mem, tmp); tmp == nullptr) { // enter only once
                return;
            }
            else{
                cnt = tmp->owners_dec();
            }

            xdebug(im_inner_gc_$meta, "%s | routing ref:%llu root:%p\n", xtypeid(attribute).name(), cnt, tmp);
            auto & old = cast<self>(tmp);

            if constexpr (not need_gc){
                if (cnt == 0){
                    old.free();
                }
            }
            else if (old->in_free()){
                return;
            }
            else if (old.routing_entry(gui) == can_release){
                old.clear_footmark(gui);
                old->set_in_free();
                old.free();
            }
            else{
                old.clear_footmark(gui);
            }
        }

        xtemp template<class guide> inline bool
        xself::routing_entry(guide gui){
            using tuplep = tuple<attribute, typename attribute::member_list> *;
            attribute *         ptr = mem; // lis convert
            routing_result r;
            mem->set_visit();
            mem->set_can_arrive_root();

            if (r = tuplep(ptr)->routing(gui); r.can_arrive_root){
                r.degree_dvalue += mem->owners();
            }

            xdebug(im_inner_gc_meta_routing_entry, "%s | routing io:%lld\n", 
                xtypeid(attribute).name(), 
                r.degree_dvalue
            );

            auto result = r.degree_dvalue <= 0 ? can_release : not can_release;

            if (result != can_release){
                mem->reset_can_arrive_root();
            }
            return result;
        }

        xtemp template<class guide> inline routing_result
        xself::routing(guide gui){
            routing_result r;

            if (mem == nullptr){
                return { 0 }; // no way
            }

            xdebug(im_inner_gc_meta_routing, "%s | routing visited:%llu can:%llu ref:%llu mem:%p\n", 
                xtypeid(attribute).name(), 
                uxx(mem->visited()), 
                uxx(mem->can_arrive_root()), 
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
                xdebug(im_inner_gc_meta_routing, "%s | routing ref:%llu\n", 
                    xtypeid(attribute).name(), 
                    mem->owners()
                );
                return { 0 };
            }
        }

        xtemp template<class guide> inline routing_result
        xself::subrouting(guide gui){
            using tuplep = tuple<attribute, typename attribute::member_list> *;
            attribute *     ptr = mem; // lis convert
            routing_result  r;
            mem->set_visit();

            if (r = tuplep(ptr)->routing(gui); r.can_arrive_root){
                mem->set_can_arrive_root();
            }
            return r;
        }

        xtemp template<class guide>
        inline void xself::clear_footmark(guide gui){
            using tuplep = tuple<attribute, typename attribute::member_list> *;
            xdebug(im_inner_gc_meta_clear_footmark, "%s | clear   visited:%llu can:%llu ref:%llu mem:%p\n", 
                xtypeid(attribute).name(), 
                uxx(mem->visited()), 
                uxx(mem->can_arrive_root()), 
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
                xdebug(im_inner_gc_meta_clear_footmark, "%s | clear ref:%llu\n", 
                    xtypeid(attribute).name(), 
                    mem->owners()
                );
            }
        }

        xtemp template<class guide>
        inline void xself::clear_footmark(guide gui, voidp root){
            if (mem == nullptr){
                return;
            }
            clear_footmark(gui);
        }

        #undef xtemp
        #undef xself
    }

#endif

namespace xuser::inc{
    template<class impl, class item>
    using ref_ptr = mixc::gc_ref::meta<impl, mixc::gc_ref::dummy_t, item, false>;

    template<class impl, class item, class attribute = mixc::gc_ref::dummy_t>
    using ref_array = mixc::gc_ref::meta<impl, item, attribute, true>;
}

