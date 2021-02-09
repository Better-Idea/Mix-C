#pragma once
#include<unistd.h>
#include"define/base_type.hpp"

template<class ... args>
inline void cmd(asciis exe_path, asciis exe, args const & ... list){
    if (vfork() == 0){ // this thread will wait child finished
        execl(exe_path, exe, (asciis)list..., NULL);
        _exit(-1);
    }
}
