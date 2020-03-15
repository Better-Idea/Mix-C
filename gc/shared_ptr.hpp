#pragma once
#include"gc/private/ref.hpp"

namespace mixc{
    template<class type> struct shared_ptr;
    template<class type>
    struct shared_ptr : mixc::inner_gc::meta<shared_ptr<type>, dummy_t, type, false> {
        using meta = mixc::inner_gc::meta<shared_ptr<type>, dummy_t, type, false>;
        explicit shared_ptr(ini) : meta(length(0)) {}

        template<class ... args>
        explicit shared_ptr(ini, args const & ... list) : meta(length(0), list...) {}
        shared_ptr() {}

        template<class ... args>
        shared_ptr<type> & operator()(ini, args const & ... list){
            using metap = meta *;
            this->~meta();
            new (metap(this)) meta(length(0), list...);
            return this[0];
        }
    };
}
