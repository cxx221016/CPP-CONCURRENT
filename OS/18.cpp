#include "Semaphore.h"
#include <vector>
#include<set>
using namespace std;
Semaphore odd(0),even(0);
Semaphore emptyt(10);
Semaphore mtx(1);
int countodd=0,counteven=0;
set<int> buffer;

void produce()
{
    while(true)
    {
        emptyt.wait();
        mtx.wait();
        int tmp=rand();
        printf("produce %d\n",tmp);
        buffer.insert(tmp);
        mtx.signal();
        if(tmp%2==0)
            even.signal();
        else
            odd.signal();
    }
}

void getodd()
{
    while(true)
    {
        odd.wait();
        mtx.wait();
        countodd++;
        for(auto& i:buffer)
            if(i%2==1)
            {
                printf("get odd %d\n",i);
                buffer.erase(i);
                break;
            }
        mtx.signal();
        emptyt.signal();
    }
}

void geteven()
{
    while(true)
    {
        even.wait();
        mtx.wait();
        counteven++;
        for(auto& i:buffer)
            if(i%2==0)
            {
                printf("get even %d\n",i);
                buffer.erase(i);
                break;
            }
        mtx.signal();
        emptyt.signal();
    }
}

int main()
{
    thread t1(produce);
    thread t2(getodd);
    thread t3(geteven);
    t1.join();
    t2.join();
    t3.join();
    system("pause");
    return 0;
}

