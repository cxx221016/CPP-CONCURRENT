#include<iostream>
#include <thread>
#include<mutex>
#include<deque>
#include<chrono>
#include<condition_variable>
#include<vector>
#include<algorithm>
#include<functional>

std::mutex mtx;
std::deque<int> que;
std::condition_variable cv;

void producer()
{
    static int cnt=0;

    while(true)
    {
        std::unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, [](){return que.size()<100;});
        std::cout<<"producer: "<<std::this_thread::get_id()<<std::endl;
        que.push_back(cnt++);
        cv.notify_one();
    }
}

void consumer()
{
    while(true)
    {
        std::unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, [](){return !que.empty();});
        std::cout<<"consumer: "<<std::this_thread::get_id()<<std::endl;
        std::cout<<"front(): "<<que.front()<<std::endl;
        que.pop_front();
        std::cout<<"size: "<<que.size()<<std::endl;
        cv.notify_one();
    }
}

int main()
{
    std::vector<std::thread> producers(2);
    std::vector<std::thread> consumers(2);
    for(auto& p:producers)
    {
        p=std::thread(producer);
    }
    for(auto& c:consumers)
    {
        c=std::thread(consumer);
    }
    std::for_each(producers.begin(), producers.end(), std::mem_fn(&std::thread::join));
    std::for_each(consumers.begin(), consumers.end(), std::mem_fn(&std::thread::join));
    system("pause");
    return 0;
}