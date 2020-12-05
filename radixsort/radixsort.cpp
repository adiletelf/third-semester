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


int max(int* arr, int size)
{
    int max = arr[0];
    for (int i = 1; i < size; ++i)
        if (arr[i] > max)
            max = arr[i];
    return max;
}


void counting_sort(int* arr, int size, int exp)
{
    int* output_arr = new int [size]; 

    const int RADIX = 10;
    int* count_arr = new int[RADIX]; // 0-9 digits
    for (int i = 0; i < RADIX; ++i)
        count_arr[i] = 0;

    int index;
    for (int i = 0; i < size; ++i)
    {
        index = arr[i] / exp;
        count_arr[ (index % 10) ] += 1;
    }

    for (int i = 1; i < RADIX; ++i)
        count_arr[i] += count_arr[i - 1];

    for (int i = size - 1; i > -1; --i)
    {
        index = arr[i] / exp;
        output_arr[count_arr[ (index % 10) ] - 1] = arr[i];
        count_arr[ (index % 10) ] -= 1;
    }

    for (int i = 0; i < size; ++i)
        arr[i] = output_arr[i];

    delete []output_arr;
    delete []count_arr;
}


void radixsort(int* arr, int size)
{
    int maxValue = max(arr, size);
    int exp = 1;

    while ((maxValue / exp) > 0)
    {
        counting_sort(arr, size, exp);
        exp *= 10;
    }
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
        radixsort(newArr, size);
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

    std::ofstream outf("radixsortResult.txt");
    if (!outf)
        return 1;


    int size, range;
    outf << "Radix Sort\n";
    outf << "Amount of attempts: " << ::attempts << '\n';
    while (!inf.eof())
    {
        inf >> size;
        inf >> range;
        if (inf.eof())
            break;
        
        int* arr = new int[size];
        for (int i = 0; i < size; ++i)
        {
            inf >> arr[i];
            arr[i] += range; // Making positive numbers
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
