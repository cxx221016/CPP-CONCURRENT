#include "Semaphore.h"
#include <vector>

using namespace std;
int M=20,N=10;
int x=5,y=5;

Semaphore mtxA(1),fullA(x),emptyA(M-x);
Semaphore mtxB(1),fullB(y),emptyB(N-y);

void A()
{
    while(true)
    {
        fullA.wait();
        mtxA.wait();
        printf("get A\n");
        mtxA.signal();
        emptyA.signal();
        emptyB.wait();
        mtxB.wait();
        printf("put B\n");
        mtxB.signal();
        fullB.signal();
    }
}

void B()
{
    while(true)
    {
        fullB.wait();
        mtxB.wait();
        printf("get B\n");
        mtxB.signal();
        emptyB.signal();
        emptyA.wait();
        mtxA.wait();
        printf("put A\n");
        mtxA.signal();
        fullA.signal();
    }
}

int main()
{
    thread t1(A),t2(B);
    t1.join();
    t2.join();
    return 0;
}

