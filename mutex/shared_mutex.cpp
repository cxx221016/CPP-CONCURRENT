#include<iostream>
#include<thread>
#include<mutex>
#include<vector>
#include<algorithm>
#include<functional>
#include<shared_mutex>

std::shared_mutex mtx;
int data=0;

bool read()
{
    for(int i=0;i<10;++i)
    {
        std::shared_lock<std::shared_mutex> lck(mtx);
        //std::cout<<"read: " << data << std::endl;
        printf("read %d",std::this_thread::get_id());
        printf("read: %d\n", data);
    }
    return true;
}

bool write()
{
    for(int i=0;i<10;++i)
    {
        std::unique_lock<std::shared_mutex> lck(mtx);
        data++;
        //std::cout<<"write: " << data << std::endl;
        printf("write %d",std::this_thread::get_id());
        printf("write: %d\n", data);
    }
    return true;
}

int main()
{
    std::vector<std::thread> readthreads(2);
    std::vector<std::thread> writethreads(1);
    for(auto& writet:writethreads)
    {
        writet=std::thread(write);
    }
    for(auto& readt:readthreads)
    {
        readt=std::thread(read);
    }
    std::for_each(readthreads.begin(), readthreads.end(), std::mem_fn(&std::thread::join));
    std::for_each(writethreads.begin(), writethreads.end(), std::mem_fn(&std::thread::join));
    
    system("pause");
    return 0;
}
