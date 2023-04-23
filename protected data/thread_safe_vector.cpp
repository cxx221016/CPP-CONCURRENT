#include<vector>
#include<thread>
#include<mutex>
#include<condition_variable>

template<typename T>
class threadsafe_vector
{
private:
    std::vector<T> data;
    mutable std::mutex mut;
    std::condition_variable data_cond;
public:
    threadsafe_vector(){}
    threadsafe_vector(threadsafe_vector const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data=other.data;
    }
    void push_back(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data.push_back(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop_back(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data.empty();});
        value=data.back();
        data.pop_back();
    }
    std::shared_ptr<T> wait_and_pop_back()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data.back()));
        data.pop_back();
        return res;
    }
    bool try_pop_back(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data.empty()) return false;
        value=data.back();
        data.pop_back();
        return true;
    }
    std::shared_ptr<T> try_pop_back()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data.back()));
        data.pop_back();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data.empty();
    }

    T& operator[](int i)
    {
        std::lock_guard<std::mutex> lk(mut);
        return data[i];
    }
};

int main()
{
    threadsafe_vector<int> v;
    
}