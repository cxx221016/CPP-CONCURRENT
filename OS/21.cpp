#include "Semaphore.h"
#include <vector>

using namespace std;

Semaphore mtx(1), emptyt(1000),fullt(0);
Semaphore gett(1);

atomic<int> i = 1;
vector<int> buffer;

void produce()
{
    while(true)
    {
        emptyt.wait();
        mtx.wait();
        buffer.push_back(i++);
        mtx.signal();
        fullt.signal();
    }
}

void consume()
{
    while(true)
    {
        gett.wait();
        for(int i=0;i<10;++i)
        {
            fullt.wait();
            mtx.wait();
            printf("%d Consumer for %d\n", this_thread::get_id(), buffer.back());
            buffer.pop_back();
            mtx.signal();
            emptyt.signal();
        }
        gett.signal();
    }
}

int main()
{
    std::vector<std::thread> threadpros;
    std::vector<std::thread> threadcons;
    for (int i = 0; i < 1; i++)
    {
        threadpros.push_back(std::thread(produce));
    }
    for (int i = 0; i < 2; i++)
    {
        threadcons.push_back(std::thread(consume));
    }
    for (auto &t : threadpros)
    {
        t.join();
    }
    for (auto &t : threadcons)
    {
        t.join();
    }
    system("pause");
    return 0;
}
