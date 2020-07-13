#ifndef xpack_docker_private_dbit_indicator
#define xpack_docker_private_dbit_indicator
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::docker_dbit_indicator
    #include"define/base_type.hpp"
    #include"docker/private/bit_indicator_adapter.hpp"
    #include"dumb/disable_copy.hpp"
    #include"interface/can_alloc.hpp"
    #include"lock/atom_swap.hpp"
    #include"macro/xstruct.hpp"
    #include"memop/zeros.hpp"
    #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_dbit_indicator{
        xstruct(
            xname(dbit_indicator_t),
            xpubb(inc::disable_copy),
            xprif(pbmp      , uxx *),
            xprif(pheight   , uxx),
            xprif(psize     , uxx),
            xprif(pneed_free, bool)
        )
            using final = the_t;

            dbit_indicator_t() : 
                pbmp(nullptr), pheight(0), psize(0) {}

            dbit_indicator_t(uxx bits, inc::can_alloc<uxx> alloc){
                uxx buf[sizeof(uxx) * 8 / inc::bwidth + 1];

                the.pheight = 0;
                the.psize   = 0;

                do{
                    bits               = bits / inc::bwidth + (bits % inc::bwidth != 0);
                    buf[the.pheight]   = bits;
                    the.pheight       += 1;
                    the.psize         += bits;
                }while(bits > 1);

                the.bmp(
                    alloc(cost())
                );

                for(uxx i = 0; i < the.pheight; i++){
                    the.level_lut()[i] = buf[i];
                }

                inc::zeros(the.bmp(), the.size() * sizeof(uxx));
                the.pneed_free = false;
            }

            dbit_indicator_t(uxx bits) : 
                dbit_indicator_t(bits, [](uxx length){
                    return inc::alloc<uxx>(
                        inc::memory_size{
                            sizeof(uxx) * length
                        }
                    );
                }){
                the.pneed_free = true;
            }

        protected:
            ~dbit_indicator_t(){
                if (not the.pneed_free){
                    return;
                }

                auto bytes = cost_bytes();
                auto ptr   = inc::atom_swap<uxx *>(& pbmp, nullptr);

                if (ptr != nullptr){
                    inc::free(ptr - the.height(), bytes);
                }
            }

            xproget(height)
            xproget_priset(size)

            xproget_prisetx(bmp, uxx *)
                xr{ return the.pbmp; }
                xw{ the.pbmp = value + the.height(); }

            xprogetx(level_lut, uxx *){
                return the.pbmp - the.height();
            }

            xprigetx(cost, inc::memory_size){
                return inc::memory_size(the.psize + the.pheight);
            }

            xprigetx(cost_bytes, inc::memory_size){
                return inc::memory_size(cost() * sizeof(uxx));
            }

            xpubgetx(length, uxx){
                return the.level_lut()[0];
            }
        $
        
        template<class final>
        using dbit_indicator = inc::bit_indicator<final, dbit_indicator_t>;
    }

#endif

#undef  xusing_docker_dbit_indicator
#define xusing_docker_dbit_indicator     ::mixc::docker_dbit_indicator
