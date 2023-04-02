#include <iostream>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include<vector>
#include<algorithm>
#include<numeric>
#include<functional>


std::map<std::string, std::string> memfiles;
std::mutex mtx;

#define CHANGE_IMPL(S) #S
#define CHANGE(S) CHANGE_IMPL(S)
#define PP_CONCAT(a, b) PP_CONCAT_IMPL(a, b)
#define PP_CONCAT_IMPL(a, b) a##b

bool set_value(const std::string& key, const std::string& value)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    std::lock_guard<std::mutex> lck(mtx);
    memfiles[key] = value;
    return true;
}

int main()
{
    std::vector<std::thread> threads(2);
    for (int i = 0; i < 2; ++i)
    {
        threads[i] = std::thread(set_value, CHANGE(PP_CONCAT(key_,__COUNTER__)), CHANGE(PP_CONCAT(value_,__COUNTER__)));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    for(auto& p : memfiles)
    {
        std::cout << p.first << " : " << p.second << std::endl;
    }
    system("pause");
    return 0;
}