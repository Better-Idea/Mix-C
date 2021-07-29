#ifndef xpack_concurrency_lock_group_barrier
#define xpack_concurrency_lock_group_barrier
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_group_barrier::inc
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/lock/atom_fetch_sub.hpp"
#include"concurrency/thread_self.hpp"
#include"macro/xexport.hpp"
#include"macro/xref.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_group_barrier::origin{
    constexpr uxx bits = sizeof(uxx) * 8;

    constexpr uxx index_of_last_set(uxx value){
        for(ixx i = bits; i-- > 0;){
            if (value & (uxx(1) << i)){
                return i;
            }
        }
        return not_exist;
    }

    // 同一组的操作可以同时进行
    // 不同组的操作不能同时进行

    template<uxx groups_v = 64>
    requires(groups_v != 0)
    xstruct(
       xtmpl(group_barrier, groups_v),

       // 当前锁属于的组
       xpric(m_group   , (index_of_last_set(groups_v) + 1), uxx),

       // 属于 m_group 组执行流一共获取了多少个锁
       xpric(m_owner   , (bits - (index_of_last_set(groups_v) + 1)) / 2, uxx),

       // m_active = m_owner + 其他尝试获取锁的执行流个数
       xpric(m_active  , (bits - (index_of_last_set(groups_v) + 1) + 1) / 2, uxx)
    )
    public:
        constexpr group_barrier(uxx group, uxx owner, uxx active):
            m_group (group),
            m_owner (owner),
            m_active(active){
        }

        constexpr group_barrier(uxx value = 0) : group_barrier(0, 0, 0)/*避免警告*/{
            *uxxp(this) = value;
        }

        template<auto operation_v>
        uxx try_lock() const {
            constexpr uxx group = uxx(operation_v);
            check<group>();
            the_t try_enter = { 0, 0, 1 };
            the_t try_enter_terminated = { 0, 1, 1 };
            the_t new_member = { 0, 1, 0 };
            the_t old = inc::atom_fetch_add(xref(*this), try_enter);
            uxx   offset = group + ~old.m_group + 1;
            the_t change_group{ offset, group >= old.m_group, 0 };

            if (old.m_active == 0) {
                inc::atom_fetch_add(xref(*this), change_group);
                return old.m_owner;
            }
            if (old.m_group != group or old.m_owner == 0) {
                inc::atom_fetch_sub(xref(*this), try_enter);
                return not_exist;
            }
            else{
                the_t test = inc::atom_fetch_add(xref(*this), new_member);
                return test.m_owner;
            }
        }

        template<auto operation_v>
        void unlock() const {
            constexpr uxx op = uxx(operation_v);
            check<op>();

            uxx sub_value = the_t{ 0, 1, 1};
            uxx & value = *this;
            inc::atom_fetch_sub(xref(*this), sub_value);
        }

        template<auto operation_v>
        uxx lock() const {
            while(true){
                if (uxx i_owner = try_lock<operation_v>(); i_owner != not_exist){
                    return i_owner;
                }
                inc::thread_self::yield();
            }
        }

        // 兼容 meta/is_builtin_lock 的接口
        template<auto operation_v, class this_t, class callback_t>
        void lock(this_t, callback_t const & call) const {
            lock<operation_v>(call);
        }

        template<auto operation_v, class callback_t>
        void lock(callback_t const & call) const {
            lock<operation_v>();
            call();
            unlock<operation_v>();
        }

    private:
        operator uxx &() const {
            return inc::cast<uxx>(*this);
        }

        template<uxx operation_v>
        static void check(){
            static_assert(operation_v != 0);
            static_assert(operation_v <= groups_v);
        }
    };
}

#endif

xexport_space(mixc::concurrency_lock_group_barrier::origin)