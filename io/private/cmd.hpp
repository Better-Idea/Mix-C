#pragma once
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include"define/base_type.hpp"

template<class ... args_t>
inline bstate_t cmd(asciis exe_path, asciis exe, args_t const & ... list){
    auto is_success = true;
    auto pid = vfork();

    if (pid == 0){ // this thread will wait child finished
        auto fd = open("/dev/null", O_RDWR);
        dup2(fd, STDERR_FILENO);
        execl(exe_path, exe, (asciis)list..., NULL);
        _exit(-1);
    }

    auto ret_code   = 0;
    waitpid(pid, & ret_code, 0);
    return ret_code == 0 ? bstate_t::success : bstate_t::fail;
}
