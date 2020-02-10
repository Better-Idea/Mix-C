#pragma once
#include"definition/mini.hpp"
#include"utility/bit_bind.hpp"

#define xprefix   template<uxx bits, class bit_type>
#define xargs     bits, bit_type

xspace(mixc) xspace(docker)
    xspace(bit_array_private)
        xskeleton()
            template<class ... args>
            xini(
                skeleton(args const & ... list) : 
                item { 0 })
                implicit_value<uxx> values[] = { list..., 0 }; // put a 0 for avoid empty array

                xfor(uxx i = 0; i < sizeof...(args); i++)
                    thex[values[i]] = set_state;
                $
            $

            xop([], uxx index)
                return utility::bit_bind<u08, bit_type>(item + (index >> 3), index & 0x7);
            $
        private:
            u08 item[bits / 8 + (bits % 8 != 0)];
        $;

        xinterface()
        private:
        public:
        $;
    $

    xprefix xstruct(bit_array : bit_array_private::skeleton<xargs>)
        template<class ... args>
        xini(
            bit_array(args const & ... list) : 
            bit_array_private::skeleton<xargs>(list...)
        ) $
    $;
$ $

#undef xprefix
#undef xargs

