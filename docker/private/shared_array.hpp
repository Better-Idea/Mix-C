// 注意：
// shared_array 默认是空数组，此时 attribute 域是不允许访问的
// shared_array::length() == 0
// shared_array::is_empty() == true
// shared_array::operator==(nullptr) == true
// shared_array::operator=(nullptr) 默认是将该数组指向空数组而不是 nullptr
// 如非必要请勿创建长度 ::length(0) 的数组
// 创建长度为 0 的数组与默认空数组的不同点
// shared_array::operator==(nullptr) == false
#ifndef xpack_docker_shared_array
#define xpack_docker_shared_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_shared_array
#include"docker/private/adapter.array_access.hpp"
#include"dumb/struct_type.hpp"
#include"gc/ref.hpp"
#include"macro/xis_nullptr.hpp"
#include"memory/new.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_shared_array{
    template<class final, class type, uxx rank, class attribute>
    xstruct(
        xtmpl(shared_array_t, final, type, rank, attribute),
        xpubb(inc::ref_array<
            shared_array_t<final, type, rank, attribute>,
            typename 
            shared_array_t<final, type, rank - 1, attribute>::the_t,
            attribute
        >)
    )
        using item_t = typename shared_array_t<final, type, rank - 1, attribute>::the_t;
        using base_t = inc::ref_array<the_t, item_t, attribute>;
    public:
        xseqptr(item_t);

        shared_array_t() : 
            shared_array_t(*(the_t *)& inc::empty_array_ptr) {
            static_assert(sizeof(inc::empty_array) >= sizeof(inc::struct_type<attribute>));
        }

        shared_array_t(shared_array_t const &) = default;

        template<class finalx>
        shared_array_t(shared_array_t<finalx, type, rank, attribute> const & self) : 
            shared_array_t((the_t &)(shared_array_t<finalx, type, rank, attribute> &)self){
        }

        shared_array_t(::length length) :
            base_t(length) {}

        template<class ... args>
        shared_array_t(item_t const & first, args const & ... rest) : 
            base_t(first, rest...){
        }

        template<class ... args>
        shared_array_t(::length length, args const & ... list) : 
            base_t(length, list...) {}

        template<class ... args>
        auto & operator()(::length length, args const & ... list){
            base_t::operator=(nullptr);
            new (this) base_t(length, list...);
            return thex;
        }

        template<class ... args>
        auto & operator()(item_t const & first, args const & ... rest){
            base_t::operator=(nullptr);
            new (this) base_t(first, rest...);
            return thex;
        }

        the_t & operator=(decltype(nullptr)){
            base_t::operator=(nullptr);
            new (this) the_t(*(the_t *)& inc::empty_array_ptr);
            return thex;
        }

        operator item_t *() const {
            return base_t::operator item_t * ();
        }

        item_t & operator[](uxx index) {
            return base_t::operator item_t * ()[index];
        }

        item_t const & operator[](uxx index) const {
            return base_t::operator item_t * ()[index];
        }

        attribute * operator->(){
            return base_t::operator->();
        }

        attribute const * operator->() const {
            return base_t::operator->();
        }

        xis_nullptr(
            the == *(the_t *)& inc::empty_array_ptr
        )

        xpubgetx(is_empty, bool) {
            return length() == 0;
        }

        xpubgetx(length, uxx) {
            return base_t::length();
        }
    $

    template<class final, class type, class attribute>
    struct shared_array_t<final, type, 0, attribute>{
        using the_t = type;
    };

    template<class final, class type, uxx rank, class attribute>
    using shared_array = inc::adapter_array_access<
        shared_array_t<final, type, rank, attribute>
    >;
}

#endif

#include"math/index_system.hpp"
#define xusing_docker_shared_array        ::mixc::docker_shared_array
