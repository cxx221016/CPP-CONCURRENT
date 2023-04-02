
#include <iostream>
#include <future>
#include <thread>
using namespace std;

mutex mtx;

void task(int a, shared_future<int>& b, promise<int>& p)
{
    p.set_value(2);
    b.wait();
    int bb = b.get();
    lock_guard<mutex> guard(mtx);
    cout << "task b =" << b.get()<<endl;
}
int main()
{
    // 把promise和future做关联
    promise<int> p_ret_1,p_ret_2;

    future<int> f1 = p_ret_1.get_future();
    future<int> f2 = p_ret_2.get_future();

    promise<int> p_in;
    shared_future<int> s_f = p_in.get_future();

    thread task1(task, 1, ref(s_f), ref(p_ret_1));
    thread task2(task, 2, ref(s_f), ref(p_ret_2));

    //do somesthing
    f1.wait();
    f2.wait();
    //...
    p_in.set_value(3);

    //get promise value
    {
        lock_guard<mutex> guard(mtx);
        cout << "main() return value is " << f1.get() << '\n';//只能get一次。
        cout << "main() return value is " << f2.get() << '\n';//只能get一次。
    }

    task1.join();
    task2.join();
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
   promise<int> prom;
   auto f1 = prom.get_future();
   auto f_shared = f1.share();

   std::future<int> f2 = std::async(std::launch::async, [&f_shared]()
                                    { return f_shared.get() + 1; });
   std::future<int> f3 = std::async(std::launch::async, [&f_shared]()
                                    { return f_shared.get() + 2; });

   prom.set_value(99);
   std::cout << "result1 is " << f2.get() << '\n';
   std::cout << "result2 is " << f3.get() << '\n';
   system("pause");
   return 0;
}
*/

