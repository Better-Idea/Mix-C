#ifndef xpack_utils_memory
#define xpack_utils_memory
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::utils_memory::inc
#include"macro/xdebug.hpp"
#include"macro/xexport.hpp"
#include"macro/xtypeid.hpp"
#include"macro/xnew.hpp"
#pragma pop_macro("xuser")

namespace mixc::utils_memory{
    extern voidp tiny_alloc(uxx bytes);
    extern void  tiny_free(voidp ptr, uxx bytes);
}

namespace mixc::utils_memory::origin::memory{
    struct size{
        explicit size(uxx value) : 
            value(value){
        }

        operator uxx (){
            return value;
        }
    private:
        uxx value;
    };

    extern voidp malloc(uxx bytes);
    extern voidp malloc_aligned(uxx bytes, uxx align_bytes);
    extern void  mfree(voidp ptr);
    extern void  mfree_aligned(voidp ptr);

    template<class type, class ... args_t>
    inline type * alloc_with_initial(size bytes, args_t const & ... list){
        auto ptr = (type *)tiny_alloc(bytes);
        xdebug(im_alloc_with_initial, xtypeid(type).name, ptr, bytes);
        return xnew(ptr) type(list...);
    }

    template<class type, class ... args_t>
    inline type * alloc_with_initial(args_t const & ... list){
        return alloc_with_initial<type>(size(sizeof(type)), list...);
    }

    template<class type>
    inline type * alloc(size bytes){
        auto ptr = (type *)tiny_alloc(bytes);
        xdebug(im_alloc, xtypeid(type).name, ptr, bytes);
        return ptr;
    }

    template<class type>
    inline type * alloc(){
        return alloc<type>(size(sizeof(type)));
    }

    template<class type>
    inline void free(type const * ptr, size bytes){
        xdebug(im_free, xtypeid(type).name, ptr, bytes);
        tiny_free((type *)ptr, bytes);
    }

    template<class type>
    inline void free(type const * ptr){
        free(ptr, size(sizeof(type)));
    }

    template<class type>
    inline void free_with_destroy(type const * ptr, size bytes){
        ptr->~type();
        free(ptr, bytes);
    }

    template<class type>
    inline void free_with_destroy(type const * ptr){
        free_with_destroy(ptr, size(sizeof(type)));
    }

    template<class type>
    constexpr auto default_alloc = [](uxx bytes) -> type * {
        return alloc<type>(size{bytes});
    };

    template<class type>
    constexpr auto default_free = [](type * ptr, uxx bytes){
        free(ptr, size{bytes});
    };

    extern uxx used_bytes();
    extern uxx alive_object();
    extern uxx alive_pages();
    extern void handle_async_memory_event();
}

#define xsizeof(...)       \
    ::mixc::utils_memory::origin::size{ sizeof(__VA_ARGS__) }

#endif

xexport_space(mixc::utils_memory::origin)