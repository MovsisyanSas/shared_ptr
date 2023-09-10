#include <iostream>
#include "sharedptr.h"

template<typename T>
sharedptr<T> makeshared(const T& obj) {
	return sharedptr<T>(new T(obj));
}



int main() {
	sharedptr<int> sp1 = makeshared<int>(5);
	sharedptr<int> sp2;
	sharedptr<int> sp3;

	std::cout << sp1.use_count() << std::endl;
	std::cout << sp2.use_count() << std::endl;
	
	sp2 = std::move(sp1);
	std::cout << sp2.use_count() << std::endl;
	
	sp3 = sp2;
	std::cout << sp2.use_count() << std::endl;
	
	sharedptr<int> sp4(sp3);
	std::cout << sp2.use_count() << std::endl;
}