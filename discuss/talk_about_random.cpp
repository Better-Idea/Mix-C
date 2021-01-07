#define xuser mixc::powerful_cat
#include"docker/array.hpp"
#include"instruction/count_of_set.hpp"
#include"io/tty.hpp"
#include"math/const.hpp"
#include"math/random.hpp"

namespace xuser{
    f64                  sum;
    array<uxx, 64>       popcnt;
    array<uxx, 2>        count_of {};

    void test(){
        uxx max = 1024 * 1024;

        for(uxx i = 0; i < max; i++){
            uxx v       = random<uxx>();
            sum        += v;
            count_of[v % 2]++;
            popcnt[count_of_set(v)]++;
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

        // 置位位个数分布的情况
        for(uxx i = 0; i < popcnt.length(); i++){
            tty.write_line("\e set bit count:\e", ph::v{i}.l(2), popcnt[i]);
        }
    }
}

int main(){
    xuser::test();
    return 0;
}