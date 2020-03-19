#pragma once
#include"define/base_type.hpp"
#include"macro/xdebug.hpp"
#include<malloc.h>

namespace mixc{
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
    inline type * alloc(memory_size bytes = memory_size(0)){
        auto size = bytes == 0 ? sizeof(type) : bytes;
        auto ptr = (type *)malloc(size);
        xdebug(im_alloc, "alloc mem:%p  size:%llu\n", ptr, u64(bytes));
        return ptr;
    }

    template<class type>
    inline void free(type * ptr, memory_size bytes = memory_size(0)){
        xdebug(im_free, "free  mem:%p  size:%llu\n", ptr, u64(bytes));
        ::free(ptr);
    }

    template<class type>
    inline void free_with_destroy(type * ptr, memory_size bytes = memory_size(0)){
        xdebug(im_free_with_destroy, "free  mem:%p  size:%llu\n", ptr, u64(bytes));
        ptr->~type();
        ::free(ptr);
    }

    namespace memory_inner{
        inline auto the_size_t(){
            if constexpr (sizeof(void *) == 4){
                return (unsigned int)(0);
            }
            else{
                return (unsigned long int)(0);
            }
        }
    }
}

inline void * operator new(decltype(mixc::memory_inner::the_size_t()) bytes, void * ptr) {
    return ptr;
}
