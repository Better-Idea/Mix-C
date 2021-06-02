#ifndef xpack_ttyctrl_text_line
#define xpack_ttyctrl_text_line
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::ttyctrl_text_line::inc
#include"algo/insert.hpp"
#include"algo/remove.hpp"
#include"define/base_type.hpp"
#include"docker/shared_array.hpp"
#include"macro/xexport.hpp"
#include"macro/xindex_rollback.hpp"
#include"macro/xref.hpp"
#include"macro/xstruct.hpp"
#include"memop/copy.hpp"
#include"memop/swap.hpp"
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
        xprif(m_text          ,   inc::shared_array<char16_t>),
        xprif(m_left          ,   u16),
        xprif(m_top           ,   u16),
        xprif(m_column        ,   u16),
        xprif(m_cursor_column ,   u16),
        xprif(m_cursor_index  ,   u32),
        xprif(m_length        ,   u32)
    )
        using final_t = the_t;

        enum{ over_boundary = true, };

        xprigetx(capacity, uxx){
            return the.m_text.length();
        }
    public:
        text_line() : 
            m_text(::length{4}){
        }

        char16_t & operator[](ixx index){
            xindex_rollback(length(), index);
            return the.m_text[uxx(index)];
        }

        xpubgetx(length, uxx){ 
            return the.m_text.length();
        }

        xpubget_pubset(left)
        xpubget_pubset(top)
        xpubget_pubset(column)
        xpubget_pubset(cursor_index)
        xpubget_pubset(cursor_column)

        bool go_left(){
            if (the.m_cursor_column != the.m_left){
                auto v = the[m_cursor_index];
                the.m_cursor_column -= inc::tty_width(v);

                if (the.m_cursor_column + 1 == the.m_left){ // over bound
                    the.m_cursor_column += 1;
                }
            }
            if (the.m_cursor_index != 0){
                the.m_cursor_index -= 1;
                return not over_boundary;
            }
            return over_boundary;
        }

        bool go_right(){
            if (auto bound = m_left + m_column; m_cursor_column != bound){
                auto v = the[m_cursor_index];
                the.m_cursor_column += inc::tty_width(v);

                if (the.m_cursor_column - 1 == bound){ // over bound
                    the.m_cursor_column -= 1;
                }
            }
            if (the.m_cursor_index != the.length()){
                the.m_cursor_index += 1;
                return not over_boundary;
            }
            return over_boundary;
        }

        void go_home(){
            the.m_cursor_column = the.m_left;
            the.m_cursor_index  = 0;
        }

        void go_end(){
            the.m_cursor_column = the.m_left + the.m_column;
            the.m_cursor_index  = the.length();
        }

        bool backspace(){
            if (go_left() == over_boundary){
                return over_boundary;
            }
            return the.compact();
        }

        bool deletes(){
            if (the.m_cursor_index == the.length()){
                return over_boundary;
            }
            return the.compact();
        }

        void write(char16_t value){
            uxx new_length = inc::insert(the.m_text, the.m_cursor_index, value);

            if (the.capacity() == new_length){
                the.resize(new_length * 4);
            }
        }
    private:
        bool compact(){
            uxx new_length = inc::remove(the.m_text, the.m_cursor_index);
            if (the.capacity() == new_length * 4){
                the.resize(new_length);
            }
            return not over_boundary;
        }

        void resize(uxx new_length){
            inc::shared_array<char16_t> compact {
                ::length{new_length}
            };
            inc::copy(compact, the.m_text, compact.length());
            the.m_text.swap(xref(compact));
        }
    $
}
#endif

xexport(mixc::ttyctrl_text_line::text_line)