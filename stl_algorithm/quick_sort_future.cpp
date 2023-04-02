#include <iostream>
#include<thread>
#include<algorithm>
#include<numeric>
#include<vector>
#include<list>
#include<ctime>
#include<functional>
#include<future>
using namespace std;

//快速排序--顺序实现版
template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
    if(input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(),input,input.begin());
    T const& pivot=*result.begin();
    auto divide_point=std::partition(input.begin(),input.end(),[&](T const& t){return t<pivot;});
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(),input,input.begin(),divide_point);
    auto new_lower(sequential_quick_sort(std::move(lower_part)));
    auto new_higher(sequential_quick_sort(std::move(input)));
    result.splice(result.end(),new_higher);
    result.splice(result.begin(),new_lower);
    return result;
}

//快速排序--期望并行版
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)
{
    if(input.empty())
    {
        return input;
    }
    std::list<T> result;
    result.splice(result.begin(),input,input.begin());
    T const& pivot=*result.begin();
    auto divide_point=std::partition(input.begin(),input.end(),[&](T const& t){return t<pivot;});
    std::list<T> lower_part;
    lower_part.splice(lower_part.end(),input,input.begin(),divide_point);
    std::future<std::list<T>> new_lower(std::async(&parallel_quick_sort<T>,std::move(lower_part)));
    auto new_higher(parallel_quick_sort<T>(std::move(input)));
    result.splice(result.end(),new_higher);
    result.splice(result.begin(),new_lower.get());
    return result;
}

int main()
{
    srand(time(nullptr));
    constexpr int N=(int)100;
    list<int> nums(N);
    for(auto& num:nums) num=rand()%1000;
    auto thread_begin=clock();
    auto res=parallel_quick_sort(nums);
    auto thread_end=clock();
    cout<<(double)(thread_end-thread_begin)/CLOCKS_PER_SEC<<'\n';

    cout<<"----------------------------"<<endl;

    auto begin=clock();
    sequential_quick_sort(nums);
    auto end=clock();
    cout<<(double)(end-begin)/CLOCKS_PER_SEC;
    system("pause");
    return 0;

}

