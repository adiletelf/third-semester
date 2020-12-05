#include <iostream>
#include <fstream>

using namespace std;


int* createArr(int size, int range)
{
    if (size < 0)
        size = 0;

    int* arr = new int [size];
    for (int i = 0; i < size; ++i)
    {
        arr[i] = rand() % range;
        if (rand() % 2)
            arr[i] *= -1;
    }
    return arr;
}



int main()
{
    std::ofstream outf("arr6.txt");
    if (!outf)
        return 1;


    int sizes[] = { 10000, 100000, 1000000 };
    int ranges[] = { 10, 1000, 10000 };

    for (auto& size : sizes)
    {
        for (auto& range : ranges)
        {
        outf << size << '\n';
        outf << range << '\n';

        int *arr = createArr(size, range);
        for (int i = 0; i < size; ++i)
            outf << arr[i] << '\n';
        delete []arr;

        
        }
    }
    
    outf.close();
    return 0;
}
