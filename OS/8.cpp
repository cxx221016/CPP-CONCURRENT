#include "Semaphore.h"
#include <vector>
using namespace std;
Semaphore vat(1),emptyt(10),full(0);
Semaphore tong(3);
Semaphore well(1);

void Producer()
{
    while(1)
    {
        emptyt.wait();
        tong.wait();
        well.wait();
        well.signal();
        vat.wait();
        std::cout<<std::this_thread::get_id()<<" produce"<<std::endl;
        vat.signal();
        full.signal();
        tong.signal();
    }
}

void Consumer()
{
    while(1)
    {
        full.wait();
        tong.wait();
        vat.wait();
        std::cout<<std::this_thread::get_id()<<" consume"<<std::endl;
        vat.signal();
        emptyt.signal();
        tong.signal();
    }
}

int main()
{
    int producerNum=10;
    int consumerNum=1;
    vector<thread> threadPs(producerNum);
    vector<thread> threadCs(consumerNum);
    for(int i=0;i<producerNum;i++)
    {
        threadPs[i]=thread(Producer);
    }
    for(int i=0;i<consumerNum;i++)
    {
        threadCs[i]=thread(Consumer);
    }
    for(int i=0;i<producerNum;i++)
    {
        threadPs[i].join();
    }
    for(int i=0;i<consumerNum;i++)
    {
        threadCs[i].join();
    }
    return 0;
}