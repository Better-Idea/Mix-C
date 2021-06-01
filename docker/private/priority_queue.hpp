#ifndef xpack_docker_priority_queue
#define xpack_docker_priority_queue
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_priority_queue::inc
#include"algo/heap_root.hpp"
#include"define/base_type.hpp"
#include"docker/shared_array.hpp"
#include"docker/transmitter.hpp"
#include"dumb/mirror.hpp"
#include"macro/xexport.hpp"
#include"macro/xis_nullptr.hpp"
#include"macro/xstruct.hpp"
#include"memop/cast.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_priority_queue {
    template<class final_t, class item_t>
    xstruct(
        xtmpl(priority_queue, final_t, item_t),
        xprif(m_items, inc::shared_array<item_t>)
    )
        using mirror = inc::shared_array<inc::mirror<item_t>>;

        priority_queue(){}

        template<class finalx_t >
        priority_queue(priority_queue<finalx_t, item_t> const & object) : 
            m_items((inc::shared_array<item_t> &)object.m_items){}

        priority_queue(::length initial_capacity) : 
            m_items(initial_capacity){}

        void clear() {
            the_t{}.m_items.swap(xref m_items);
        }

        void push(item_t const & value) {
            // 本次 push 无需拷贝构造
            inc::cast<mirror>(m_items).push(inc::mirror<item_t>{});

            // 本次 push 默认内部拷贝构造
            inc::heap_root::push(m_items, m_items.length() - 1, value);
        }

        inc::transmitter<item_t> pop() {
            auto length = m_items.length();
            inc::transmitter<item_t> r = m_items[0];
            inc::heap_root::pop(m_items, length, m_items[length - 1]);
            inc::cast<mirror>(m_items).pop();
            return r;
        }

        void swap(the_t * object){
            m_items.swap(object);
        }

        final_t & operator= (decltype(nullptr)){
            m_items = nullptr;
            return thex;
        }

        final_t & operator= (the_t const & object){
            m_items = object.m_items;
            return thex;
        }

        xpubgetx(root, inc::transmitter<item_t>){
            return m_items[0];
        }

        xpubgetx(length, uxx){
            return m_items.length();
        }

        xpubgetx(is_empty, bool){
            return m_items.length() == 0;
        }

        xis_nullptr(
            m_items == nullptr
        )
    };
}
#endif

#define xusing_docker_name  ::mixc::docker_priority_queue
