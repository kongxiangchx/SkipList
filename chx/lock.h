#ifndef __CHX_LOCK_H__
#define __CHX_LOCK_H__

#include <mutex>
#include <atomic>
#include <condition_variable>

#define spinMaxCnt 10

namespace chx {

class SpinLock {
public:
    typedef std::lock_guard<SpinLock> SpinLockGuard;

    void lock();
    void unlock();

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

void SpinLock::lock() {
    uint32_t cnt = 0;
    while(flag.test_and_set(std::memory_order_acquire)) {
        cnt++;
        if(cnt >= spinMaxCnt) {
            cnt = 0;
            sleep(0);
        }
    }
}
void SpinLock::unlock() {
    flag.clear(std::memory_order_release);
}

}
#endif