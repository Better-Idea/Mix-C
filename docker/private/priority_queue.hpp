#ifndef xpack_docker_priority_queue
#define xpack_docker_priority_queue
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_priority_queue::inc
#include"algo/heap_root.hpp"
#include"docker/shared_array/stacklize.hpp"
#include"docker/shared_array.hpp"
#include"docker/transmitter.hpp"
#include"dumb/mirror.hpp"
#include"macro/xis_nullptr.hpp"
#include"memop/cast.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_priority_queue {
    template<class final, class item_t>
    xstruct(
        xtmpl(priority_queue, final, item_t),
        xprif(items, inc::shared_array<item_t>)
    )
        using mirror = inc::shared_array<inc::mirror<item_t>>;

        priority_queue(){}

        template<class finalx>
        priority_queue(priority_queue<finalx, item_t> const & object) : 
            items((inc::shared_array<item_t> &)object.items){}

        priority_queue(::length initial_capacity) : 
            items(initial_capacity){}

        void clear() {
            the_t{}.items.swap(xref items);
        }

        void push(item_t const & value) {
            // 本次 push 无需拷贝构造
            inc::cast<mirror>(items).push(inc::mirror<item_t>{});

            // 本次 push 默认内部拷贝构造
            inc::heap_root::push(items, items.length() - 1, value);
        }

        inc::transmitter<item_t> pop() {
            auto length = items.length();
            inc::transmitter<item_t> r = items[0];
            inc::heap_root::pop(items, length, items[length - 1]);
            inc::cast<mirror>(items).pop();
            return r;
        }

        void swap(the_t * object){
            items.swap(object);
        }

        final & operator= (decltype(nullptr)){
            items = nullptr;
            return thex;
        }

        final & operator= (the_t const & object){
            items = object.items;
            return thex;
        }

        xpubgetx(root, inc::transmitter<item_t>){
            return items[0];
        }

        xpubgetx(length, uxx){
            return items.length();
        }

        xpubgetx(is_empty, bool){
            return items.length() == 0;
        }

        xis_nullptr(
            items == nullptr
        )
    };
}
#endif

#define xusing_docker_name  ::mixc::docker_priority_queue
