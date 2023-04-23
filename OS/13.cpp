#include "Semaphore.h"
#include <vector>
using namespace std;
int N=12;

Semaphore emptyt(N),fullt(0);
Semaphore mtx(1);

void P()
{
    while(true)
    {
        emptyt.wait();
        mtx.wait();
        printf("P pro %d",std::this_thread::get_id());
        mtx.signal();
        fullt.signal();
    }

}

void Q()
{
    while(true)
    {
        fullt.wait();
        mtx.wait();
        printf("Q con %d",std::this_thread::get_id());
        mtx.signal();
        emptyt.signal();
    }
}

void R()
{
    while(true)
    {
        int i=rand()%2;
        if(i==0)
        {
            fullt.wait();
            mtx.wait();
            printf("R con %d",std::this_thread::get_id());
            mtx.signal();
            fullt.signal();
        }
        else
        {
            emptyt.wait();
            mtx.wait();
            printf("R pro %d",std::this_thread::get_id());
            mtx.signal();
            emptyt.signal();
        }
    }
}


int main()
{
    thread t1(P);
    thread t2(Q);
    thread t3(R);
    t1.join();
    t2.join();
    t3.join();
    system("pause");
    return 0;
}
