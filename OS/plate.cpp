#include <ctime>
#include <atomic>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
using namespace std;
 
class Semaphore
{
public:
    Semaphore(int value = 1) :count(value) {}
 
    void wait()
    {
        unique_lock<mutex> lck(mtk);
        if (--count < 0)//资源不足挂起线程
        {
            cv.wait(lck);
        }
          
    }
 
    void signal()
    {
        unique_lock<mutex> lck(mtk);
        if (++count <= 0)//有线程挂起，唤醒一个
        {
            cv.notify_one();
        }       
    }
 
private:
    int count;
    mutex mtk;
    condition_variable cv;
};
 
Semaphore plate(1), apple(0), orange(0);
void father()
{
    while (true)
    {
        plate.wait();
        cout << "往盘中放一个苹果" << endl;
        apple.signal();
    }
}
 
void mother()
{
    while (true)
    {
        plate.wait();
        cout << "往盘中放一个橘子" << endl;
        orange.signal();
    }
}
 
void son()
{
    while (true)
    {
        apple.wait();
        cout << "儿子吃苹果" << endl;
        plate.signal();
    }
}
 
void daughter()
{
    while (true)
    {
        orange.wait();
        cout << "女儿吃橘子" << endl;
        plate.signal();
    }
}
 
int main()
{
    thread f(father), m(mother), s(son), d(daughter);
    f.join();
    m.join();
    s.join();
    d.join();
    return 0;
}