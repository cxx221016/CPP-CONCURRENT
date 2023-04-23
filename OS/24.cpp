#include"Semaphore.h"
#include<vector>
#include<complex>
using namespace std;

int n=5;
int m=5;

vector<Semaphore> mtx(n);
Semaphore full(m);

void eat(int i )
{
    full.wait();
    mtx[i].wait();
    mtx[(i+1)%n].wait();
    printf("%d eat\n",i);
    mtx[i].signal();
    mtx[(i+1)%n].signal();
    full.signal();
}

int main()
{
    vector<thread> t;
    for(int i=0;i<n;i++)
    {
        t.push_back(thread(eat,i));
    }
    for(int i=0;i<n;i++)
    {
        t[i].join();
    }
    system("pause");
    return 0;
}


