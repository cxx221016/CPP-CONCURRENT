#include "Semaphore.h"
#include <vector>

using namespace std;
Semaphore A(0),B(0),C(0),D(0);

void fa()
{
    printf("A\n");
    A.signal();
}

void fb()
{
    printf("B\n");
    B.signal();
}

void fc()
{
    A.wait();
    B.wait();
    printf("C\n");
    C.signal();
}

void fd()
{
    printf("D\n");
    D.signal();
}

void fe()
{
    C.wait();
    D.wait();
    printf("E\n");
}

int main()
{
    thread t1(fa),t2(fb),t3(fc),t4(fd),t5(fe);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    system("pause");
    return 0;
}

