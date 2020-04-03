#ifndef xpack_memory_alloctor
#define xpack_memory_alloctor
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memory_alloctor
        #include"define/base_type.hpp"
        #include"macro/xdebug.hpp"
        #include"macro/xtypeid.hpp"
        #include"memory/new.hpp"
        #include<malloc.h>
    #pragma pop_macro("xuser")

    namespace mixc::memory_alloctor{
        struct memory_size{
            explicit memory_size(uxx size) : 
                size(size){
            }
            operator uxx (){
                return size;
            }
        private:
            uxx size;
        };

        template<class type, class ... args>
        inline type * alloc_with_initial(args const & ... list){
            return alloc_with_initial<type>(memory_size(sizeof(type)), list...);
        }

        template<class type, class ... args>
        inline type * alloc_with_initial(memory_size bytes, args const & ... list){
            auto mem = (type *)malloc(bytes);
            xdebug(im_alloc_with_initial, xtypeid(type).name, mem, bytes);
            return new(mem) type(list...);
        }

        template<class type>
        inline type * alloc(memory_size bytes){
            auto mem = (type *)malloc(bytes);
            xdebug(im_alloc, xtypeid(type).name, mem, bytes);
            return mem;
        }

        template<class type>
        inline type * alloc(){
            return alloc<type>(memory_size(sizeof(type)));
        }

        template<class type>
        inline void free(type * mem, memory_size bytes){
            xdebug(im_free, xtypeid(type).name, mem, bytes);
            ::free(mem);
        }

        template<class type>
        inline void free(type * mem){
            free(mem, memory_size(sizeof(type)));
        }

        template<class type>
        inline void free_with_destroy(type * mem, memory_size bytes){
            xdebug(im_free_with_destroy, xtypeid(type).name, mem, bytes);
            mem->~type();
            ::free(mem);
        }

        template<class type>
        inline void free_with_destroy(type * mem){
            free_with_destroy(mem, memory_size(sizeof(type)));
        }
    }
#endif

namespace xuser::inc{
    using ::mixc::memory_alloctor::alloc;
    using ::mixc::memory_alloctor::alloc_with_initial;
    using ::mixc::memory_alloctor::free;
    using ::mixc::memory_alloctor::free_with_destroy;
    using ::mixc::memory_alloctor::memory_size;
}
