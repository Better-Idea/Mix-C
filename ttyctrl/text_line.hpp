#ifndef xpack_ttyctrl_text_line
#define xpack_ttyctrl_text_line
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::ttyctrl_text_line::inc
#include"algo/insert.hpp"
#include"algo/remove.hpp"
#include"docker/shared_array.hpp"
#include"macro/xindex_rollback.hpp"
#include"memop/copy.hpp"
#include"memop/swap.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::ttyctrl_text_line{
    // temp
    namespace inc{
        inline uxx tty_width(char16_t value){
            return value < 0xff ? 1 : 2;
        }
    }

    xstruct(
        xname(text_line),
        xprif(ptext          ,   inc::shared_array<char16_t>),
        xprif(pleft          ,   u16),
        xprif(ptop           ,   u16),
        xprif(pcolumn        ,   u16),
        xprif(pcursor_column ,   u16),
        xprif(pcursor_index  ,   u32),
        xprif(plength        ,   u32)
    )
        using final = the_t;

        enum{ over_boundary = true, };

        xprigetx(capacity, uxx){
            return the.ptext.length();
        }
    public:
        text_line() : 
            ptext(::length{4}){
        }

        char16_t & operator[](ixx index){
            xindex_rollback(length(), index);
            return the.ptext[uxx(index)];
        }

        xpubgetx(length, uxx){ 
            return the.ptext.length();
        }

        xpubget_pubset(left)
        xpubget_pubset(top)
        xpubget_pubset(column)
        xpubget_pubset(cursor_index)
        xpubget_pubset(cursor_column)

        bool go_left(){
            if (the.pcursor_column != the.pleft){
                auto v = the[pcursor_index];
                the.pcursor_column -= inc::tty_width(v);

                if (the.pcursor_column + 1 == the.pleft){ // over bound
                    the.pcursor_column += 1;
                }
            }
            if (the.pcursor_index != 0){
                the.pcursor_index -= 1;
                return not over_boundary;
            }
            return over_boundary;
        }

        bool go_right(){
            if (auto bound = pleft + pcolumn; pcursor_column != bound){
                auto v = the[pcursor_index];
                the.pcursor_column += inc::tty_width(v);

                if (the.pcursor_column - 1 == bound){ // over bound
                    the.pcursor_column -= 1;
                }
            }
            if (the.pcursor_index != the.length()){
                the.pcursor_index += 1;
                return not over_boundary;
            }
            return over_boundary;
        }

        void go_home(){
            the.pcursor_column = the.pleft;
            the.pcursor_index  = 0;
        }

        void go_end(){
            the.pcursor_column = the.pleft + the.pcolumn;
            the.pcursor_index  = the.length();
        }

        bool backspace(){
            if (go_left() == over_boundary){
                return over_boundary;
            }
            return the.compact();
        }

        bool deletes(){
            if (the.pcursor_index == the.length()){
                return over_boundary;
            }
            return the.compact();
        }

        void write(char16_t value){
            uxx new_length = inc::insert<char16_t>(the.ptext, the.pcursor_index, value);

            if (the.capacity() == new_length){
                the.resize(new_length * 4);
            }
        }
    private:
        bool compact(){
            uxx new_length = inc::remove<char16_t>(the.ptext, the.pcursor_index);
            if (the.capacity() == new_length * 4){
                the.resize(new_length);
            }
            return not over_boundary;
        }

        void resize(uxx new_length){
            inc::shared_array<char16_t> compact {
                ::length{new_length}
            };
            inc::copy(compact, the.ptext, compact.length());
            the.ptext.swap(xref compact);
        }
    $
}
#endif

xexport(mixc::ttyctrl_text_line::text_line)