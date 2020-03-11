#pragma once
#include"define/base_type.hpp"

namespace mixc{
    /*
    用作将指定类型b隐式转换成类型a
    场景：
    template<class ... args>
    auto function(args const & ... list){
        implicit<int &> seq[] = { list... };
        ...
    }
    */
    template<class a>
    struct implicit {
        template<class b>
        implicit(b const & value) : 
            value((a)(b &)value) {}

        operator a () const {
            return value;
        }

        auto operator -> () const {
            return & value;
        }
    private:
        a value;
    };
}
