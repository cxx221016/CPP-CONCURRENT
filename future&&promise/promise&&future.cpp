#include <thread>
#include <iostream>
#include <future>
#include<functional>
using namespace std;

/*
void add(int a, int b,promise<int>& p)
{
    p.set_value(a+b);
}

int main()
{
    promise<int> p;
    future<int> f=p.get_future();
    thread t(add,1,2,ref(p));

    f.wait();
    cout<<f.get()<<endl;
    //t.join();
    system("pause");
    return 0;
}
*/

void task(int a,future<int>& f,promise<int>& p)
{
    //f.wait();
    p.set_value(a+f.get());
}

int main()
{
    promise<int> p_ret;
    future<int> f_ret=p_ret.get_future();

    promise<int> p_in;
    future<int> f_in=p_in.get_future();

    thread t(task,1,ref(f_in),ref(p_ret));
    p_in.set_value(2);

    //f_ret.wait();
    cout<<f_ret.get()<<endl;
    //t.join();
    system("pause");
    return 0;
}