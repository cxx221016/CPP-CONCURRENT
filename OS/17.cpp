#include "Semaphore.h"
#include <vector>
using namespace std;
Semaphore go(0),open(0);

void driver()
{
    while(true)
    {
        go.wait();
        printf("driver %d\n",std::this_thread::get_id());
        open.signal();
    }
}

void passenger()
{
    while(true)
    {
        go.signal();
        printf("passenger %d\n",std::this_thread::get_id());
        open.wait();
    }
}

int main()
{
    thread t1(driver);
    thread t2(passenger);
    t1.join();
    t2.join();
    system("pause");
    return 0;
}