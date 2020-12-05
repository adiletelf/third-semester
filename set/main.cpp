#include <iostream>
#include "set.h"

int main()
{
    Set set;
    set += '0';
    set += 'a';
    set += ' ';
    set += '\\';

    std::cout << set << std::endl;

    return 0;
}
