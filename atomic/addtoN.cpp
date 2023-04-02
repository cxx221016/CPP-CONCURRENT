#include<atomic>
#include<thread>
#include<mutex>
#include<ctime>
#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<functional>

using namespace std;

const int N=10000000;
const int threadsizes=10;

mutex mtx;

void update(int& hits)
{
    for(int i=0;i<N/threadsizes;++i)
    {
        lock_guard<mutex> guard(mtx);
        hits++;
    }
}

void update_atomic(atomic<int>& hits_atomic)
{
    for(int i=0;i<N/threadsizes;++i)
    {
        hits_atomic++;
    }
}

int main()
{
   static int hits=0;
   vector<thread> threads;

   auto thread_begin=clock();
   for(int i=0;i<threadsizes;++i)
   {
       threads.push_back(thread(update,ref(hits)));
   }
   for_each(threads.begin(),threads.end(),mem_fn(&thread::join));
   auto thread_end=clock();
   cout<<"hits: "<<hits<<'\n';
   cout<<(double)(thread_end-thread_begin)/CLOCKS_PER_SEC<<'\n';

   static atomic<int> hits_atomic=0;
   vector<thread> threads_atomic;

   auto thread_begin_atomic=clock();
   for(int i=0;i<threadsizes;++i)
   {
       threads_atomic.push_back(thread(update_atomic,ref(hits_atomic)));
   }
   for_each(threads_atomic.begin(),threads_atomic.end(),mem_fn(&thread::join));
   auto thread_end_atomic=clock();
   cout<<"hits_atomic: "<<hits_atomic<<'\n';
   cout<<(double)(thread_end_atomic-thread_begin_atomic)/CLOCKS_PER_SEC<<'\n';

   system("pause");
   return 0;
}