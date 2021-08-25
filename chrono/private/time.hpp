#ifndef xpack_chrono_private_time
#define xpack_chrono_private_time
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_time::inc
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"memop/seqlize.hpp"
#include"memop/cmp.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_private_time::origin{
    // 允许闰秒的存在
    constexpr uxx allow_leap_second = 60;

    /**
     * @brief 
     * 时间对象，时分秒毫秒
     * 小时可以大于等于 24 表示时间间隔
     * @note
     * 注意当 feild_t = u32 时，m_hour 的最大值为 1023，
     * 当小时表示时间间隔时，需要留意溢出问题
     * @tparam final_t time<> 的最终类型
     * @tparam field_t 构成时分秒毫秒的类型
     */
    template<class final_t, class field_t = u32>
    xstruct(
        xtmpl(time, final_t, field_t),
        xproc(m_milisecond, 10                      , field_t),  // 低位
        xproc(m_second    , 6                       , field_t),
        xproc(m_minute    , 6                       , field_t),
        xproc(m_hour      , sizeof(field_t) * 8 - 22, field_t)   // 高位，按比较的顺序
    )
        template<class, class> friend struct time;

        time(time const &) = default;

        /**
         * @brief 
         * 通过 chrono/now.hpp 初始化当前时间
         * @tparam now_t chrono/now.hpp 中的 now 符合该约束
         */
        template<class now_t>
        requires(now_t::im_now_t == true)
        time(now_t):
            time(now_t::time()){
        }

        /**
         * @brief
         * 通过与 time<> 相似的结构初始化该结构体
         * @tparam finalx_t value 的最终类型
         * @tparam fieldx_t value 时分秒毫秒字段的类型
         * @param value 当前结构的类似物
         */
        template<class finalx_t , class fieldx_t>
        time(time<finalx_t, fieldx_t> const & value) : 
            time(
                value.hour(),
                value.minute(),
                value.second(),
                value.milisecond()
            ){
        }

        /**
         * @brief
         * 通过时分秒毫秒初始化该结构
         * @param hour 时
         * @param minute 分
         * @param second 秒
         * @param milisecond 毫秒
         */
        time(field_t hour = 0, field_t minute = 0, field_t second = 0, field_t milisecond = 0) :
            m_milisecond(milisecond),
            m_second(second),
            m_minute(minute),
            m_hour(hour){
        }

        /**
         * @brief 
         * 是否时合法的分秒毫秒，不包含小时的判断
         * @param max_second 
         * 秒钟最大值，默认是 59 秒，允许 allow_leap_second 允许秒钟为 60 闰秒的情况
         * 但内部不会去判断什么时候是闰秒，只是简单的比较秒钟是否小于等于 max_second
         * @return true 合法时间
         * @return false 非法时间
         */
        bool is_valid(uxx max_second = 59) const {
            return m_minute <= 59 and m_second <= max_second and m_milisecond <= 999;
        }

        /**
         * @brief 
         * 是否时合法的 12 小时制时间，本结构在转换时使用 24 小时计数
         * @param max_second
         * 秒钟最大值，默认是 59 秒，允许 allow_leap_second 允许秒钟为 60 闰秒的情况
         * 但内部不会去判断什么时候是闰秒，只是简单的比较秒钟是否小于等于 max_second
         * @return true 合法时间
         * @return false 非法时间
         */
        bool is_valid_12h_clock(uxx max_second = 59) const {
            return m_hour != 0 and m_hour <= 12 and is_valid(max_second);
        }

        /**
         * @brief 
         * 是否时合法的 24 小时制时间，本结构在转换时使用 24 小时计数
         * @param max_second
         * 秒钟最大值，默认是 59 秒，允许 allow_leap_second 允许秒钟为 60 闰秒的情况
         * 但内部不会去判断什么时候是闰秒，只是简单的比较秒钟是否小于等于 max_second
         * @return true 合法时间
         * @return false 非法时间
         */
        bool is_valid_24h_clock(uxx max_second = 59) const {
            return m_hour <= 23 and is_valid(max_second);
        }

        /**
         * @brief 
         * 比较两个时间
         * @param value 比较数
         * @return ixx 
         * 当前日期 == value 返回 0
         * 当前日期 >  value 返回正数（不局限于 1）
         * 当前日期 <  value 返回负数（不局限于 -1）
         */
        ixx compare(the_t const & value) const {
            return inc::cmp_des(the, value); // 将结构转换成无符号整数序列，从高字往低字比较
        }

        /**
         * @brief 
         * 时间差
         * @return ixx 
         * 两个时间相差的毫秒数
         */
        friend ixx operator- (the_t const & left, the_t const & right){
            return (ixx)(left.total_milisecond() - right.total_milisecond());
        }

        #define xa_args_list       the_t const & left, the_t const & right
        #define xa_invoke          left.compare(right)
        #define xa_is_friend
        #include"macro/xgen.cmp.hpp"

        xpubget_pubset(milisecond)
        xpubget_pubset(second)
        xpubget_pubset(minute)
        xpubget_pubset(hour)

        xpubget_pubsetx(total_milisecond, u64)
            xr{
                auto ms = 
                    u64(m_milisecond) + 
                    u64(m_second) * (1000) + 
                    u64(m_minute) * (60 * 1000) + 
                    u64(m_hour)   * (60 * 60 * 1000);
                return ms;
            }
            xw{
                m_hour       = (value / (60 * 60 * 1000));
                value       %= (60 * 60 * 1000);
                m_minute     = (value / (60 * 1000));
                value       %= (60 * 1000);
                m_second     = (value / (1000));
                value       %= (1000);
                m_milisecond = (value);
            }
    $
}

#endif

#define xusing_chrono_time  ::mixc::chrono_private_time::origin
