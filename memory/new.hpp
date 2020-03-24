#pragma once
#ifndef _NEW
    namespace mixc::dumb{
        inline auto the_size_t(){
            if constexpr (sizeof(void *) == 4){
                return (unsigned int)(0);
            }
            else{
                return (unsigned long int)(0);
            }
        }
    }

    inline void * operator new(
        decltype(
            mixc::dumb::the_size_t()
        ) bytes, void * ptr
    ) {
        return ptr;
    }
#endif
