#include <iostream>
#include<thread>
#include<mutex>
#include <chrono>
#include<time.h>
#include<vector>
#include<queue>
#include<future>
#include <mutex>
#include <queue>
#include <functional>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <condition_variable>
#include<string>
#include<shared_mutex>

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
    void pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> pop()
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

class threadpool
{
private:
    friend class worker;
    class worker
    {
    private:
        threadpool* pool;
    public:
        worker(threadpool* pool=nullptr):pool(pool){}
        void operator()()
        {
            while(!pool->is_shut_down)
            {
                bool flag=false;
                std::function<void()> task;
                std::unique_lock<std::mutex> lk(pool->mtx);
                pool->cv.wait(lk,[this]{return this->pool->is_shut_down||!this->pool->work_queue.empty();});
                flag=pool->work_queue.try_pop(task);
                if(flag) task();
            }
        }
    };
    bool is_shut_down;
    threadsafe_queue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    std::mutex mtx;
    std::condition_variable cv;
public:
    threadpool(int n):threads(n),is_shut_down(false)
    {
        for(auto& th:threads) th=std::thread{worker(this)};
    }
    threadpool(const threadpool&)=delete;
    threadpool& operator=(const threadpool&)=delete;
    threadpool (threadpool&&)=delete;
    threadpool& operator=(threadpool&&)=delete;

    ~threadpool()
    {
        is_shut_down=true;
        cv.notify_all();
        for(auto& th:threads) 
        {
            if(th.joinable()) th.join();
        }
    }
    

    //C++17
    template<typename F,typename... Args,
    typename=std::enable_if_t<std::is_void_v<std::invoke_result_t<std::decay_t<F>,std::decay_t<Args>...>>>>
    std::future<bool> submit(F&& f,Args&&... args)
    {
        auto promise=std::make_shared<std::promise<bool>>();
        auto future=promise->get_future();
        std::function<void()> func=std::bind(std::forward<F>(f),std::forward<Args>(args)...);
        work_queue.push([func,promise]()
        {
            func();
            promise->set_value(true);
        });
        cv.notify_one();
        return future;
    }
    
    
    template<typename F,typename... Args,
    typename R=std::invoke_result_t<std::decay_t<F>,std::decay_t<Args>...>,
    typename=std::enable_if_t<!std::is_void_v<R>>>
    std::future<R> submit(F&& f,Args&&... args)
    {
        auto promise=std::make_shared<std::promise<R>>();
        auto future=promise->get_future();
        std::function<R()> func=std::bind(std::forward<F>(f),std::forward<Args>(args)...);
        work_queue.push([func,promise]()
        {
            promise->set_value(func());
        });
        cv.notify_one();
        return future;
    }

};

void change(int& a,int& b)
{
    a=4;
    b=5;
    //return a+b;
}


int main()
{
    threadpool pool(4);
    
    std::vector<std::future<int>> res;
    for(int i=0;i<10;i++)
    {
        res.push_back(pool.submit([i](){return i*i;}));
    }
    std::vector<std::future<bool>> res1;
    for(int i=0;i<10;i++)
    {
        res1.push_back(pool.submit([i](){printf("%d\n",i);}));
    }
    for(auto& r:res)
    {
        printf("%d\n",r.get());
    }
    for(auto& r:res1)
    {
        r.get();
    }
    /*
    std::future<int> res;

    int a=1,b=2;

    threadpool pool(4);
    res=pool.submit(change,std::ref(a),std::ref(b));   
    pool.~threadpool();

    printf("%d %d\n",a,b);
    printf("%d\n",res.get());
    
    std::future<bool> res;
    int a=1,b=2;

    threadpool pool(4);
    res=pool.submit(change,std::ref(a),std::ref(b));   
    pool.~threadpool();

    printf("%d %d\n",a,b);
    printf("%d\n",res.get());*/
    system("pause");
    return 0;
}