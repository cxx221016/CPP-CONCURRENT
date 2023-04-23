#include"Semaphore.h"
#include<vector>
#include<complex>
using namespace std;

complex<double> x,y,z;

Semaphore my(1),mz(1);

void f1()
{
    complex<double> w;
    my.wait();
    w=x+y;
    my.signal();
    std::cout<<"f1"<<w<<std::endl;
}

void f2()
{
    complex<double> w;
    mz.wait();
    my.wait();
    w=y+z;
    my.signal();
    mz.signal();
    std::cout<<"f2"<<w<<std::endl;
}

void f3()
{
    complex<double> w(1,1);
    mz.wait();
    z=z+w;
    mz.signal();
    my.wait();
    y=y+w;
    my.signal();
}

int main()
{
    thread t1(f1),t2(f2),t3(f3);
    t1.join();
    t2.join();
    t3.join();
    std::cout<<x<<std::endl;
    std::cout<<y<<std::endl;
    std::cout<<z<<std::endl;
    system("pause");
    return 0;
}


