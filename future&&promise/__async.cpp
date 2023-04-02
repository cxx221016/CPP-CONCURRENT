//使用async向函数传递参数
/*
#include<string>
#include<future>
#include<iostream>
struct X
{
    void foo(int x,std::string const& str)
    {
        std::cout<<"X "<<x<<" "<<str<<" "<<std::endl;
    }
    std::string bar(std::string const& str)
    {
        std::cout<<"X"<<str<<std::endl;
        return str;
    }
};
X x;
auto f1=std::async(&X::foo,&x,42,"hello");
auto f2=std::async(&X::bar,x,"goodbye");
struct Y
{
    double operator()(double d)
    {
        std::cout<<"Y "<<d<<std::endl;
        return d;
    }
};
Y y;
auto f3=std::async(Y(),3.141);
auto f4=std::async(std::ref(y),2.718);
//X baz(X&);
//std::async(baz,std::ref(x));
class move_only
{
public:
    move_only()=default;
    move_only(move_only&&)=default;
    move_only(move_only const &)=delete;
    move_only& operator=(move_only&&)=default;
    move_only& operator=(move_only const&)=delete;

    void operator()()
    {
        std::cout<<"f5"<<std::endl;
    }
};
auto f5=std::async(move_only());

int main()
{
   //f1.get();
   //f2.get();
   //f3.get();
   //f4.get();
   //f5.get();
   system("pause");
   return 0;
}*/

#include <thread>
#include <iostream>
#include <future>
using namespace std;


int mythread()
{
	cout << "mythread start ,id=" << this_thread::get_id() << endl;
	this_thread::sleep_for(3s);      //睡眠3s
	cout << "mythread end ,id=" << this_thread::get_id() << endl;
	return 5;
}

int main()
{
    #define ASYNC
    #ifdef ASYNC
    cout << "main start ,id=" << this_thread::get_id() << endl;
	future<int> result = async(launch::async, &mythread); 
	//创建异步任务并绑定future，
	//launch::async相当于创建异步线程
    cout << "result.get();" << endl;
	int res = result.get();      //get()会阻塞等待异步任务完成
	cout << res << endl;
    #endif

    //#define DEFERRED
    #ifdef DEFERRED
    cout << "main start ,id=" << this_thread::get_id() << endl;
	future<int> result = async(launch::deferred, mythread);
	//创建延时任务,会等待get()调用才会开始此任务
	//不创建线程，属于同步任务
	this_thread::sleep_for(100ms);
	cout << "result.get();" << endl;
	int res = result.get();      //get()开启延时任务
	cout << res << endl;
    #endif

    //#define ASYNE_OR_DEFERRED
    #ifdef ASYNE_OR_DEFERRED
    cout << "main start ,id=" << this_thread::get_id() << endl;
	future<int> result = async(launch::deferred | launch::async, mythread);
    future_status status=result.wait_for(std::chrono::seconds(0));
    if(status==future_status::ready)
    {
        cout<<"async Ready"<<endl;
        cout << result.get() << endl;
    }
    else if (status == future_status::deferred)
	{
		cout << "deferred Ready" << endl;
		cout << result.get() << endl;
	}
	else if (status == future_status::timeout)
	{
		cout << "Not End" << endl;
		cout << result.get() << endl;
	}
    #endif
    system("pause");
	return 0;
}

