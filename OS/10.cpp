#include "Semaphore.h"
#include <vector>
using namespace std;
int SN = 0, NS = 0;
Semaphore mutexSN(1), mutexNS(1);
Semaphore bridge(1);

void FNS()
{

    mutexNS.wait();
    if (NS > 0)
    {
        NS++;
        mutexNS.signal();
    }
    else
    {
        bridge.wait();
        NS++;
        mutexNS.signal();
    }
    //std::cout << std::this_thread::get_id() << " NS" << std::endl;
    printf("%d NS\n", std::this_thread::get_id());
    mutexNS.wait();
    NS--;
    if (NS == 0)
    {
        bridge.signal();
    }
    mutexNS.signal();
}

void FSN()
{

    mutexSN.wait();
    if (SN > 0)
    {
        SN++;
        mutexSN.signal();
    }
    else
    {
        bridge.wait();
        SN++;
        mutexSN.signal();
    }
    //std::cout << std::this_thread::get_id() << " SN" << std::endl;
    printf("%d SN\n", std::this_thread::get_id());
    mutexSN.wait();
    SN--;
    if (SN == 0)
    {
        bridge.signal();
    }
    mutexSN.signal();
}

int main()
{
    int NSNum = 5;
    int SNNum = 5;
    vector<thread> threadNSs(NSNum);
    vector<thread> threadSNs(SNNum);
    for (int i = 0; i < NSNum; i++)
    {
        threadNSs[i] = thread(FNS);
    }
    for (int i = 0; i < SNNum; i++)
    {
        threadSNs[i] = thread(FSN);
    }
    for (int i = 0; i < NSNum; i++)
    {
        threadNSs[i].join();
    }
    for (int i = 0; i < SNNum; i++)
    {
        threadSNs[i].join();
    }
    system("pause");
    return 0;
}