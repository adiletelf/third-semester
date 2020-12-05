#include <iostream>
#include <chrono>
#include <fstream>
const int attempts = 3;

using namespace std;
using namespace chrono;

void swapInts(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}


void bitwiseSort(int* arr, int left, int right, int bit = 8 * sizeof(int) - 2)
{
    if (left >= right || bit < 0)
        return;
    int i = left, j = right;
    unsigned int mask = 1 << bit;
    while (i <= j)
    {
        while (i <= j && !(arr[i] & mask))
            ++i;
        while (i <= j && arr[j] & mask)
            --j;
        if (i < j)
            swapInts(arr[i++], arr[j--]);
    }
    bitwiseSort(arr, left, j, bit - 1);
    bitwiseSort(arr, i, right, bit - 1);
}

void sort(int* arr, int size)
{
    // Move all negative numbers to left,
    // Then use normal bitwiseSort
    int i = 0, j = size - 1;
    unsigned int mask = 1 << (8 * sizeof(int) - 1); // sign bit
    while (i <= j)
    {
        while (i <= j && arr[i] & mask) // if negative
            ++i;
        while (i <= j && !(arr[j] & mask))
            --j;
        if (i < j)
            swapInts(arr[i++], arr[j--]);
    }
    bitwiseSort(arr, 0, i - 1);
    bitwiseSort(arr, i, size - 1);

}



long long test(int* arr, int size)
{
    long long totalTime = 0;
    for (int attempt = 0; attempt < ::attempts; ++attempt)
    {
        int* newArr = new int[size];
        for (int i = 0; i < size; ++i)
            newArr[i] = arr[i];

        auto start = high_resolution_clock::now();
        sort(newArr, size);
        auto end = high_resolution_clock::now();
        auto time = duration_cast<milliseconds>(end - start).count();
        totalTime += time;

        delete[] newArr;
    }

    totalTime /= ::attempts;
    return totalTime;
}


int main(void)
{
    std::ifstream inf("arr6.txt");
    if (!inf)
        return 1;

    std::ofstream outf("bitwiseSortResult.txt");
    if (!outf)
        return 1;


    int size, range;
    outf << "Bitwise Sort";
    outf << "Amount of attempts: " << ::attempts << '\n';
    while (!inf.eof())
    {
        inf >> size;
        inf >> range;
        if (inf.eof())
            break;
        
        int* arr = new int [size];
        for (int i = 0; i < size; ++i)
        {
            inf >> arr[i];
        }

        long long totalTime = test(arr, size);

        outf << "Size = " << size << " Range = (" << -range << ") : (" << range << ")\n";
        outf << "Average time: " << totalTime << "(ms)\n\n";

        delete[]arr;
    }

    inf.close();
    outf.close();

    std::cout << "The program has finished." << std::endl;
    return 0;
}
