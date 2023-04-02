#include<queue>
#include<memory>
#include<mutex>
#include<condition_variable>
#include<iostream>
#include<thread>
#include<functional>
#include<atomic>
#include"join_threads.h"

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue(){}
    threadsafe_queue(threadsafe_queue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue=other.data_queue;
    }
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty()) return false;
        value=data_queue.front();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

template<typename T>
using thread_safe_queue=threadsafe_queue<T>;

class thread_pool
{
    std::atomic_bool done;
    thread_safe_queue<std::function<void()> > work_queue; // 1
    std::vector<std::thread> threads; // 2
    join_threads joiner; // 3
    void worker_thread()
    {
        while(!done) // 4
        {
            std::function<void()> task;
            if(work_queue.try_pop(task)) // 5
            {
                task(); // 6
            }
            else
            {
                std::this_thread::yield(); // 7
            }
         }
    }
public:
        thread_pool():done(false),joiner(threads)
        {
            unsigned const thread_count=std::thread::hardware_concurrency(); // 8
            try
            {
                for(unsigned i=0;i<thread_count;++i)
                {
                    threads.push_back(
                    std::thread(&thread_pool::worker_thread,this)); // 9
                }
            }
            catch(...)
            {
                done=true; // 10
                throw;
            }
        }
        ~thread_pool()
        {
            done=true; // 11
        }
        template<typename FunctionType>
        void submit(FunctionType f)
        {
            work_queue.push(std::function<void()>(f)); // 12
        }
};



int main()
{
    thread_safe_queue<int> que;
    //thread_pool tpool;
    std::vector<std::thread> threadsin,threadsout;
    std::function<void(int)> func=[&](int x)
    {
        que.push(x);
        printf("push: %d\n",x);
    };

    std::function<void()> funout=[&]()
    {
        auto ptr=que.try_pop();
        if(ptr) printf("pop: %d\n",int(*ptr));
        else printf("pop failed\n");
    };

    for(int i=0;i<3;++i)
    {
        threadsin.push_back(std::thread(func,i));
        threadsout.push_back(std::thread(funout));
        //threadsin[i].join();
        //threadsout[i].join();
    }

    for(int i=0;i<3;++i)
    {
        threadsin[i].detach();
        threadsout[i].detach();
    }

    
    system("pause");
    return 0;

}



/*
int main()
{
    threadsafe_queue<int> que;
    while(true)
    {
        std::string cmd;
        std::cin>>cmd;
        if(cmd=="push")
        {
            int val;
            std::cin>>val;
            que.push(val);
        }
        else if(cmd=="try_pop")
        {
            std::shared_ptr<int> sp=que.try_pop();
            std::cout<<*sp<<std::endl;
        }
        else if(cmd=="wait_pop")
        {
            int val;
            que.wait_and_pop(val);
            std::cout<<val<<std::endl;
        }
        else if(cmd=="empty")
        {
            std::cout<<que.empty()<<std::endl;
        }
        else continue;

    }
}*/