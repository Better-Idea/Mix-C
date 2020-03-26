#ifndef xpack_memory_alloctor
#define xpack_memory_alloctor
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memory_alloctor
        #include"define/base_type.hpp"
        #include"macro/xdebug.hpp"
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
            auto ptr = (type *)malloc(sizeof(type));
            xdebug(im_alloc_with_initial, "alloc mem:%p  size:%llu\n", ptr, u64(sizeof(type)));
            return new(ptr) type(list...);
        }

        template<class type, class ... args>
        inline type * alloc_with_initial(memory_size bytes, args const & ... list){
            auto ptr = (type *)malloc(bytes);
            xdebug(im_alloc_with_initial, "alloc mem:%p  size:%llu\n", ptr, u64(bytes));
            return new(ptr) type(list...);
        }

        template<class type>
        inline type * alloc(memory_size bytes){
            auto ptr = (type *)malloc(bytes);
            xdebug(im_alloc, "alloc mem:%p  size:%llu\n", ptr, u64(bytes));
            return ptr;
        }

        template<class type>
        inline type * alloc(){
            return alloc<type>(memory_size(sizeof(type)));
        }

        template<class type>
        inline void free(type * ptr, memory_size bytes){
            xdebug(im_free, "free  mem:%p  size:%llu\n", ptr, u64(bytes));
            ::free(ptr);
        }

        template<class type>
        inline void free(type * ptr){
            free(ptr, memory_size(0));
        }

        template<class type>
        inline void free_with_destroy(type * ptr, memory_size bytes){
            xdebug(im_free_with_destroy, "free  mem:%p  size:%llu\n", ptr, u64(bytes));
            ptr->~type();
            ::free(ptr);
        }

        template<class type>
        inline void free_with_destroy(type * ptr){
            free_with_destroy(ptr, memory_size(sizeof(type)));
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
