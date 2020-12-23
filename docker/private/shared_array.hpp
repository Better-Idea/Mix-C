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
#define xuser mixc::docker_shared_array::inc
#include"docker/private/adapter.array_access.hpp"
#include"dumb/struct_type.hpp"
#include"gc/ref.hpp"
#include"macro/xis_nullptr.hpp"
#include"macro/xnew.hpp"
#include"meta/has_cast.hpp"
#include"meta/has_constructor.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_shared_array{
    template<class final, class type, uxx rank, class attribute, bool is_binary_aligned_alloc>
    xstruct(
        xtmpl(shared_array_t, final, type, rank, attribute, is_binary_aligned_alloc),
        xpubb(inc::ref_array<
            final,
            typename 
            shared_array_t<final, type, rank - 1, attribute, is_binary_aligned_alloc>::the_t,
            attribute,
            is_binary_aligned_alloc
        >)
    )
        using item_t = typename shared_array_t<final, type, rank - 1, attribute, is_binary_aligned_alloc>::the_t;
        using base_t = inc::ref_array<final, item_t, attribute, is_binary_aligned_alloc>;
        using typename base_t::item_initial_invoke;
        using typename base_t::item_initial_invokex;
        using base_t::operator==;
        using base_t::operator!=;
    public:
        shared_array_t()                        = default;
        shared_array_t(the_t const &)           = default;
        shared_array_t(the_t &&)                = default;
        the_t & operator=(the_t const &)        = default;
        the_t & operator=(the_t &&)             = default;

        shared_array_t(::length length) :
            base_t(length) {}

        template<class finalx>
        shared_array_t(shared_array_t<finalx, type, rank, attribute, is_binary_aligned_alloc> const & self) : 
            shared_array_t((the_t &)(shared_array_t<finalx, type, rank, attribute, is_binary_aligned_alloc> &)self){
        }

        template<class ... args>
        requires(... && inc::has_cast<item_t, args>)
        shared_array_t(item_t const & first, args const & ... rest) : 
            base_t(first, rest...){
        }

        template<class ... args>
        requires(inc::has_constructor<item_t, void(args const &...)>)
        shared_array_t(::length length, args const & ... list) : 
            base_t(length, list...) {}

        template<class initial_invoke>
        requires(
            inc::has_cast<item_initial_invoke , initial_invoke> or 
            inc::has_cast<item_initial_invokex, initial_invoke>
        )
        shared_array_t(::length length, initial_invoke const & initial) :
            base_t(length, initial){
        }

        template<class ... args>
        requires(inc::has_constructor<item_t, void(args const &...)>)
        auto & operator()(::length length, args const & ... list){
            the_t{length, list...}.swap(this);
            return thex;
        }

        template<class ... args>
        requires(... && inc::has_cast<item_t, args>)
        auto & operator()(item_t const & first, args const & ... rest){
            the_t{first, rest...}.swap(this);
            return thex;
        }

        operator item_t *() {
            return base_t::operator item_t * ();
        }

        operator item_t const *() const {
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

        xpubgetx(is_empty, bool) {
            return length() == 0;
        }

        xpubget_prosetx(length, uxx)
            xr{ return base_t::length(); }
            xw{ base_t::length(value); }
    $

    template<class final, class type, class attribute, bool is_binary_aligned_alloc>
    struct shared_array_t<final, type, 0, attribute, is_binary_aligned_alloc>{
        using the_t = type;
    };

    template<class final, class type, uxx rank, class attribute, bool is_binary_aligned_alloc>
    using shared_array = inc::adapter_array_access<
        shared_array_t<final, type, rank, attribute, is_binary_aligned_alloc>
    >;
}

#endif

#include"math/index_system.hpp"
#define xusing_docker_shared_array        ::mixc::docker_shared_array
