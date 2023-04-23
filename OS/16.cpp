#include "Semaphore.h"
#include <vector>
using namespace std;
Semaphore TN(1),NT(1);
Semaphore L(1),K(1);

void FTN()
{
    TN.wait();
    L.wait();
    printf("FTN L %d\n",std::this_thread::get_id());
    L.signal();
    K.wait();
    printf("FTN K %d\n",std::this_thread::get_id());
    K.signal();
    TN.signal();
}

void FNT()
{
    NT.wait();
    K.wait();
    printf("FNT K %d\n",std::this_thread::get_id());
    K.signal();
    L.wait();
    printf("FNT L %d\n",std::this_thread::get_id());
    L.signal();
    NT.signal();
}

int main()
{
    thread t1(FTN);
    thread t2(FNT);
    t1.join();
    t2.join();
    system("pause");
    return 0;
}