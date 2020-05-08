#ifndef xpack_algo_test_binary_search
#define xpack_algo_test_binary_search
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_test_binary_search
        #include"define/base_type.hpp"
        #include"algo/binary_search.hpp"
        #include"lang/cxx/+.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xassert.hpp"
    #pragma pop_macro("xuser")

    xtest(binary_search) {
        constexpr uxx max_length = 128;
        static uxx buf[max_length];
        inc::cxx<uxx> c(buf, max_length);

        auto search = [&](auto target, auto mode){
            for (uxx i = 0; i < c.length(); i++){
                if (c[i] == target){
                    return i;
                }
                if (c[i] > target){
                    switch(mode){
                    case 0: break;
                    case 1: return i - 1;
                    case 2: return i;
                    }
                }
            }
            if (mode == 1 && c[c.length() - 1] < target){
                return c.length() - 1;
            }
            return not_exist;
        };

        for (uxx i = 0; i < c.length(); i++){
            c[i] = i * 2 + 1;
        }

        uxx actual;
        uxx wanted;
        
        for (uxx length = 1; length <= max_length; length++){
            c.length(length);

            for (uxx find = 0; find < length * 2 + 2; find++){
                actual = inc::binary_search<uxx>(c, find);
                wanted = search(find, 0);
                xassert(actual == wanted or c[actual] == c[wanted], length, find, actual, wanted);

                actual = inc::binary_search_match_up<uxx>(c, find);
                wanted = search(find, 1);
                xassert(actual == wanted or c[actual] == c[wanted], length, find, actual, wanted);

                actual = inc::binary_search_match_down<uxx>(c, find);
                wanted = search(find, 2);
                xassert(actual == wanted or c[actual] == c[wanted], length, find, actual, wanted);
            }
        }
    };
#endif
