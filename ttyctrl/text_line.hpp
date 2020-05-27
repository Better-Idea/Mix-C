#ifndef xpack_ttyctrl_text_line
#define xpack_ttyctrl_text_line
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::ttyctrl_text_line
        #include"algo/insert.hpp"
        #include"algo/remove.hpp"
        #include"define/base_type.hpp"
        #include"docker/darray.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"macro/xprop.hpp"
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

        struct text_line{
            xgc_fields(
                xiam(text_line),
                xpri(ptext          ,   inc::darray<char16_t>),
                xpri(pleft          ,   u16),
                xpri(ptop           ,   u16),
                xpri(pcolumn        ,   u16),
                xpri(pcursor_column ,   u16),
                xpri(pcursor_index  ,   u32),
                xpri(plength        ,   u32),
            );

            using final = the_t;

            enum{ over_boundary = true, };

            xpriget(capacity, uxx){
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

            xpubget(length, uxx){ 
                return the.ptext.length();
            }

            xpubget_pubset(left, uxx){
                xr { return the.pleft; }
                xw { the.pleft = value; }
            };

            xpubget_pubset(top, uxx){
                xr { return the.ptop; }
                xw { the.ptop = value; }
            };

            xpubget_pubset(column, uxx){
                xr { return the.pcolumn; }
                xw { the.pcolumn = value; }
            };

            xpubget_pubset(cursor_index, uxx){
                xr { return the.pcursor_index; }
                xw { the.pcursor_index = value; }
            };

            xpubget_pubset(cursor_column, uxx){
                xr { return the.pcursor_column; }
                xw { the.pcursor_column = value; }
            };

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
                inc::darray<char16_t> compact {
                    ::length{new_length}
                };
                inc::copy(compact, the.ptext, compact.length());
                inc::swap(xref the.ptext, xref compact);
            }
        };
    }
#endif

namespace xuser::inc{
    using ::mixc::ttyctrl_text_line::text_line;
}
