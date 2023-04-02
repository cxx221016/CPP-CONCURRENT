#include <future>
#include <iostream>
bool is_prime(int x) {
	for (int i = 2; i < x; ++i) if (x % i == 0) return false;
	return true;
}
int main() {
	std::future<bool> fut = std::async(is_prime, 700020007);
	std::cout << "please wait";
    std::chrono::duration<int,std::ratio<1,1000>> span(3);
	while (fut.wait_for(span) != std::future_status::ready)
		std::cout << ".";

	std::cout << std::endl;
	bool ret = fut.get();
	std::cout << "final result: " << ret << std::endl;
    system("pause");
	return 0;
}
