#ifndef xpack_docker_page_array
#define xpack_docker_page_array
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_page_array::inc
#include"algo/mmu.hpp"
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
    // constexpr auto a1_v = 0_pages;

    template<class item_t, auto a0_v = 1_init_size, auto a1_v = 0_pages>
    requires(
        (inc::is_same<decltype(a0_v), init_size_t> and inc::is_same<decltype(a1_v), pages_t>) or
        (inc::is_same<decltype(a1_v), init_size_t> and inc::is_same<decltype(a0_v), pages_t>)
    )
    struct page_array{
    private:
        enum : uxx{
            pages_v = inc::is_same<decltype(a0_v), pages_t> ? uxx(a0_v) : uxx(a1_v),
            init_size_tmp = inc::is_same<decltype(a0_v), init_size_t> ? uxx(a0_v) : uxx(a1_v),
            init_size_v = init_size_tmp ? init_size_tmp : 1,
        };

        using var = inc::var_array<
            init_size_v, 
            pages_v != 0 ? inc::with_fixed_page_table : not inc::with_fixed_page_table
        >;

    public:
        page_array():
            m_table_ptr(nullptr),
            m_length(0){
        }

        void push(item_t const & item){
            item_t ** tab   = m_table_ptr;
            var::push(xref(tab), xref(m_length), item, inc::default_alloc<void>, inc::default_free<void>);
        }

        void pop(item_t * item){
            item_t ** tab   = m_table_ptr;
            var::pop(xref(tab), xref(m_length), item, inc::default_alloc<void>, inc::default_free<void>);
        }

        item_t & operator[](uxx index){
            item_t ** tab   = m_table_ptr;
            return var::access(tab, index);
        }
    private:
        union{
            item_t *    m_table[pages_v + !pages_v];
            item_t **   m_table_ptr;
        };

        uxx             m_length;
    };
}

#endif
