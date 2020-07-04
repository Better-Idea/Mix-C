#ifndef xpack_lang_cxx_usage
#define xpack_lang_cxx_usage
    #define xuser mixc::lang_cxx_usage
    #include"lang/cxx/+.hpp"
    #include"lang/cxx.hpp"
    #include"io/tty.hpp"

    namespace xuser::inc{
        void example(){
            // lang/cxx/ 概念：
            // cxx 没有 null 这个概念，指向 "" 的空字符串

            // 重点：
            // 这里为使用可复用的缓冲区，并在例子中保证不会有两个 cxx 对象同时保存了信息在该缓冲区中
            auto alloc = [](uxx length){
                static char buf[1024];
                buf[length] = '\0';
                return buf;
            };

            // cxx::align_center
            {
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

            // cxx::align_left
            {
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

            // cxx::align_right
            {
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

            // cxx::clone
            {
                // 拷贝副本
                c08 str1 = "clone";
                c08 str2 = str1.clone(alloc);
                tty.write_line("str1.ptr:", voidp(str1), " value:", str1);
                tty.write_line("str2.ptr:", voidp(str2), " value:", str2);
                tty.write_line();
            }

            // cxx::compare
            {
                // 按字典序比较
                // 比较结果：0 表示相等，负数表示小于，正数表示大于
                c08 str1 = "";

                // 默认就是空字符串
                c08 str2;
                c08 str3 = "a";
                c08 str4 = "ab";
                c08 str5 = "aB";
                tty.write_line("str1:\"", str1, "\" str2:\"", str2, "\"");
                tty.write_line("str1.compare(str2) = ", str1.compare(str2));
                tty.write_line("str3:\"", str3, "\" str4:\"", str4, "\"");
                tty.write_line("str3.compare(str4) = ", str3.compare(str4));
                tty.write_line("str4:\"", str4, "\" str5:\"", str5, "\"");
                tty.write_line("str4.compare(str5) = ", str4.compare(str5));
                tty.write_line("str4.compare(str5, ignore_case<char>) = ", str4.compare(str5, ignore_case<char>));
            }
        }
    }

    int main(){
        xuser::inc::example();
        return 0;
    }
#endif
