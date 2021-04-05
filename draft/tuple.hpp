#ifndef xpack_draft_tuple
#define xpack_draft_tuple
#pragma push_macro("xuser")
#undef  xuser   
#define xuser mixc::draft_tuple::inc
#include"define/base_type.hpp"
#include"define/nullref.hpp"
#include"meta_seq/tget.hpp"
#include"meta_seq/tlist.hpp"
#include"macro/xnew.hpp"
#include"meta/is_same.hpp"
#include"meta/remove_const.hpp"
#include"meta_seq/tnull.hpp"
#include"meta_seq/tpartial.hpp"
#pragma pop_macro("xuser")

namespace mixc::draft_tuple{
    template<class object_t>
    constexpr void wrap_init(voidp self){
        xnew(self) object_t();
    }

    template<class object_t>
    constexpr void wrap_initx(voidp self, voidp args){
        xnew(self) object_t(*(object_t *)args);
    }

    template<class object_t>
    constexpr void wrap_destroy(voidp self){
        ((object_t *)self)->~object_t();
    }
    
    template<uxx i_v, uxx depth_v, class tlist>
    constexpr auto find_alpha(tlist){
        using partial   = inc::tpartial<tlist>;
        using front     = typename partial::front;

        if constexpr (depth_v < 128){
            if constexpr (i_v >= front::length){
                return find_alpha<i_v - front::length, depth_v + 1>(typename partial::new_list{});
            }
            else{
                struct{
                    bool                    find    = true;
                    inc::tget<front, i_v> * value   = nullptr;
                    inc::tlist<>            rest    = {};
                } result;
                return result;
            }
        }
        else{
            struct{
                bool                        find    = false;
                inc::tnull                * value   = nullptr;
                uxx                         i       = i_v;
                tlist                       rest    = {};
            } result;
            return result;
        }
    }

    template<uxx i_v, uxx depth_v, class tlist>
    constexpr auto find_beta(tlist args){
        constexpr auto result = find_alpha<i_v, 0>(args);
        if constexpr (result.find or depth_v == 32 or result.rest.length == 0){
            return result;
        }
        else {
            return find_beta<result.i, depth_v + 1>(result.rest);
        }
    }

    template<uxx i_v, class tlist>
    constexpr auto find_gamma(tlist args){
        constexpr auto result = find_beta<i_v, 0>(args);
        if constexpr (result.find or result.rest.length == 0){
            return result;
        }
        else {
            return find_gamma<result.i>(result.rest);
        }
    }

    template<class ... args_t>
    constexpr auto configure(){
        using iv_t = void(*)(voidp);
        using ix_t = void(*)(voidp, voidp);

        enum : uxx{
            length = sizeof...(args_t)
        };

        struct meta{
            iv_t    init[length]        = {nullptr};
            ix_t    initx[length]       = {nullptr};
            iv_t    destroy[length]     = {nullptr};
            uxx     offset[length + 1]  = {0};
            uxx     total_bytes         = {0};
        } m;

        uxx    lut_alignof[]    = { alignof(args_t)..., 0 };
        uxx    lut_sizeof[]     = { sizeof(args_t)..., 0 };
        uxx  & max_alignof      = lut_alignof[length];
        iv_t   init[length]     = { & wrap_init<args_t>... };
        ix_t   initx[length]    = { & wrap_initx<args_t>... };
        iv_t   destroy[length]  = { & wrap_destroy<args_t>... };

        for(uxx i = 0; i < length; i++){
            uxx mask            = lut_alignof[i + 1] - 1;
            m.init[i]           = init[i];
            m.initx[i]          = initx[i];
            m.destroy[i]        = destroy[i];
            m.offset[i + 1]     = lut_sizeof[i] + m.offset[i];
            m.offset[i + 1]    += mask;
            m.offset[i + 1]    &= ~mask;

            if (max_alignof < lut_alignof[i]){
                max_alignof     = lut_alignof[i];
            }
        }
        m.total_bytes           = m.offset[length];
        return m;
    }
}

namespace mixc::draft_tuple::origin{
    template<class ... args_t>
    struct tuple{
        tuple(args_t const & ... items) :
            tuple((args_t &)items...){
        }

        tuple(args_t & ... items){
            constexpr auto meta = configure<args_t...>();
            voidp arg_list[]    = { & items... };

            for(uxx i = 0; i < this->length(); i++){
                uxx offset      = meta.offset[i];
                meta.initx[i](mem + offset, arg_list[i]);
            }
        }

        static constexpr uxx length(){
            return sizeof...(args_t);
        }

        tuple(){
            constexpr auto meta = configure<args_t...>();

            for(uxx i = 0; i < this->length(); i++){
                uxx offset      = meta.offset[i];
                meta.init[i](mem + offset);
            }
        }

        template<ixx index_v>
        auto & get(){
            constexpr auto meta     = configure<args_t...>();
            constexpr uxx i_real    = 
                index_v >= 0 ? uxx(index_v) : sizeof...(args_t) + index_v;

            if constexpr (i_real < sizeof...(args_t)){
                using list          = inc::tlist<args_t...>;
                using typep         = inc::remove_const<
                    decltype(find_gamma<i_real>(list{}).value)
                >;
                return *typep(mem + meta.offset[i_real]);
            }
            else{
                return inc::nullref;
            }
        }

        template<ixx index_v, class type_t>
        void set(type_t const & value){
            this->get<index_v>() = value;
        }

    private:
        u08 mem[configure<args_t...>().total_bytes];
    };
}

#endif

xexport_space(mixc::draft_tuple::origin)
