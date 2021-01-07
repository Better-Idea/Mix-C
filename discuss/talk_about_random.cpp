#define xuser mixc::powerful_cat
#include"docker/array.hpp"
#include"instruction/count_of_set.hpp"
#include"io/tty.hpp"
#include"math/const.hpp"
#include"math/random.hpp"

namespace xuser{
    f64                  sum;
    array<uxx, 64>       popcnt;
    array<uxx, 64>       contiguous_zero;
    array<uxx, 64>       contiguous_one;
    array<uxx, 2>        count_of {};

    void test(){
        uxx max         = 1024 * 1024;
        uxx old         = 0;
        uxx cnt_one     = 0;
        uxx cnt_zero    = 0;

        for(uxx i = 0; i < max; i++){
            uxx v       = random<uxx>();
            sum        += v;
            count_of[v % 2]++;
            popcnt[count_of_set(v)]++;

            if (v % 2 == 1){
                if (old == 1){
                    cnt_one++;
                }
                else{
                    contiguous_one[cnt_one]++;
                    cnt_one     = 0;
                }
                old             = 1;
            }
            else{
                if (old == 1){
                    contiguous_zero[cnt_zero]++;
                    cnt_zero    = 0;
                }
                else{
                    cnt_zero++; 
                }
                old             = 0;
            }
        }

        // 0 和 1 分布的情况
        tty.write_line("0-1 distribution");
        tty.write_line(count_of[0]);
        tty.write_line(count_of[1]);
        tty.write_line();

        // 平均值和期望值
        tty.write_line("\e times random<>() call", max);
        tty.write_line("average:", sum / max);
        tty.write_line("expect :", max_value_of<ixx>);
        tty.write_line();

        // 置位位个数分布的情况
        tty.write_line("set bit count distribution, bits:count");

        for(uxx i = 0; i < popcnt.length(); i++){
            tty.write("\e:\e, ", ph::v{i}.l(2), ph::v{popcnt[i]}.l(6), (i + 1) % 8 == 0 ? "\n" : "");
        }

        tty.write_line();

        // 连续的 0 的分布情况
        tty.write_line("contiguous zero distribution");

        for(uxx i = 0; i < contiguous_zero.length(); i++){
            tty.write("\e:\e, ", ph::v{i}.l(2), ph::v{contiguous_zero[i]}.l(6), (i + 1) % 8 == 0 ? "\n" : "");
        }

        tty.write_line();

        // 连续的 1 的分布情况
        tty.write_line("contiguous one distribution");

        for(uxx i = 0; i < contiguous_one.length(); i++){
            tty.write("\e:\e, ", ph::v{i}.l(2), ph::v{contiguous_one[i]}.l(6), (i + 1) % 8 == 0 ? "\n" : "");
        }

        tty.write_line();
    }
}

int main(){
    xuser::test();
    return 0;
}