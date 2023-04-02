#include <iostream>
#include<thread>
#include<algorithm>
#include<numeric>
#include<vector>
#include<future>
#include<ctime>
#include<functional>
#include<mutex>
using namespace std;

std::mutex mtx;

struct func
{
    unsigned long long int& i;
    func(unsigned long long int& i_):i(i_){}
    void operator()(const int& a)
    {
        unique_lock<mutex> lck(mtx);
        i+=a;
    }
};

//std::async   实现 std::for_each
template<typename Iterator,typename Func>
void parallel_for_each(const Iterator& first,const Iterator& last,Func&& f)
{
    unsigned long const length=std::distance(first,last);

    if(!length) return;
    unsigned long const min_per_thread=25;

    if(length<(2*min_per_thread))
    {
        std::for_each(first,last,std::forward<Func>(f));
    }
    else
    {
        Iterator const mid_point=first+length/2;
        std::future<void> first_half=std::async(&parallel_for_each<Iterator,Func>,first,mid_point,std::forward<Func>(f));
        parallel_for_each(mid_point,last,std::forward<Func>(f));
        first_half.get();
    }
}

int main()
{
    srand(time(nullptr));
    constexpr int N=(int)1e5;
    vector<int> nums(N);
    for(int i=0;i<N;++i)
    {
        nums[i]=rand()%100000;
    }
    auto thread_begin=clock();
    unsigned long long int res=0;
    parallel_for_each(nums.begin(),nums.end(),func(res));
    cout<<res<<endl;
    auto thread_end=clock();
    cout<<(double)(thread_end-thread_begin)/CLOCKS_PER_SEC<<'\n';
    
    cout<<"----------------------------"<<endl;

    auto begin=clock();
    unsigned long long int nres=0;
    for_each(nums.begin(),nums.end(),func(nres));
    cout<<nres<<endl;
    auto end=clock();
    cout<<(double)(end-begin)/CLOCKS_PER_SEC<<'\n';
    system("pause");
    return 0;
}