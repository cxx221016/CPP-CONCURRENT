#include <iostream>
#include<thread>
#include<algorithm>
#include<numeric>
#include<vector>
#include<ctime>
#include<functional>
using namespace std;

template<typename Iterator,typename T>
struct accumulate_block
{
    void operator()(Iterator first,Iterator last,T& result)
    {
        result=accumulate(first,last,result);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
    unsigned long const length=distance(first,last);

    if(!length) return init;

    unsigned long const min_per_thread=25;
    unsigned long const max_threads=(length+min_per_thread-1)/min_per_thread;

    unsigned long const hardware_threads=thread::hardware_concurrency();
    unsigned long const num_threads=min(hardware_threads!=0 ? hardware_threads :2,max_threads);

    unsigned long const block_size=length/num_threads;

    vector<T> results(num_threads);
    vector<thread> threads(num_threads-1);

    Iterator block_start=first;
    for(unsigned long i=0;i<num_threads-1;++i)
    {
        Iterator block_end=block_start;
        advance(block_end,block_size);
        threads[i]=thread(accumulate_block<Iterator,T>(),block_start,block_end,ref(results[i]));
        block_start=block_end;
    }
    accumulate_block<Iterator,T>()(block_start,last,results[num_threads-1]);
    for_each(threads.begin(),threads.end(),mem_fn(&thread::join));
    return accumulate(results.begin(),results.end(),init);

}



int main()
{
    srand(time(nullptr));
    constexpr int N=(int)1e8;
    vector<int> nums(N);
    for(int i=0;i<N;++i)
    {
        nums[i]=rand()%100000;
    }
    auto thread_begin=clock();
    cout<<(unsigned long long)parallel_accumulate(nums.begin(),nums.end(),0ULL)<<'\n';
    auto thread_end=clock();
    cout<<(double)(thread_end-thread_begin)/CLOCKS_PER_SEC<<'\n';

    cout<<"----------------------------"<<endl;

    auto begin=clock();
    cout<<(unsigned long long)accumulate(nums.begin(),nums.end(),0ULL)<<'\n';
    auto end=clock();
    cout<<(double)(end-begin)/CLOCKS_PER_SEC;
    system("pause");
    return 0;

}
