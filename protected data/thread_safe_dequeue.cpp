
#include <mutex>
#include <deque>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <functional>


#ifndef NDEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)//__VA_ARGS__:用于在宏替换部分中，表示可变参数列表
#else
#define DEBUG_PRINT(...) ((void)0)
#endif


using Callable = std::function<void()>;

template<typename T>
class SafeDequeue {
public:
    SafeDequeue() = default;
    ~SafeDequeue() = default;
    SafeDequeue(const SafeDequeue&) = delete;
    SafeDequeue& operator=(const SafeDequeue&) = delete;

private:
    using unique_lock = std::unique_lock<std::mutex>;

public:
    template<typename _Fun>
    void push_front(_Fun&& callback);//万能引用

    template<typename _Fun>
    void push_back(_Fun&& callback);//万能引用

    void pop_front(T& callback);
    void pop_back(T& callback);

    bool pop_front(T& callback, uint32_t timeout);
    bool pop_back(T& callback, uint32_t timeout);

    
	template<typename U, typename S= std::enable_if_t<std::is_convertible_v<U, T>>>
    void swap(SafeDequeue<U>& right);

    size_t size() const;
    bool empty() const;
    void clear();
private:
    std::deque<T> mDequeue;
    mutable std::mutex mMutex;//常成员函数不能改变类的成员变量的值，除非该成员变量为mutable
    std::condition_variable mCond;

	template<typename>
	friend class SafeDequeue;
};

template<typename T>
template<typename _Fun>
inline void SafeDequeue<T>::push_front(_Fun&& callback)
{
	unique_lock lock(mMutex);
	mDequeue.emplace_front(std::forward<_Fun>(callback));//原地构造，完美转发
	lock.unlock();

	DEBUG_PRINT("push_front: %d\n", callback);
	mCond.notify_one();
}

template<typename T>
template<typename _Fun>
inline void SafeDequeue<T>::push_back(_Fun&& callback)
{
	unique_lock lock(mMutex);
	mDequeue.emplace_back(std::forward<_Fun>(callback));//原地构造，完美转发
	lock.unlock();

	DEBUG_PRINT("push_back: %d\n", callback);
	mCond.notify_one();
}

template<typename T>
inline void SafeDequeue<T>::pop_front(T& callback)
{
	unique_lock lock(mMutex);
	mCond.wait(lock, [this] {
		return !mDequeue.empty();
		});

	callback = std::move(mDequeue.front());
	mDequeue.pop_front();
	DEBUG_PRINT("pop_front: %d\n", callback);
}

template<typename T>
inline bool SafeDequeue<T>::pop_front(T& callback, uint32_t timeout)
{
	unique_lock lock(mMutex);
	const bool result = mCond.wait_for(lock, std::chrono::milliseconds(timeout), [this] {
		return !mDequeue.empty();
		});

	if (result) {
		callback = std::move(mDequeue.front());
		mDequeue.pop_front();
	}
	DEBUG_PRINT("pop_front %d\n", callback);
	return result;
}

template<typename T>
inline void SafeDequeue<T>::pop_back(T& callback)
{
	unique_lock lock(mMutex);
	mCond.wait(lock, [this] {
		return !mDequeue.empty();
		});

	callback = std::move(mDequeue.back());
	DEBUG_PRINT("pop_back: %d\n", callback);
	mDequeue.pop_back();
}

template<typename T>
inline bool SafeDequeue<T>::pop_back(T& callback, uint32_t timeout)
{
	unique_lock lock(mMutex);
	const bool result = mCond.wait_for(lock, std::chrono::milliseconds(timeout), [this] {
		return !mDequeue.empty();
		});

	if (result) {
		callback = std::move(mDequeue.back());
		mDequeue.pop_back();
	}
	DEBUG_PRINT("pop_back: %d\n", callback);
	return result;
}


template<typename T>
template<typename U, typename S>
inline void SafeDequeue<T>::swap(SafeDequeue<U>& right)
{
	/*
	unique_lock lock1(mMutex, std::defer_lock);
	unique_lock lock2(right.mMutex, std::defer_lock);
	std::lock(lock1, lock2);
	*/
	std::lock(mMutex, right.mMutex);
	std::unique_lock<std::mutex> lock1(mMutex, std::adopt_lock);
	std::unique_lock<std::mutex> lock2(right.mMutex, std::adopt_lock);
	mDequeue.swap(right.mDequeue);
	DEBUG_PRINT("swap\n");
}

template<typename T>
inline size_t SafeDequeue<T>::size() const
{
	std::unique_lock<std::mutex> mlock(mMutex);//常成员函数不能改变类的成员变量的值，除非该成员变量为mutable
	DEBUG_PRINT("size\n");
	return mDequeue.size();
}

template<typename T>
inline bool SafeDequeue<T>::empty() const
{
	std::unique_lock<std::mutex> mlock(mMutex);
	DEBUG_PRINT("empty\n");
	return mDequeue.empty();
}

template<typename T>
inline void SafeDequeue<T>::clear()
{
	std::unique_lock<std::mutex> mlock(mMutex);
	DEBUG_PRINT("clear\n");
	return mDequeue.clear();
}

void pop_back(SafeDequeue<int>& safeDequeue, int& back)
{
	safeDequeue.pop_back(back);
	//printf("pop_back:%d\n", back);
}

void pop_front(SafeDequeue<int>& safeDequeue, int& front)
{
	safeDequeue.pop_front(front);
	//printf("pop_front:%d\n", front);
}

void push_back(SafeDequeue<int>& safeDequeue, int back)
{
	safeDequeue.push_back(back);
	//printf("push_back:%d\n", back);
}

void push_front(SafeDequeue<int>& safeDequeue, int front)
{
	safeDequeue.push_front(front);
	//printf("push_front:%d\n", front);
}



int main()
{
	int back, front;
	static int in=0;
	SafeDequeue<int> safeDequeue;

    /*
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; i++) {
		threads.push_back(std::thread(push_back, std::ref(safeDequeue), in++));
		//threads.push_back(std::thread(push_front, std::ref(safeDequeue), in++));
		threads.push_back(std::thread(pop_back, std::ref(safeDequeue), std::ref(back)));
		threads.push_back(std::thread(pop_front, std::ref(safeDequeue), std::ref(front)));
	}
	std::vector<std::thread> threads1;
    std::this_thread::sleep_for(std::chrono::seconds(5));
	printf("------------------\n");
	for(int i=0;i<10;i++)	threads1.push_back(std::thread(push_front, std::ref(safeDequeue), in++));
	for(auto& thread : threads)
		thread.join();
	for(auto& thread : threads1)
		thread.join();
    */
   	safeDequeue.push_back(1);
	SafeDequeue<int> safeDequeue1;
	for(int i=0;i<10;i++)	safeDequeue1.push_back(i);
    printf("safeDequeue.size:%d\n", safeDequeue.size());
	printf("safeDequeue1.size:%d\n", safeDequeue1.size());
	try
	{
		safeDequeue1.swap(safeDequeue);
	}
	catch(const std::exception& e)
	{
		printf("e.what:%s\n", e.what());
		system("pause");
	}
	
	printf("safeDequeue.size:%d\n", safeDequeue.size());
	printf("safeDequeue1.size:%d\n", safeDequeue1.size());
	system("pause");
	return 0;
}