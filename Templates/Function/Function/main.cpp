#include <iostream>

#include "Function.h"

void print_num(int i)
{
	std::cout << i << '\n';
}

struct Callable {
	~Callable() {
		std::cout << "Destruct";
	}
	void operator() () {
		std::cout << "call\n";
	}

	
	char data[1024];
};

int main()
{
	function<void(int)> f_display = print_num;
	f_display(1);

	function<void(int)> f_lam = [](int i) {std::cout << i << '\n'; };
	f_lam(2);

	{
		function<void()> f_str = Callable();
		f_str();
	}
	std::cin.get();
}