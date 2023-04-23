#include "Semaphore.h"
#include <vector>
using namespace std;
Semaphore mtx1(1),empty1(10),full1(0);
Semaphore mtx2(1),empty2(10),full2(0);

void Producer1()
{
    while(1)
    {
        empty1.wait();
        mtx1.wait();
        std::cout<<"produce A"<<std::endl;
        mtx1.signal();
        full1.signal();
    }
}

void Producer2()
{
    while(1)
    {
        empty2.wait();
        mtx2.wait();
        std::cout<<"produce B"<<std::endl;
        mtx2.signal();
        full2.signal();
    }
}

void Consumer()
{
    while(1)
    {
        full1.wait();
        mtx1.wait();
        std::cout<<"consume A"<<std::endl;
        mtx1.signal();
        empty1.signal();
        full2.wait();
        mtx2.wait();
        std::cout<<"consume B"<<std::endl;
        mtx2.signal();
        empty2.signal();
    }
}

int main()
{
    int producerNum=10;
    int consumerNum=10;
    vector<thread> threadPs(producerNum);
    vector<thread> threadCs(consumerNum);
    for(int i=0;i<producerNum;i++)
    {
        if(i%2==0) threadPs[i]=thread(Producer1);
        else threadPs[i]=thread(Producer2);
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
    system("pause");
    return 0;
}