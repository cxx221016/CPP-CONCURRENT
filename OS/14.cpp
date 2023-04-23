#include "Semaphore.h"
#include <vector>
using namespace std;
int N=3;
int god=0;
Semaphore mutexgod(1);
Semaphore customers=0;
Semaphore barbers=0;
void barber()
{
    while(true)
    {
        customers.wait();
        mutexgod.wait();
        god--;
        barbers.signal();
        mutexgod.signal();
        printf("barber %d\n",std::this_thread::get_id());
    }
}

void customer()
{
    mutexgod.wait();
    if(god<N)
    {
        god++;
        customers.signal();
        mutexgod.signal();
        barbers.wait();
        printf("customer %d\n",std::this_thread::get_id());
    }
    else
    {
        mutexgod.signal();
        printf("customer %d\n",std::this_thread::get_id());
    }
}

int main()
{
    int costumers=10;
    thread t1(barber);
    vector<thread> threadcos(costumers);
    for(int i=0;i<costumers;i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        threadcos[i]=thread(customer);
    }
    for(int i=0;i<costumers;i++)
    {
        threadcos[i].join();
    }
    t1.join();
    system("pause");
    return 0;
}