#include<queue>
#include<memory>
#include<mutex>
#include<condition_variable>
#include<iostream>
#include<thread>
#include<functional>
#include<atomic>

template<typename T,typename Sequence=std::vector<T>,typename Compare=std::less<typename T::value_type>> 
class threadsafe_priority_queue
{
private:
    mutable std::mutex mut;
    std::priority_queue<T,Sequence,Compare> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_priority_queue(){}
    threadsafe_priority_queue(threadsafe_priority_queue const& other)
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
        value=data_queue.top();
        data_queue.pop();
    }
    std::shared_ptr<T> pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.top()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty()) return false;
        value=data_queue.top();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.top()));
        data_queue.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

int main()
{
    threadsafe_priority_queue<int,std::vector<int>,std::less<int>> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    while(!q.empty())
    {
        std::cout<<*(q.try_pop())<<std::endl;
    }
    system("pause");
    return 0;
}