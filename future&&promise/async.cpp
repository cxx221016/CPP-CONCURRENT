#include<iostream>
#include<future>
#include<thread>

using namespace std;


//#define NEWTHREAD

int task(int a, int b)
{
    printf("async thread: %d\n", this_thread::get_id());
    //cout<<"async thread: "<<this_thread::get_id()<<endl;
    return a + b;
}

int main()
{
    #ifdef NEWTHREAD
    future<int> f=async(task,1,2);
    printf("main thread: %d\n",this_thread::get_id());
    //cout<<"main thread: "<<this_thread::get_id()<<endl;
    cout<<f.get()<<endl;
    #else 
    future<int> f=async(std::launch::deferred,task,1,2);
    printf("main thread: %d\n",this_thread::get_id());
    //cout<<"main thread: "<<this_thread::get_id()<<endl;
    cout<<f.get()<<endl;
    #endif
    system("pause");
    return 0;
}


/*
#include <future>
#include <iostream>
#include <thread>
using namespace std;

int main()
{
   std::future<int> f = std::async(std::launch::async, []()
                                   {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 100; });

    std::future<int> g=std::async(std::launch::deferred,[](){
        return 10;
    });

   std::future_status status;
   do
   {
      status = f.wait_for(std::chrono::seconds(1));
      if (status == std::future_status::deferred)
      {
         std::cout << "deferred\n";
      }
      else if (status == std::future_status::timeout)
      {
         std::cout << "timeout\n";
      }
      else if (status == std::future_status::ready)
      {
         std::cout << "ready!\n";
      }
   } while (status != std::future_status::ready);
   std::cout << "result is " << f.get() << '\n';


   int g_res;
   do
   {
      status = g.wait_for(std::chrono::seconds(1));
      if (status == std::future_status::deferred)
      {
         std::cout << "deferred\n";
        g.wait();
      }
      else if (status == std::future_status::timeout)
      {
         std::cout << "timeout\n";
      }
      else if (status == std::future_status::ready)
      {
         std::cout << "ready!\n";
      }
   } while (status != std::future_status::ready);
   std::cout << "result is " << g.get() << '\n';
   
   system("pause");
   return 0;
}
*/