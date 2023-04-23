#include "Semaphore.h"
#include <vector>
const int M=5;
const int N=5;
using namespace std;
Semaphore mtx(1),AB(M-1),BA(N-1);

int countA=0,countB=0;

void A()
{
    while(true)
    {
        AB.wait();
        mtx.wait();
        //cout<<"A"<<endl;
        countA++;
        cout<<"countA= "<<countA<<" countB= "<<countB<<"sub= "<<countA-countB<<endl;
        mtx.signal();
        BA.signal();
    }
}

void B()
{
    while(true)
    {
        BA.wait();
        mtx.wait();
        //cout<<"B"<<endl;
        countB++;
        cout<<"countA= "<<countA<<" countB= "<<countB<<"sub= "<<countA-countB<<endl;
        mtx.signal();
        AB.signal();
    }
}

int main()
{
    int asum=2*M;
    int bsum=1;
    vector<thread> threadAs(asum);
    vector<thread> threadBs(bsum);
    for(int i=0;i<asum;i++)
    {
        threadAs[i]=thread(A);
    }
    for(int i=0;i<bsum;i++)
    {
        threadBs[i]=thread(B);
    }
    for(int i=0;i<asum;i++)
    {
        threadAs[i].join();
    }
    for(int i=0;i<bsum;i++)
    {
        threadBs[i].join();
    }
    return 0;
}