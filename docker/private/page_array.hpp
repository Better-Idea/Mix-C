#ifndef xpack_docker_page_array
#define xpack_docker_page_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_page_array::inc
#include"algo/mmu.hpp"
#include"docker/private/adapter.array_access.hpp"
#include"interface/ranger.hpp"
#include"macro/xcheck_binary_aligned.hpp"
#include"meta/is_same.hpp"
#include"utils/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_page_array{
    enum class pages_t : uxx {};
    enum class init_size_t : uxx{};

    inline constexpr pages_t operator ""_pages(uxx value){
        return pages_t(value);
    }

    inline constexpr init_size_t operator ""_init_size(uxx value){
        return init_size_t(value);
    }

    // using item_t = int;
    // constexpr auto a0_v = 1_init_size;
    // constexpr auto a1_v = 1_pages;

    template<auto a0_v, auto a1_v>
    constexpr bool check_targ =
        (
            inc::is_same<decltype(a0_v), init_size_t> and
            inc::is_same<decltype(a1_v), pages_t>
        ) or
        (
            inc::is_same<decltype(a1_v), init_size_t> and
            inc::is_same<decltype(a0_v), pages_t>
        ) or
        (
            inc::is_same<decltype(a0_v), init_size_t> and
            inc::is_same<decltype(a1_v), init_size_t> and 
            uxx(a0_v) > 0 and
            uxx(a1_v) == not_exist
        ) or
        (
            inc::is_same<decltype(a0_v), pages_t> and
            inc::is_same<decltype(a1_v), pages_t> and 
            uxx(a0_v) > 0 and
            uxx(a1_v) == not_exist
        );

    template<
        class final_t, 
        class itemx_t, 
        auto  a0_v = pages_t(not_exist), 
        auto  a1_v = init_size_t(not_exist)
    >
    requires(check_targ<a0_v, a1_v>)
    xstruct(
        xtmpl(page_array_t, final_t, itemx_t, a0_v, a1_v)
    )
    public:
        using item_t        = itemx_t;
    private:
        enum : uxx{
            // 先匹配 a0_v，因为 a1_v 可能缺省
            pages_tmp       = inc::is_same<decltype(a0_v), pages_t> ? uxx(a0_v) : uxx(a1_v),
            init_size_tmp   = inc::is_same<decltype(a0_v), init_size_t> ? uxx(a0_v) : uxx(a1_v),
            is_static_page  = pages_tmp != not_exist,
            pages_v         = pages_tmp == not_exist ? 1 : pages_tmp,
            init_size_v     = init_size_tmp == not_exist ? 1 : init_size_tmp,
        };

        union{
            item_t *    m_table[pages_v];
            item_t **   m_table_ptr;
        };

        uxx             m_length;

        using var = inc::var_array<
            init_size_v, 
            is_static_page ? inc::with_fixed_page_table : not inc::with_fixed_page_table
        >;

        xcheck_binary_aligned(init_size_v)
    public:
        page_array_t(the_t const &) = default;

        page_array_t():
            m_table_ptr(nullptr),
            m_length(0){
        }

        template<class finalx_t>
        page_array_t(page_array_t<finalx_t, item_t, a0_v, a1_v> const & self):
            page_array_t((the_t &)self){
        }

        void push(item_t const & item){
            item_t **   table_ptr   = m_table;
            item_t ** & tab         = is_static_page ? table_ptr : m_table_ptr;
            var::push(xref(tab), xref(m_length), item, inc::default_alloc<void>, inc::default_free<void>);
        }

        void pop(item_t * item){
            item_t **   table_ptr   = m_table;
            item_t ** & tab         = is_static_page ? table_ptr : m_table_ptr;
            var::pop(xref(tab), xref(m_length), item, inc::default_alloc<void>, inc::default_free<void>);
        }

        item_t pop(){
            item_t value{};
            this->pop(xref(value));
            return value;
        }

        item_t & operator[](uxx index){
            item_t **   table_ptr   = m_table;
            item_t ** & tab         = is_static_page ? table_ptr : m_table_ptr;
            return var::access(tab, index);
        }

    protected:
        ~page_array_t(){
            item_t **   table_ptr   = m_table;
            item_t ** & tab         = is_static_page ? table_ptr : m_table_ptr;
            var::clear(xref(tab), xref(m_length), inc::default_free<void>);
        }

    public:
        xranger(item_t)
    public:
        xpubget(length)
    $;

    template<class final_t, class item_t, auto a0_v, auto a1_v>
    using page_array = inc::adapter_array_access<
        page_array_t<final_t, item_t, a0_v, a1_v>
    >;
}

namespace mixc::docker_page_array::origin{
    using mixc::docker_page_array::pages_t;
    using mixc::docker_page_array::init_size_t;
    using mixc::docker_page_array::operator ""_pages;
    using mixc::docker_page_array::operator ""_init_size;
}

#endif

#include"math/index_system.hpp"

#undef  xusing_docker_page_array
#define xusing_docker_page_array     ::mixc::docker_page_array
