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

                // 从 3 好索引插入 " + b^2"
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
        }
    }

    int main(){
        xuser::inc::example();
        return 0;
    }
#endif
