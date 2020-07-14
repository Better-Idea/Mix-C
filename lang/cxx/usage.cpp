#ifndef xpack_lang_cxx_usage
#define xpack_lang_cxx_usage
    #define xuser mixc::lang_cxx_usage
    #include"io/tty.hpp"
    #include"lang/cxx/+.hpp"
    #include"lang/cxx.hpp"
    #include"macro/xhint.hpp"

    namespace xuser::inc{
        void example(){
            // lang/cxx/ 概念：
            // cxx 没有 null 这个概念，只有指向 "" 的空字符串，且默认为空字符串

            // 注意：
            // 在此使用可复用的缓冲区，并在例子中保证不会有两个 cxx 对象同时保存了信息在该缓冲区中
            auto alloc = [](uxx length){
                static char buf[1024];
                buf[length] = '\0';
                return buf;
            };

            {
                // cxx::align_center
                tty.write_line("cxx::align_center");
                c08 str0 = "align_center";
                tty.write_line("123456789a123456789a123456789a");

                // 居中对齐，并用空格填充空白处
                c08 str1 = str0.align_center(20, alloc);
                tty.write_line(str1);

                // 居中对齐，并用 '-' 填充空白处
                // 此时 str1 的值无效了
                c08 str2 = str0.align_center(20, '-', alloc);
                tty.write_line(str2);
                tty.write_line();
            }

            {
                // cxx::align_left
                tty.write_line("cxx::align_left");
                c08 str0 = "align_left";
                tty.write_line("123456789a123456789a123456789a");

                // 左对齐，并用空格填充空白处
                c08 str1 = str0.align_left(20, alloc);
                tty.write_line(str1);

                // 左对齐，并用 '+' 填充空白处
                // 此时 str1 的值无效了
                c08 str2 = str0.align_left(20, '+', alloc);
                tty.write_line(str2);
                tty.write_line();
            }

            {
                // cxx::align_right
                tty.write_line("cxx::align_right");
                c08 str0 = "align_right";
                tty.write_line("123456789a123456789a123456789a");

                // 左对齐，并用空格填充空白处
                c08 str1 = str0.align_right(20, alloc);
                tty.write_line(str1);

                // 左对齐，并用 '*' 填充空白处
                // 此时 str1 的值无效了
                c08 str2 = str0.align_right(20, '*', alloc);
                tty.write_line(str2);
                tty.write_line();
            }

            {
                // cxx::clone
                tty.write_line("cxx::clone");

                // 拷贝副本
                c08 str1 = "clone";
                c08 str2 = str1.clone(alloc);
                xhint(str1, str2, voidp(str1), voidp(str2));
                xhint(str1, str1.compare("hello"));
                tty.write_line();
            }

            {
                // cxx::compare
                tty.write_line("cxx::compare");

                // 按字典序比较
                // 遵循传统比较结果：0 表示相等，负数表示小于，正数表示大于
                c08 str1 = "";

                // 默认就是空字符串
                c08 str2;
                c08 str3 = "a";
                c08 str4 = "ab";
                c08 str5 = "aB";
                xhint(str1, str2, str3, str4, str5);
                xhint(str1.compare(str2));
                xhint(str3.compare(str4));
                xhint(str4.compare(str5));
                xhint(str4.compare(str5, ignore_case<char>));
                tty.write_line();
            }

            {
                // cxx::compare_fastly
                tty.write_line("cxx::compare_fastly");

                // 先按串长比较，再按按字典序比较
                // 遵循传统比较结果：0 表示相等，负数表示小于，正数表示大于
                c08 str1 = "";

                // 默认就是空字符串
                c08 str2;
                c08 str3 = "a";
                c08 str4 = "ab";
                c08 str5 = "aB";
                c08 str6 = "aaa";

                xhint(str1, str2, str3, str4, str5, str6);
                xhint(str1.compare_fastly(str2));
                xhint(str3.compare_fastly(str4));
                xhint(str4.compare_fastly(str5));
                xhint(str4.compare_fastly(str5, ignore_case<char>));
                xhint(str4.compare_fastly(str6));
                tty.write_line();
            }

            {
                // cxx::index_of_first
                tty.write_line("cxx::index_of_first");
                tty.write_line("0123456789a123456789a123456789a");
                c08 str1 = "i love c++, do you love it";
                tty.write_line(str1);
                xhint(str1.index_of_first("love"));
                xhint(str1.index_of_first("LOVE", ignore_case<char>));
                xhint(str1.index_of_first('c'));
                xhint(str1.index_of_first('C', ignore_case<char>));
                xhint(str1.index_of_first("java"), not_exist);

                // 正序遍历所有以 "love" 开头的索引
                str1.index_of_first("love", [](uxx match_index){
                    xhint(match_index);
                });
                tty.write_line();
            }

            {
                // cxx::index_of_first_miss
                tty.write_line("cxx::index_of_first_miss");
                tty.write_line("0123456789a123456789a123456789a");
                c08 str1 = "    ....i love c++";
                tty.write_line(str1);
                xhint(str1.index_of_first_miss(' '));
                xhint(str1.index_of_first_miss({ ' ', '.' }));
                tty.write_line();
            }

            {
                // cxx::index_of_last
                tty.write_line("cxx::index_of_last");
                tty.write_line("0123456789a123456789a123456789a");
                c08 str1 = "1st c++, 2nd c++, 3rd C++";
                tty.write_line(str1);
                xhint(str1.index_of_last("c++"));
                xhint(str1.index_of_last("c++", ignore_case<char>));
                xhint(str1.index_of_last("C++"));

                // 反序遍历所有以 "love" 开头的索引
                str1.index_of_last("c++", [](uxx match_index){
                    xhint(match_index);
                }, ignore_case<char>);

                tty.write_line();
            }

            {
                // cxx::index_of_last_miss
                tty.write_line("cxx::index_of_last_miss");
                tty.write_line("0123456789a123456789a123456789a");
                c08 str1 = "hello?balabla...balabala...";
                tty.write_line(str1);
                xhint(str1.index_of_last_miss({ 'a', 'b', 'l', '.' }));
                tty.write_line();
            }

            {
                // cxx::insert
                tty.write_line("cxx::insert");
                tty.write_line("0123456789a123456789a123456789a");
                c08 str1 = "a^2 = c^2";
                tty.write_line(str1);

                // 从 3 号索引插入 " + b^2"
                xhint(str1.insert(3, " + b^2", alloc));
                
                c08 str2 = "";
                xhint(str2);

                // 将 "anything" 插入到末尾
                xhint(str2.insert(-1, "anything", alloc));

                c08 str3 = "love you";
                xhint(str3);
                xhint(str3.insert(-1, " 3000 times", alloc));
                tty.write_line();
            }

            {
                // cxx::is_contains
                tty.write_line("cxx::is_contains");
                c08 str1 = "cat is a kind of Liquid";
                tty.write_line(str1);
                xhint(str1.is_contains("cat"));
                xhint(str1.is_contains("CAT"));
                xhint(str1.is_contains("CAT", ignore_case<char>));
                xhint(str1.is_contains(' '));
                xhint(str1.is_contains("kind"));
                xhint(str1.is_contains("of"));
                xhint(str1.is_contains("liquid"));
                xhint(str1.is_contains("liquid", ignore_case<char>));
                tty.write_line();
            }

            {
                // cxx::is_ends_with
                tty.write_line("cxx::is_ends_with");
                c08 str1 = "doesn't kill you will makes you stronger";
                xhint(str1);

                // 任何串都不以空串结尾
                xhint(str1.is_ends_with(""));

                // 除空串外，任何串可以都自己结尾
                xhint(str1.is_ends_with(str1));
                xhint(str1.is_ends_with("stronger"));
                xhint(str1.is_ends_with("Stronger", ignore_case<char>));
                tty.write_line();
            }

            {
                // cxx::is_starts_with
                tty.write_line("cxx::is_starts_with");
                c08 str1 = "PowerfulCat is the most strongest cat";
                xhint(str1);

                // 任何串都不以空串开头
                xhint(str1.is_starts_with(""));

                // 除空串外，任何串可以都自己开始
                xhint(str1.is_starts_with(str1));
                xhint(str1.is_starts_with("Powerful"));
                xhint(str1.is_starts_with("POWERFULCAT", ignore_case<char>));
                tty.write_line();
            }

            {
                // cxx::match 专题
                // to be continue
            }

            {
                // cxx::pad_bends
                tty.write_line("cxx::pad_bends");
                tty.write_line("0123456789a123456789a123456789a");
                c08 str1 = "[title]";
                tty.write_line(str1);
                tty.write_line(str1.pad_bends(10/*left*/, 10/*right*/, '=', alloc));
                tty.write_line();
            }

            {
                // cxx::pad_left
                tty.write_line("cxx::pad_left");
                tty.write_line("0123456789a123456789a123456789a");
                c08 str1 = ":addition";
                tty.write_line(str1);
                tty.write_line(str1.pad_left(10/*left*/, '+', alloc));
                tty.write_line();
            }

            {
                // cxx::pad_right
                tty.write_line("cxx::pad_right");
                tty.write_line("0123456789a123456789a123456789a");
                c08 str1 = "deletions:";
                tty.write_line(str1);
                tty.write_line(str1.pad_right(10/*right*/, '-', alloc));
                tty.write_line();
            }

            {
                // cxx::parse
                tty.write_line("cxx::parse");
                c08 str1 = "inf";
                c08 str2 = "+inf";
                c08 str3 = "-inf";
                c08 str4 = "1";
                c08 str5 = "1.234";
                c08 str6 = "1.234e2";
                c08 str7 = "1.234e-2";
                c08 str8 = "-1.234e-2";
                c08 str9 = "+12.34e+2";
                c08 stra = "18446744073709551615";
                c08 strb = "-9223372036854775808";
                xhint(str1, str1.parse<f32>());
                xhint(str2, str2.parse<f32>());
                xhint(str3, str3.parse<f32>());
                xhint(str4, str4.parse<f32>());
                xhint(str5, str5.parse<f32>());
                xhint(str6, str6.parse<f32>());
                xhint(str7, str7.parse<f32>());
                xhint(str8, str8.parse<f32>());
                xhint(str9, str9.parse<f32>());
                xhint(stra, stra.parse<u64>());
                xhint(strb, strb.parse<i64>());
                tty.write_line();
                // TODO: 部分转换成功的情况
            }

            {
                // cxx::parse_json
                // TODO: 专题
            }

            {
                // cxx::ph
                // TODO: 专题
            }

            {
                // cxx::remove
                tty.write_line("cxx::remove");
                tty.write_line("0123456789a123456789a123456789a");
                c08 str1     = "emmm...1234";
                tty.write_line(str1);
                tty.write_line(str1.remove(cc{7/*left close interval*/, 7/*right close interval*/}, alloc));

                // -1 指示最后一个字符之后一个位置的索引(值等于串长)
                tty.write_line(str1.remove(co{4/*left close interval*/, -1/*right open interval*/}, alloc));
                tty.write_line();
            }

            {
                // cxx::replace
                tty.write_line("cxx::replace");
                c08 str1 = "ABABBABa";
                c08 str2 = "";

                xhint(str1, str1.replace("A", "C", alloc));
                xhint(str1, str1.replace("A", "C", ignore_case<char>, alloc));
                xhint(str1, str1.replace("A", "", ignore_case<char>, alloc));

                // 无效果
                xhint(str2, str2.replace("", "B", alloc));
                tty.write_line();

                // 误用，由于 alloc 始终时分配缓冲区首地址，所以在传参的时候
                // 第二个 replace 中的 alloc 分配的内存会覆盖第一个 replace 中的值
                // xhint(str1, str1.replace("A", "C", alloc), str1.replace("A", "C", ignore_case<char>, alloc));
            }

            {
                // cxx::reverse
                tty.write_line("cxx::reverse");
                c08 str1 = "123456";
                xhint(str1, str1.reverse(alloc));
                tty.write_line();
            }

            {
                // cxx::slice
                tty.write_line("cxx::slice");
                c08 str1 = "123456789";
                xhint(str1);

                // 当 slice 切片区间不是反序时可以不分配内存，直接与 str1 共享内存
                xhint(str1.slice(co{1/*left close interval*/, -2/*right open interval*/}));
                xhint(str1.slice(oc{-2, 0}, alloc));
                tty.write_line();
            }

            {
                // cxx::strcat
                tty.write_line("cxx::strcat");
                c08 str1 = "the blind cat ";
                c08 str2 = "chinglish is ";
                xhint(str1.strcat("catchs the mouse", alloc));
                xhint(str2.strcat({ "wonderful, ", "fantastic and ", "interesting" }, alloc));
                tty.write_line();
            }

            {
                // cxx::strlize
                tty.write_line("cxx::strlize");

                // 默认不保留前导零
                // 默认为 10 进制
                xhint(c08(1024      , alloc));
                xhint(c08(-1024     , alloc));
                xhint(c08(0x0a      , numeration_t::bin, alloc));
                xhint(c08(0b00100101, numeration_t::bin, alloc));
                xhint(c08(066       , numeration_t::oct, alloc));
                xhint(c08(0xfc      , numeration_t::hex, alloc));
                xhint(c08(3.1415926 , alloc));

                // 保留 8 位有效数字
                xhint(c08(3.1415926 , 8, alloc));

                // 一般计数法
                xhint(c08(3141.5926 , float_format_t::fmt_n , alloc));

                // 始终显示正负号
                xhint(c08(3141.5926 , float_format_t::fmt_sn, alloc));
                xhint(c08(-3141.5926, float_format_t::fmt_sn, alloc));

                // 科学计数法
                // 指数部分使用小写 e，一般形式
                xhint(c08(3141.5926, float_format_t::fmt_1p2e3  , alloc));

                // 指数部分使用小写 e，始终显示实数部分正负号
                xhint(c08(3141.5926, float_format_t::fmt_s1p2e3 , alloc));

                // 指数部分使用小写 e，始终显示指数部分正负号
                xhint(c08(3141.5926, float_format_t::fmt_1p2es3 , alloc));

                // 指数部分使用小写 e，始终显示实数与指数部分正负号
                xhint(c08(3141.5926, float_format_t::fmt_s1p2es3, alloc));

                // 指数部分使用大写 e，一般形式
                xhint(c08(3141.5926, float_format_t::fmt_1p2E3  , alloc));

                // 指数部分使用大写 e，始终显示实数部分正负号
                xhint(c08(3141.5926, float_format_t::fmt_s1p2E3 , alloc));

                // 指数部分使用大写 e，始终显示指数部分正负号
                xhint(c08(3141.5926, float_format_t::fmt_1p2Es3 , alloc));

                // 指数部分使用大写 e，始终显示实数与指数部分正负号
                xhint(c08(3141.5926, float_format_t::fmt_s1p2Es3, alloc));
                tty.write_line();
            }

            {
                // cxx::trim_bends
                // 清空首位指定字符
                tty.write_line("cxx::trim_bends");
                c08 str1 = "    value   ";
                c08 str2 = "1234value1234";

                // 返回缩减范围的 c08 浅副本，不分配内存
                xhint(str1, str1.trim_bends(' '));

                // 分配内存返回结果
                xhint(str1, str1.trim_bends(' ', alloc));

                // 在首位两端清空列表中的元素
                xhint(str2, str2.trim_bends({ '1', '2', '3', '4' }));
                xhint(str2, str2.trim_bends({ '1', '2', '3', '4' }, alloc));
                tty.write_line();
            }

            {
                // cxx::trim_end
                tty.write_line("cxx::trim_end");
                c08 str1 = "   value   ";
                c08 str2 = "1234value1234";
                xhint(str1, str1.trim_end(' '));
                xhint(str1, str1.trim_end(' ', alloc));
                xhint(str2, str2.trim_end({ '1', '2', '3', '4' }));
                xhint(str2, str2.trim_end({ '1', '2', '3', '4' }, alloc));
                tty.write_line();
            }

            {
                // cxx::trim_start
                tty.write_line("cxx::trim_start");
                c08 str1 = "   value   ";
                c08 str2 = "1234value1234";
                xhint(str1, str1.trim_start(' '));
                xhint(str1, str1.trim_start(' ', alloc));
                xhint(str2, str2.trim_start({ '1', '2', '3', '4' }));
                xhint(str2, str2.trim_start({ '1', '2', '3', '4' }, alloc));
                tty.write_line();
            }
        }
    }

    int main(){
        xuser::inc::example();
        return 0;
    }
#endif
