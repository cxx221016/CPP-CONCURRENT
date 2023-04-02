#include<exception>
#include<memory>
#include<mutex>
#include<stack>
#include<iostream>

struct  empty_stack: std::exception
{
    const char* what() const throw()
    {
        return "empty stack!";
    };
};

template<typename T>
class threadsafe_stack
{
private:
    std::stack<T>data;
    mutable std::mutex m;
public:
    threadsafe_stack():data(std::stack<int>()){}
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data=other.data;
    }
    threadsafe_stack& operator=(const  threadsafe_stack&)=delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack {};

        std:: shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
    }
    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack {};
        value=data.top();
        data.pop();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

int main()
{
    threadsafe_stack<int> stk;
    while(true)
    {
        std::string cmd;
        std::cin>>cmd;
        if(cmd=="push")
        {
            int val;
            std::cin>>val;
            stk.push(val);
        }
        else if(cmd=="pop()")
        {
            std::shared_ptr<int> sp=stk.pop();
            std::cout<<*sp<<std::endl;
        }
        else if(cmd=="pop")
        {
            int val;
            stk.pop(val);
            std::cout<<val<<std::endl;
        }
        else if(cmd=="empty")
        {
            std::cout<<stk.empty()<<std::endl;
        }
        else continue;

    }
}