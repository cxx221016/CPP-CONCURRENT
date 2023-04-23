#include "Semaphore.h"
#include <vector>
using namespace std;
int a=0,b=0,c=0;
int x=0,y=0,z=0;
Semaphore s1(1),s2(0),s3(0);

void P1()
{
    s1.wait();
    std::cin>>a;
    std::cout<<"P1:"<<a<<std::endl;
    s2.signal();
    s1.wait();
    x=a+b;
    std::cout<<"P1 "<<x<<","<<y<<","<<z<<std::endl;
}

void P2()
{
    s2.wait();
    std::cin>>b;
    std::cout<<"P2:"<<b<<std::endl;
    y=a*b;
    s3.signal();
}

void P3()
{
    s3.wait();
    std::cin>>c;
    std::cout<<"P3:"<<c<<std::endl;
    z=y+c-a;
    s1.signal();
}

int main()
{
    thread t1(P1),t2(P2),t3(P3);
    t1.join();
    t2.join();
    t3.join();
    //std::cout<<x<<","<<y<<","<<z<<std::endl;
    system("pause");
    return 0;
}