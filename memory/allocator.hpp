#ifndef xpack_memory_alloctor
#define xpack_memory_alloctor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memory_alloctor::inc
#include"macro/xdebug.hpp"
#include"macro/xexport.hpp"
#include"macro/xtypeid.hpp"
#include"macro/xnew.hpp"
#pragma pop_macro("xuser")

namespace mixc::memory_alloctor{
    voidp malloc(uxx bytes);
    void  mfree(voidp ptr, uxx bytes);
}

namespace mixc::memory_alloctor::origin{
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
    inline type * alloc_with_initial(memory_size bytes, args const & ... list){
        auto ptr = (type *)malloc(bytes);
        xdebug(im_alloc_with_initial, xtypeid(type).name, ptr, bytes);
        return xnew(ptr) type(list...);
    }

    template<class type, class ... args>
    inline type * alloc_with_initial(args const & ... list){
        return alloc_with_initial<type>(memory_size(sizeof(type)), list...);
    }

    template<class type>
    inline type * alloc(memory_size bytes){
        auto ptr = (type *)malloc(bytes);
        xdebug(im_alloc, xtypeid(type).name, ptr, bytes);
        return ptr;
    }

    template<class type>
    inline type * alloc(){
        return alloc<type>(memory_size(sizeof(type)));
    }

    template<class type>
    inline void free(type * ptr, memory_size bytes){
        xdebug(im_free, xtypeid(type).name, ptr, bytes);
        mfree(ptr, bytes);
    }

    template<class type>
    inline void free(type * ptr){
        free(ptr, memory_size(sizeof(type)));
    }

    template<class type>
    inline void free_with_destroy(type * ptr, memory_size bytes){
        ptr->~type();
        free(ptr, bytes);
    }

    template<class type>
    inline void free_with_destroy(type * ptr){
        free_with_destroy(ptr, memory_size(sizeof(type)));
    }

    template<class type>
    constexpr auto default_alloc = [](uxx bytes) -> type * {
        return alloc<type>(memory_size{bytes});
    };

    template<class type>
    constexpr auto default_free = [](type * ptr, uxx bytes){
        free(ptr, memory_size{bytes});
    };

    uxx used_bytes();
    uxx need_free_count();
    uxx alive_pages();
}

#endif

xexport_space(mixc::memory_alloctor::origin)