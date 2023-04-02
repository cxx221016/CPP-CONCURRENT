#include <iostream>
#include<thread>
#include<algorithm>
#include<numeric>
#include<vector>
#include<ctime>
#include<functional>
using namespace std;
/*
void func(std::thread::id& id)
{
    if(id==std::this_thread::get_id())
    {
        cout<<"same thread"<<endl;
    }
    else
    {
        cout<<"different thread"<<endl;
    }
}

int main()
{
    std::thread::id mainid=std::this_thread::get_id();
    std::thread t(func,std::ref(mainid));
    t.join();
    system("pause");
    return 0;
}
*/
/*
int main()
{
    srand(time(nullptr));
    constexpr int N=(int)1e8;
    vector<int> nums(N);
    for(int i=0;i<N;++i)
    {
        nums[i]=rand()%100000;
    }
    cout<<this_thread::get_id()<<endl;
    auto begin=clock();
    cout<<(unsigned long long)accumulate(nums.begin(),nums.end(),0ULL)<<'\n';
    auto end=clock();
    cout<<(double)(end-begin)/CLOCKS_PER_SEC;
    system("pause");
    return 0;

}
*/

void f()
{
    std::this_thread::sleep_for(std::chrono::duration<int>(1));
}

int main()
{
    std::vector<std::thread> threads(2);
    std::vector<std::thread::id> ids(2);
    for(int i=0;i<2;++i)
    {
        threads[i]=std::thread(f);
        ids[i]=threads[i].get_id();
    }

    std::cout<<"thread 1 id: "<<ids[0]<<'\n';
    std::cout<<"thread 2 id: "<<ids[1]<<'\n';
    std::cout<<"main thread id: "<<std::this_thread::get_id()<<'\n';
    std::cout<<"hardware concurrency: "<<std::thread::hardware_concurrency()<<'\n';
    system("pause");
    return 0;
}