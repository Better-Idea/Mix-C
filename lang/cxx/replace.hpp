#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_replace
#define xpack_lang_cxx_replace
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_replace
        #include"define/base_type.hpp"
        #include"docker/darray.hpp"
        #include"interface/can_alloc.hpp"
        #include"interface/can_compare.hpp"
        #include"lang/cxx/index_of_first.hpp"
        #include"lang/cxx.hpp"
        #include"memop/copy.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_replace{
        // using item = char;
        // template<class item> struct core;
        // template<>
        // struct core<item> : inc::cxx<item> {

        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto replace(
                the_t                     old_value, 
                the_t                     new_value, 
                inc::can_compare<item>    compare, 
                inc::can_alloc<item> alloc) const {

                constexpr uxx buf_size = 64;
                struct {
                    uxx              i = 0;
                    uxx              buf[buf_size];
                    uxx              total_length;
                    uxx              dis;
                    inc::darray<uxx> heap;

                    void push(uxx value){
                        uxx index = i++;
                        
                        if (total_length += dis; index < buf_size){
                            buf[index] = value;
                            return;
                        }
                        if (index - buf_size - heap.length() == 0){
                            inc::darray<uxx> new_heap { inc::length(index) };
                            inc::copy(new_heap, heap, heap.length());
                            heap = new_heap;
                        }
                        heap[index - buf_size] = value;
                    }
                } pack;

                pack.dis          = new_value.length() - old_value.length();
                pack.total_length = the.length();

                the.index_of_first(old_value, [&](uxx index){
                    pack.push(index);
                });

                the_t r { alloc(pack.total_length), pack.total_length };
                the_t target     = r;
                the_t source     = the;
                bool  only_stack = pack.i < buf_size;
                uxx   top        = only_stack ? pack.i : buf_size;

                auto && replace = [&](uxx length){
                    inc::copy<item>(target, source, length);
                    target = target.backward(length);
                    source = source.backward(length + old_value.length());
                    inc::copy<item>(target, new_value, length = new_value.length());
                    target = target.backward(length);
                };

                auto && remove = [&](uxx length){
                    inc::copy<item>(target, source, length);
                    target = target.backward(length);
                    source = source.backward(length + old_value.length());
                };

                auto && change = 
                    new_value.is_empty() ?  remove :  replace;

                for(uxx i = 0; i < top; i++){
                    change(pack.buf[i]);
                }
                for(ixx i = 0; i < ixx(pack.i - buf_size); i++){
                    change(pack.heap[i]);
                }
                return r;
            }
        };
    }
#endif

namespace mixc::lang_cxx_replace::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item>{
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final replace(final old_value, final new_value, inc::can_alloc<item> alloc) const {
            return the.replace(old_value, new_value, inc::default_compare<item>, alloc);
        }

        final replace(
            final                  old_value, 
            final                  new_value, 
            inc::can_compare<item> compare, 
            inc::can_alloc<item>   alloc) const {
            return the.replace(old_value, new_value, compare, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_replace::xuser
