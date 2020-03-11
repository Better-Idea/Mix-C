#pragma once
#include"component/memory.hpp"
#include"definition/mini.hpp"
#include"instruction/base.hpp"

xspace(mixc) xspace(docker)
    xspace(array_private)
        #define xprefix   template<class item, uxx page_level>
        #define xargs     item, page_level

        xskeleton()
            uxx    length;
            item * page[page_level];

            xini(skeleton() : length(0)) $

            template<class ... items> xini(skeleton(items const & ... list) : skeleton())
                thei.push_back(list...);
            $

            ~xini(skeleton())
                item * current_page;
                uxx    index;
                uxx    count;
                uxx    page_index = instruction::last_index_of_set(thei.length);

                xif(page_index == not_exist)
                    return;
                $

                count           = thei.length;
                current_page    = thei.page[page_index + not thei.reach_the_page_junction];
                instruction::bit_test_and_reset(& count, page_index);

                xif(count) 
                    xfor(index = 0; index < count; index++) 
                        current_page[index].~item();
                    $
                    count = uxx(1) << page_index;
                    memory::free<item>(current_page, count);
                xes 
                    count = uxx(1) << page_index;
                $

                xwhile(count >>= 1) 
                    xfor(current_page = thei.page[page_index--], index = 0; index < count; index++) 
                        current_page[index].~item();
                    $
                    memory::free<item>(current_page, count);
                $
                thei.length = 0;
                thei.page[0]->~item();
                memory::free<item>(thei.page[0]);
            $

            xmark_test xoprc(item &, [], uxx index) $$
                uxx page_index = instruction::last_index_of_set(index);
                instruction::bit_test_and_reset(& index, page_index);
                return thei.page[page_index + 1][index];
            $
        $;

        xinterface()
        private:
            xprop(item **, page, xpubget) $;
        public:
            xprop(uxx &, length, xpubget, xpriset) $;

            xpropx(bool, reach_the_page_junction) 
                xmark_test xpubget $$
                    return instruction::count_of_set(thes.length) == 1;
                $
            $;

            xpropx(uxx, capacity)
                xmark_test xpubget $$
                    uxx index;
                    uxx bytes = 0;
                    index = instruction::last_index_of_set(thes.length);

                    xif(index == not_exist)
                        return 0;
                    $

                    instruction::bit_test_and_set(& bytes, index);
                    
                    xif(bytes == thes.length) // page[0] and page[1] just has one item
                        return bytes;
                    $
                    return bytes << 1; //page[i] has 2^(i-1) items
                $
            $;

            xpropx(bool, empty)
                xmark_test xpubget $$
                    return thes.length == 0;
                $
            $;
            
            xpropx(item &, first)
                xmark_test xpubget $$
                    return thes.page[0][0];
                $
                xmark_test xpubset $$
                    thes.page[0][0] = value;
                $
            $;
            
            xpropx(item &, last)
                xmark_test xpubget $$
                    return thes[thei.length - 1];
                $
                xmark_test xpubset $$
                    thes[thei.length - 1] = value;
                $
            $;

            xmark_test xdef(push_back, item const & value) $$
                item *        ptr;
                uxx           offset;
                uxx           index;
                offset      = 0;
                index       = instruction::last_index_of_set(length);

                xif(index == not_exist) 
                    ptr     = page[0] = memory::alloc<item>();
                xei(reach_the_page_junction == false) 
                    offset  = length;
                    instruction::bit_test_and_reset(& offset, index);
                    index  += 1;
                    ptr     = page[index];
                    ptr    += offset;
                xes
                    ptr     = page[index + 1] = memory::alloc<item>(uxx(1) << index);
                $
                
                length     += 1;
                xnew(item, ptr[0])(value);
            $

            #define xgen                \
                item const & a,         \
                item const & b,         \
                args const & ... list

            xmark_test template<class ... args> xdef(push_back, xgen) $$
                push_back(a);
                push_back(b, list...);
            $
            #undef xgen

            xmark_test xdef(pop_back) $$
                equivalence_bytes<item> mem;
                pop_back(& mem);
                return (item)mem;
            $

            xmark_test xdef(pop_back, item * value) $$
                uxx                   offset;
                uxx                   page_index;
                item *                current_page;
                length              = length - 1;
                offset              = length;
                page_index          = instruction::last_index_of_set(length);

                xif(page_index == not_exist) 
                    current_page    = page[0];
                    value[0]        = current_page[0];
                    current_page[0].~item();
                    memory::free<item>(current_page);
                xes
                    instruction::bit_test_and_reset(& offset, page_index);
                    current_page    = page[page_index + 1];
                    value[0]        = current_page[offset];
                    current_page[offset].~item();

                    xif(reach_the_page_junction)
                        memory::free<item>(current_page, uxx(1) << page_index);
                    $
                $
            $
        $;
    $

    template<class item, uxx rank = 1, uxx page_level = 32> struct array;

    template<class item, uxx rank, uxx page_level>
    xstruct(array : array<array<item, rank - 1, page_level>, 1, page_level>)
        template<class ... args>
        xini(
            array(args const & ... list) : 
            array<array<item, rank - 1, page_level>, 1, page_level>(list...))
        $
    $;

    template<class item, uxx page_level>
    xstruct(array<item, 1, page_level> : array_private::skeleton<item, page_level>)
        typedef array_private::skeleton<item, page_level> base;
        template<class ... args>
        xini(array(args const & ... list): base(list...)) $
    $;
$ $

#undef xprefix
#undef xargs
