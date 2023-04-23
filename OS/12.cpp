#include "Semaphore.h"
#include <vector>
using namespace std;
int N=12;

Semaphore emptyt(N),wheel(0),frame(0);
Semaphore limitwheel(N-1),limitframe(N-2);

void wheelman()
{
    while(1)
    {
        limitwheel.wait();
        emptyt.wait();
        printf("wheelman %d\n",std::this_thread::get_id());
        wheel.signal();
    }
}


void frameman()
{
    while(1)
    {
        limitframe.wait();
        emptyt.wait();
        printf("frameman %d\n",std::this_thread::get_id());
        frame.signal();
    }
}

void carman()
{
    while(1)
    {
        frame.wait();
        emptyt.signal();
        limitframe.signal();

        wheel.wait();
        wheel.wait();
        emptyt.signal();
        emptyt.signal();
        limitwheel.signal();
        limitwheel.signal();
        printf("carman %d\n",std::this_thread::get_id());

    }
}

int main()
{
    thread t1(wheelman);
    thread t2(frameman);
    thread t3(carman);
    t1.join();
    t2.join();
    t3.join();
    system("pause");
    return 0;
}
