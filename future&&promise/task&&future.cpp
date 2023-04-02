#include <thread>
#include <iostream>
#include <future>
#include<functional>
using namespace std;


int task(int a,int b)
{
    //f.wait();
    return a+b;
}

int main()
{
    packaged_task<int()> t(std::bind(task,11,12));
    t();
    future<int> result=t.get_future();

    cout<<result.get()<<endl;
    system("pause");
    return 0;
}