#include "Semaphore.h"
#include <vector>
#include<set>
#include<atomic>
using namespace std;
Semaphore mtx(1),emptyt(10),fullt(0),sever(0);
atomic<int> i=1;

void Consumer()
{
    while(true)
    {
        emptyt.wait();
        mtx.wait();
        mtx.signal();
        fullt.signal();
        sever.wait();
        cout<<i++<<endl;
    }
}

void Sever()
{
    while(true)
    {
        fullt.wait();
        emptyt.signal();
        sever.signal();
    }
}

int main()
{
    thread t1(Consumer);
    thread t2(Sever);
    t1.join();
    t2.join();
    system("pause");
    return 0;
}