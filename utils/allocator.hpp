#ifndef xpack_utils_allocator
#define xpack_utils_allocator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_allocator::inc
#include"macro/xdebug.hpp"
#include"macro/xexport.hpp"
#include"macro/xtypeid.hpp"
#include"macro/xnew.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_allocator{
    extern void  tiny_process_message();
    extern voidp tiny_alloc(uxx bytes);
    extern void  tiny_free(voidp ptr, uxx bytes);
}

namespace mixc::utils_allocator::origin{
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

    extern voidp malloc(uxx bytes);
    extern voidp malloc_aligned(uxx bytes, uxx align_bytes);
    extern void  mfree(voidp ptr);
    extern void  mfree_aligned(voidp ptr);

    template<class type, class ... args_t>
    inline type * alloc_with_initial(memory_size bytes, args_t const & ... list){
        auto ptr = (type *)tiny_alloc(bytes);
        xdebug(im_alloc_with_initial, xtypeid(type).name, ptr, bytes);
        return xnew(ptr) type(list...);
    }

    template<class type, class ... args_t>
    inline type * alloc_with_initial(args_t const & ... list){
        return alloc_with_initial<type>(memory_size(sizeof(type)), list...);
    }

    template<class type>
    inline type * alloc(memory_size bytes){
        auto ptr = (type *)tiny_alloc(bytes);
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
        tiny_free(ptr, bytes);
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

    inline void process_message(){
        tiny_process_message();
    }

    extern uxx used_bytes();
    extern uxx alive_object();
    extern uxx alive_pages();
}

#define xmemory_sizeof(...)       \
    ::mixc::utils_allocator::origin::memory_size{ sizeof(__VA_ARGS__) }

#endif

xexport_space(mixc::utils_allocator::origin)