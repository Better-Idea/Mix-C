#pragma once
#include"define/base_type.hpp"
#include"gc/private/self_management.hpp"
#include"memop/memory.hpp"
#include"memop/copy.hpp"
#include"meta_ctr/cif.hpp"
#include"meta_seq/tlist.hpp"
#include"utility/type/dummy_t.hpp"

namespace mixc {
    enum class ini {};

    constexpr ini ini_now = ini(0);

    struct length {
        explicit length(uxx value) : value(value) { }
        operator uxx () {
            return value;
        }
    private:
        uxx value;
    };
}

namespace mixc::inner_gc{
    template<class list> struct tuple;
    template<class impl, class item, class attribute = dummy_t, bool is_array = false> struct meta;

    struct empty_length {
        empty_length(uxx length) : owners(1) { }
    protected:
        constexpr auto this_length() { return uxx(0); }
        constexpr auto this_length(uxx value) { }
    private:
        template<class impl, class item, class attribute, bool is_array> friend struct meta;
        uxx owners;
    };

    struct plus_length : empty_length {
        plus_length(uxx length) : length(length), empty_length(0) { }
        const uxx length;
    protected:
        auto this_length() {
            return length;
        }
        auto this_length(uxx value) {
            return mixc::copy(& length, value);
        }
    };

    template<class type, class attribute = dummy_t, class addition = empty_length>
    struct header : addition, attribute {
        template<class ... args>
        header(uxx length, args const ... list) : 
            addition(length), attribute(list...) {}
    private:
        ~header() {
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
    };

    template<class impl, class item, class attribute, bool is_array>
    struct meta : self_management {
        using self = meta<impl, item, attribute, is_array>;
        using gc_candidate_list = mixc::tlist<
            item, attribute
        >;
        using header_t = header<
            item,
            attribute,
            typename mixc::cif<is_array, plus_length, empty_length>::result
        >;

        meta() : mem(nullptr) { }

        template<class ... args>
        meta(mixc::length length, args const & ... list) {
            new (
                mem = alloc(length)
            ) header_t(length, list...);

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
        template<class list> friend struct tuple;
        static constexpr uxx mask_in_visited    = uxx(1) << (sizeof(uxx) * 8 - 1);
        static constexpr uxx mask_in_gc_queue   = uxx(1) << (sizeof(uxx) * 8 - 2);
        static constexpr uxx step               = uxx(1);
        header_t * mem;

        template<class guide> uxx routing(guide);
        template<class guide> uxx routing(guide, voidp root);
        template<class guide> uxx subrouting(guide, voidp root);
        template<class guide> void clear_footmark(guide);
        bool visited();
        void inversion_visit_state();

        auto alloc(uxx length) {
            return mixc::alloc<header_t>(
                memory_size(
                    sizeof(header_t) + length * sizeof(item)
                )
            );
        }
    };
}



#include"define/base_type.hpp"
#include"lock/atom_add.hpp"
#include"lock/atom_fetch_or.hpp"
#include"lock/atom_or.hpp"
#include"lock/atom_sub.hpp"
#include"lock/atom_swap.hpp"
#include"lock/atom_xor.hpp"
#include"memop/cast.hpp"
#include"memop/memory.hpp"
#include"meta/is_same.hpp"
#include"meta_seq/tlist.hpp"
#include"gc/private/make_guide.hpp"
#include"gc/private/routing.hpp"
#include"gc/private/collect.hpp"
#include"gc/private/tuple.hpp"

namespace mixc::inner_gc{
    template<class impl, class item, class attribute, bool is_array>
    inline meta<impl, item, attribute, is_array>::meta(self const & value){
        if (mem = value.mem; mem != nullptr) {
            mixc::atom_add(& mem->owners, step);
        }
    }

    template<class impl, class item, class attribute, bool is_array>
    inline
    meta<impl, item, attribute, is_array> & 
    meta<impl, item, attribute, is_array>::operator = (self const & value){
        if (value.mem){ 
            mixc::atom_add(& value->owners, step);
        }
        auto ptr = mixc::atom_swap(& mem, value.mem);
        mixc::cast<self>(ptr).~meta();
        return this[0];
    }

    template<class impl, class item, class attribute, bool is_array>
    inline meta<impl, item, attribute, is_array>::~meta(){
        using guide = decltype(make_guide<impl>());
        constexpr bool      need_gc = not is_same<guide, tlist<>>;
        guide               gui;
        header_t *          tmp;
        uxx                 cnt;
        uxx                 byp = false; // bypass when not in gc-queue

        if (tmp = mixc::atom_swap(& mem, tmp); tmp != nullptr) { // enter only once
            cnt = mixc::atom_sub(& tmp->owners, step); // decease meta count

            if constexpr (need_gc){ 
                byp = mixc::atom_fetch_or(& tmp->owners, mask_in_gc_queue) & mask_in_gc_queue;
            }
            if (byp){
                return;
            }
            if (cnt == 0 || cnt <= mixc::cast<self>(tmp).routing(gui)) {
                tmp->~header();
                mixc::free(tmp, 
                    memory_size(
                        sizeof(header_t) + tmp->this_length() * sizeof(item)
                    )
                );
            }
            else{
                mixc::atom_xor(& tmp->owners, mask_in_gc_queue);
            }
        }
    }

    template<class impl, class item, class attribute, bool is_array>
    inline bool meta<impl, item, attribute, is_array>::visited(){
        return (mem->owners & mask_in_visited) != 0;
    }

    template<class impl, class item, class attribute, bool is_array>
    inline void meta<impl, item, attribute, is_array>::inversion_visit_state(){
        mixc::atom_xor(& mem->owners, mask_in_visited);
    }

    template<class impl, class item, class attribute, bool is_array>
    template<class guide>
    inline uxx meta<impl, item, attribute, is_array>::routing(guide gui){
        // inside reference count (irc)
        uxx irc = subrouting(gui, mem);
        clear_footmark(gui);
        return irc;
    }

    template<class impl, class item, class attribute, bool is_array>
    template<class guide>
    inline uxx meta<impl, item, attribute, is_array>::routing(guide gui, voidp root){
        if (root == voidp(mem)){
            return 1;
        }
        else if (mem == nullptr || visited()){
            return 0;
        }
        else{
            return subrouting(gui, root);
        }
    }

    template<class impl, class item, class attribute, bool is_array>
    template<class guide>
    inline uxx meta<impl, item, attribute, is_array>::subrouting(guide gui, voidp root){
        using tuplep = tuple<typename attribute::gc_candidate_list> *;
        inversion_visit_state();
        attribute * ptr = mem; // lis convert
        return tuplep(ptr)->routing(gui, root);
    }

    template<class impl, class item, class attribute, bool is_array>
    template<class guide>
    inline void meta<impl, item, attribute, is_array>::clear_footmark(guide gui){
        using tuplep = tuple<typename attribute::gc_candidate_list> *;
        if (mem != nullptr && visited()){
            using tuplep = tuple<typename attribute::gc_candidate_list> *;
            inversion_visit_state();
            attribute * ptr = mem; // lis convert
            tuplep(ptr)->clear_footmark(gui);
        }
    }
}

