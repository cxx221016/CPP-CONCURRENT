#include "Semaphore.h"
#include <vector>
using namespace std;
Semaphore mtx(1), emptyt(50);
atomic<int> i = 1;

void Consumer()
{
    emptyt.wait();
    mtx.wait();
    mtx.signal();
    int t = 1 + ((i++) % 3);
    printf("%d Consumer for %d\n", this_thread::get_id(), t);
    this_thread::sleep_for(chrono::seconds(t));
    mtx.wait();
    mtx.signal();
    emptyt.signal();
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 500; i++)
    {
        threads.push_back(std::thread(Consumer));
    }
    for (auto &t : threads)
    {
        t.join();
    }
    system("pause");
    return 0;
}
