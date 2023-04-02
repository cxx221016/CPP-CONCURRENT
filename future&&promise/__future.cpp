
#include <iostream>
#include <future>
#include <chrono>
#include<functional>

using F=std::function<int(int,int,int&)>;

int add(int x,int y,int &z)
{
    z=x+y+230;
    return z;
}

void Thread_1(std::promise<F>& p)
{
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout<<"Thread_1 Ready"<<std::endl;
    p.set_value(std::bind(&add,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}

template<typename F,typename ...Args>
void Thread_2(std::future<F>& f,Args&& ...args)
{
    auto fun=f.get();
    auto res=fun(std::forward<Args>(args)...);
    std::cout<<"get() ready "<<res<<std::endl;
}

int main()
{
    #define PACKAGED_TASK
    #ifdef PROMISE
    std::promise<F> p;
    std::future<F> f=p.get_future();
    int rval=0;
    std::thread thread1(Thread_1,std::ref(p));
    std::thread thread2(Thread_2<F,int,int,int&>,std::ref(f),1,2,std::ref(rval));
    thread1.join();
    thread2.join();
    std::cout<<rval<<std::endl;
    #endif

    #ifdef PACKAGED_TASK
    std::packaged_task<int(int,int,int&)> p(add);
    std::future<int> f=p.get_future();
    int rval=0;
    std::thread thread1(std::move(p),1,2,std::ref(rval));
    int res=f.get();
    std::cout<<"res=   "<<res<<std::endl;
    #endif

    #ifdef ASYNC
    std::promise<F> p;
    std::future<F> f=p.get_future();
    int rval=0;
    async(Thread_1,std::ref(p));
    async(Thread_2<F,int,int,int&>,std::ref(f),1,2,std::ref(rval));
    std::cout<<rval<<std::endl;
    #endif
    system("pause");
    return 0;
}


