#pragma once
#include"meta_seq/tappend.hpp"

namespace mixc{
    template<class tlist, class item> using tenque = tappend<tlist, item>;
}
