#include<list>
#include<mutex>
#include<algorithm>
#include<iostream>
#include<thread>
using namespace std;

list<int> some_list;
mutex some_mutex;

void add_to_list(int new_value)
{
    lock_guard<mutex> guard(some_mutex);
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find)
{
    lock_guard<mutex> guard(some_mutex);
    return find(some_list.begin(),some_list.end(),value_to_find)!=some_list.end();
}

int main()
{
    thread t1,t2;
    while(1)
    {
        char cmd;
        int val;
        cin>>cmd>>val;
        if(cmd=='A')
        {
            add_to_list(val);
        }
        else
        {
            cout<<list_contains(val)<<endl;
        }

    }
    system("pause");
    return 0;
}