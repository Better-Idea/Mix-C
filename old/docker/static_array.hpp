#pragma once
#include"definition/mini.hpp"

#define xprefix   template<class item_type, uxx count_of_items>
#define xargs     item_type, count_of_items

xspace(mixc) xspace(docker)
    xspace(static_array_private)
        xskeleton()
            template<class ... args> 
            xini(
                skeleton(args const & ... list) : items { list... }
            ) $

            xopr(auto &, [], uxx index)
                return items[index];
            $
        private:
            item_type   items[count_of_items];
        $;

        xinterface()
        public:
            xpropx(uxx, length)
                xmark_test xpubget $$
                    return count_of_items;
                $
            $;            
        $;
    $

    template<class item_type, uxx first_rank, uxx ... rest_rank>
    struct static_array;

    template<class item_type, uxx first_rank> 
    xstruct(
        static_array<item_type, first_rank> : 
        static_array_private::skeleton<xargs>)

        template<class ... args>
        xini(
            static_array(args const & ... list) : 
            static_array_private::skeleton<xargs>(list...)
        ) $
    $;

    
    template<class item_type, uxx first_rank, uxx ... rest_rank> 
    xstruct(
        static_array : 
        static_array<static_array<item_type, rest_rank...>, first_rank>)

        template<class ... args>
        xini(
            static_array(args const & ... list) : 
            static_array_private::skeleton<xargs>(list...)
        ) $
    $;

    template<class type, class ...args>
    xdef(v, type const & first_item, args const & ... list)
        return static_array<type, 1 + sizeof...(args)>(first_item, list...);
    $
$ $

#undef xprefix
#undef xargs


