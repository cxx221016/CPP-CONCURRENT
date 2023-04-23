#include <ctime>
#include <atomic>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
 
class Semaphore
{
public:
    Semaphore(int value = 1) :count(value) {}
 
    void wait()
    {
        std::unique_lock<std::mutex> lck(mtk);
        if (--count < 0)//资源不足挂起线程
        {
            cv.wait(lck);
        }
          
    }
 
    void signal()
    {
        std::unique_lock<std::mutex> lck(mtk);
        if (++count <= 0)//有线程挂起，唤醒一个
        {
            cv.notify_one();
        }       
    }
 
private:
    int count;
    std::mutex mtk;
    std::condition_variable cv;
};