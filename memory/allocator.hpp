#ifndef xpack_memory_alloctor
#define xpack_memory_alloctor
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::memory_alloctor
    #include"define/base_type.hpp"
    #include"macro/xdebug.hpp"
    #include"macro/xtypeid.hpp"
    #include"memory/new.hpp"
    #include"memory/private/tiny_allocator.hpp"

    namespace mixc::memory_alloctor{
        // 单线程
        inline static inc::tiny_allocator mem;
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
            auto ptr = (type *)mem.alloc(bytes);
            xdebug(im_alloc_with_initial, xtypeid(type).name, ptr, bytes);
            return new(ptr) type(list...);
        }

        template<class type, class ... args>
        inline type * alloc_with_initial(args const & ... list){
            return alloc_with_initial<type>(memory_size(sizeof(type)), list...);
        }

        template<class type>
        inline type * alloc(memory_size bytes){
            auto ptr = (type *)mem.alloc(bytes);
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
            mem.free(ptr, bytes);
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

        inline uxx used_bytes(){
            return mem.used_bytes();
        }

        inline uxx need_free_count(){
            return mem.need_free_count();
        }
    }
    #pragma pop_macro("xuser")

#endif

namespace xuser::inc{
    using namespace ::mixc::memory_alloctor::origin;
}
