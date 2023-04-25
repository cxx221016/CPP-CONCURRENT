#include "Semaphore.h"
#include <vector>
using namespace std;
Semaphore s(1);
int cnt1=0,cnt2=0,cnt3=0;
Semaphore s1(1),s2(1),s3(1);

void show1()
{
    s1.wait();
    cnt1++;
    if(cnt1==1)
    {
        s.wait();
    }
    s1.signal();
    printf("show1 %d\n",std::this_thread::get_id());
    s1.wait();
    cnt1--;
    if(cnt1==0)
    {
        s.signal();
    }
    s1.signal();
}

void show2()
{
    s2.wait();
    cnt2++;
    if(cnt2==1)
    {
        s.wait();
    }
    s2.signal();
    printf("show2 %d\n",std::this_thread::get_id());
    s2.wait();
    cnt2--;
    if(cnt2==0)
    {
        s.signal();
    }
    s2.signal();
}

/*
void show3()
{
    s3.wait();
    cnt3++;
    if(cnt3==1)
    {
        s.wait();
    }
    s3.signal();
    printf("show3 %d",std::this_thread::get_id());
    s3.wait();
    cnt3--;
    if(cnt3==0)
    {
        s.signal();
    }
    s3.signal();
}
*/

#define show(x) \
void show##x()\
{\
    s##x.wait();\
    cnt##x++;\
    if(cnt##x==1)\
    {\
        s.wait();\
    }\
    s##x.signal();\
    printf("show%d\n",x,std::this_thread::get_id());\
    s##x.wait();\
    cnt##x--;\
    if(cnt##x==0)\
    {\
        s.signal();\
    }\
    s##x.signal();\
}

show(3)


int main()
{
    thread t1(show1);
    thread t2(show2);
    thread t3(show3);
    t1.join();
    t2.join();
    t3.join();
    system("pause");
    return 0;
}