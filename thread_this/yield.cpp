#include <iostream>
#include<thread>
#include<algorithm>
#include<numeric>
#include<vector>
#include<atomic>
#include<mutex>
#include<chrono>
#include<functional>
#include<condition_variable>
#include<random>
using namespace std;

std::mutex mtx;
std::condition_variable cv;
bool ready = false;


void sum( int& i,int id) 
{
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, [](){return ready;});
    srand(time(nullptr));
    int jmax=rand()%10;
	for(int j=0;i<100&&j<jmax;++j) 
    {
		std::cout << "【" << id << "】→" <<++i << std::endl;
	}
	std::cout << "【线程" << id << "结束】"<<std::endl;
    cv.notify_one();
}

int main() {
    int i=0;
	std::vector<std::thread> threads(5);
	std::cout << "race of 5 threads that count to 100:\n";
	for (int id = 0; id < 5; ++id) threads[id] = std::thread(sum, std::ref(i),id);
	std::cout << "go......\n";
    ready=true;
    //cv.notify_all();
    cv.notify_one();
	auto start = std::chrono::high_resolution_clock::now();
	for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	auto end = std::chrono::high_resolution_clock::now();
	int elapse = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	std::cout << "\n执行用时:" << elapse / 1000 / 1000 << "秒" << elapse / 1000 << "毫秒\n";
	std::cout << "main()结束.";
    system("pause");
	return 0;
}