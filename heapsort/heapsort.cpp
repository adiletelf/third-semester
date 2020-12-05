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



void heapifyWithRecursion(int* arr, int size, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && arr[i] < arr[left])
        largest = left;


    if (right < size && arr[largest] < arr[right])
        largest = right;

    if (largest != i)
    {
        swapInts(arr[largest], arr[i]);
        heapifyWithRecursion(arr, size, largest);
    }            
}


void heapify(int* arr, int size, int i)
{
    int left, right, largest = i;
    do
    {
        left  = 2 * i + 1;
        right = 2 * i + 2;

        if (left < size && arr[i] < arr[left])
            largest = left;

        if (right < size && arr[largest] < arr[right])
            largest = right;

        if (largest != i)
        {
            swapInts(arr[largest], arr[i]);
            i = largest;
        }
        else
            return;

    } while (true);
}


void heapsort(int* arr, int size)
{
    for (int i = size / 2 - 1; i >= 0; --i)
        heapify(arr, size, i);

    for (int i = size - 1; i > 0; --i)
    {
        swapInts(arr[0], arr[i]);
        heapify(arr, i, 0);
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
        heapsort(newArr, size);
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

    std::ofstream outf("heapsort.txt");
    if (!outf)
        return 1;


    int size, range;
    outf << "Heap Sort\n";
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
