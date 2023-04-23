#include "Semaphore.h"
#include <vector>
using namespace std;
int i = 0,j=0;
Semaphore si(1),sj(1);

void Consumer()
{
    si.wait();
    i++;
    si.signal();
}

void Seller()
{
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        sj.wait();
        if(j<i)
        {
            std::cout<<"sell "<<j<<std::endl;
            j++;
            sj.signal();
        }
        else 
        {
            std::cout<<"no goods"<<std::endl;
            sj.signal();
        }
    }
}

int main()
{
    int consumerNum=20;
    int sellerNum=10;
    vector<thread> threadCs(consumerNum);
    vector<thread> threadSs(sellerNum);
    for(int i=0;i<sellerNum;i++)
    {
        threadSs[i]=thread(Seller);
    }
    for(int i=0;i<consumerNum;i++)
    {
        if(i>=10) std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        threadCs[i]=thread(Consumer);
    }
    for(int i=0;i<consumerNum;i++)
    {
        threadCs[i].join();
    }
    for(int i=0;i<sellerNum;i++)
    {
        threadSs[i].join();
    }
    system("pause");
    return 0;
}