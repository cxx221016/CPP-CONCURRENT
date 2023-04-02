#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
#include<algorithm>
#include<functional>

class Mutex
{
    std::recursive_mutex mtx;
    std::string shared;
public:
    void f1()
    {
        std::lock_guard<std::recursive_mutex> lck(mtx);
        shared="data in f1";
        std::cout<<"f1: " << shared << std::endl;
    }

    void f2()
    {
        std::lock_guard<std::recursive_mutex> lck(mtx);
        shared="data in f2";
        std::cout<<"f2: " << shared << std::endl;
        f1();
        std::cout<<"f2: " << shared << std::endl;
    }
};

int main()
{
    Mutex x;
    std::vector<std::thread> threads(2);
    threads[0] = std::thread(&Mutex::f1, &x);
    threads[1] = std::thread(&Mutex::f2, &x);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    system("pause");
    return 0;
}