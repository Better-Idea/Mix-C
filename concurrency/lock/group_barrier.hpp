#ifndef xpack_concurrency_lock_group_barrier
#define xpack_concurrency_lock_group_barrier
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::concurrency_lock_group_barrier::inc
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_fetch_sub.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"concurrency/thread_self.hpp"
#include"macro/xexport.hpp"
#include"macro/xref.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::concurrency_lock_group_barrier{
    // 不使用原子操作初始化
    constexpr bool atom_free    = true;
    constexpr uxx  bits         = sizeof(uxx) * 8;

    constexpr uxx index_of_last_set(uxx value){
        for(ixx i = bits; i-- > 0;){
            if (value & (uxx(1) << i)){
                return i;
            }
        }
        return not_exist;
    }
}

namespace mixc::concurrency_lock_group_barrier::origin{
    /**
     * @brief 
     * 分组屏障
     * 同一时刻只有一个组可以操作
     * @note
     * 别名：三相计数器屏障
     * @tparam total_group_v 
     * 一共有多少个组
     */
    template<uxx total_group_v = 64>
    requires(total_group_v != 0)
    xstruct(
        xtmpl(group_barrier, total_group_v),

        // 当前锁属于的组
        xpric(m_group   , (index_of_last_set(total_group_v) + 1), uxx),

        // 属于 m_group 组执行流一共获取了多少个锁
        // 当 m_group 还没有更替时，减少同一个组连任的概率
        // 因为上一个获取到锁的组在 m_active != 0 且 m_owner == 0 时会放弃一轮竞选
        xpric(m_owner   , (bits - (index_of_last_set(total_group_v) + 1)) / 2, uxx),

        // m_active = m_owner + 其他尝试获取锁的执行流个数
        xpric(m_active  , (bits - (index_of_last_set(total_group_v) + 1) + 1) / 2, uxx)
    )
        /**
         * @brief
         * 不带原子操作的初始化
         * @param group 组 id
         * @param owner 当前组所有者个数
         * @param active 当前组所有者个数 + 其他尝试获取锁的组成员个数
         */
        group_barrier(uxx group, uxx owner, uxx active, bool):
            m_group (group),
            m_owner (owner),
            m_active(active){
        }
    public:
        /**
         * @brief
         * 分别初始化
         * @param group 组 id
         * @param owner 当前组所有者个数
         * @param active 当前组所有者个数 + 其他尝试获取锁的组成员个数
         */
        group_barrier(uxx group = 0, uxx owner = 0, uxx active = 0):
            group_barrier(group, owner, active, atom_free){

            // 避免多核 cache 缓存了该段内存（来自堆内存）
            inc::atom_store(xref(*this), *this);
        }

        /**
         * @brief 
         * 尝试获取锁
         * @tparam group_v 尝试获取锁的组 id
         * @return uxx 
         * 如果获取锁成功，返回当前线程是第几个获取到锁的(从 0 开始计数)
         * 否则返回 not_exist
         */
        template<auto group_v>
        uxx try_lock() const {
            constexpr uxx group = uxx(group_v);
            check<group>();

            // 尝试获取锁的线程先让 m_active + 1
            the_t try_enter = { 0, 0, 1, atom_free };

            // 当已经确认是同一组的成员后，可以让 m_owner + 1
            the_t new_member = { 0, 1, 0, atom_free };

            // 先增加 this->m_active 计数器，维持状态
            the_t old = inc::atom_fetch_add(xref(*this), try_enter);

            // 让 this->m_group 替换成 group，以加法的方式实现
            // old.m_group + offset -> group
            uxx   offset = group - uxx(old.m_group);

            // 我们需要使用 *this += change_group 实现
            // - m_group 变成新的 group
            // - 让 m_owner + 1
            // 当 group <  old.m_group 时，*this 只要加 offset 就可以实现上面的逻辑
            // 当 group >= old.m_group 时，old.m_group + offset 是没有进位的，需要补 1
            // 所以有 group >= old.m_group 作为第二参数
            the_t change_group{ offset, group >= old.m_group, 0, atom_free };

            // 如果当前线程是第一个获取到锁的
            // 就让它做更替组的操作
            if (old.m_active == 0) {
                inc::atom_fetch_add(xref(*this), change_group);
                return old.m_owner;
            }

            // 如果 group 不属于当前组，或者该组无使用者了就放弃还原操作，放弃本轮获取锁的操作
            // 当 old.m_owner = 0 时其实还可以继续组竞选
            // 这里为了实现简单且避免同一个组总是抢占使用权，就让它放弃本轮操作
            if (old.m_group != group or old.m_owner == 0) {
                inc::atom_fetch_sub(xref(*this), try_enter);
                return not_exist;
            }
            // 是当前组的成员，就让 m_owner + 1
            else{
                the_t test = inc::atom_fetch_add(xref(*this), new_member);
                return test.m_owner;
            }
        }

        /**
         * @brief 
         * 归还一个锁资源
         * @note 
         * 只有释放锁的操作次数和获取到锁的次数一样时，才允许更替锁所有者
         * 当锁还有使用者时，是不允许更替锁所有者的组的
         * @tparam group_v 锁所有者的组
         */
        template<auto group_v>
        void unlock() const {
            constexpr uxx op = uxx(group_v);
            check<op>();

            // 同时减去 m_owner 和 m_active
            // m_active 是起到防卫的作用，它保证了更替组的原子性
            // 只要 m_active 不为 0，组就不允许被更替，
            uxx sub_value = the_t{ 0, 1, 1, atom_free };
            uxx & value = *this;
            inc::atom_fetch_sub(xref(*this), sub_value);
        }

        /**
         * @brief 
         * 尝试获取锁，如果尝试失败就会停留在该函数中
         * @tparam group_v 尝试获取锁的组
         * @return uxx 
         * 如果获取锁成功，返回当前线程是第几个获取到锁的(从 0 开始计数)
         * 否则返回 not_exist
         */
        template<auto group_v>
        uxx lock() const {
            while(true){
                if (uxx i_owner = try_lock<group_v>(); i_owner != not_exist){
                    return i_owner;
                }
                inc::thread_self::yield();
            }
        }

        // 兼容 meta/is_builtin_lock 的接口
        template<auto group_v, class this_t, class callback_t>
        void lock(this_t, callback_t const & call) const {
            lock<group_v>(call);
        }

        /**
         * @brief 
         * 带锁的闭包，只有获取到锁后才会执行 call
         * @tparam group_v 尝试获取锁的组
         * @tparam callback_t 回调函数类型
         */
        template<auto group_v, class callback_t>
        void lock(callback_t const & call) const {
            lock<group_v>();
            call();
            unlock<group_v>();
        }

    private:
        operator uxx &() const {
            return *uxxp(this);
        }

        template<uxx group_v>
        static void check(){
            static_assert(group_v != 0);        // 0 表示组尚未初始化
            static_assert(group_v <= total_group_v); // 不能超过总组数
        }
    };
}

#endif

xexport_space(mixc::concurrency_lock_group_barrier::origin)