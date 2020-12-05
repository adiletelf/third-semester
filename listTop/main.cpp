#include <iostream>
#include "list.h"

int main()
{
	int size = 10;
	List list(size, 0);
	std::cout << list << std::endl;

	return 0;
}