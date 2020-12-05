#include <iostream>
#include <fstream>
#include <chrono>
const int attempts = 3;

using namespace std;
using namespace chrono;

bool isAscending(int* arr, int size)
{
    for (int i = 0; i < size - 1; ++i)
        if (arr[i] > arr[i + 1])
            return false;

    return true;
}

int* createArr(int size, int range)
{
    int* arr = new int[size];
    for (int i = 0; i < size; ++i)
    {
        int num = rand() % range;
        if (rand() % 2)
            num *= -1;

        arr[i] = num;
    }

    return arr;
}

int* copyArr(int* arr, int size)
{
    int* newArr = new int[size];
    for (int i = 0; i < size; ++i)
        newArr[i] = arr[i];

    return newArr;
}

int power(int base, int exp)
{
    int num = 1;
    for (int i = 0; i < exp; ++i)
    {
        num *= base;
    }

    return num;
}


// --------------------------------------------------------------------------------------------


bool sort1(int* arr, int size)
{
    int step, i, j, temp;

    for (step = size / 2; step > 0; step /= 2)
    {
        for (i = step; i < size; ++i)
        {
            for (j = i - step; j >= 0 && arr[j] > arr[j + step]; j -= step)
            {
                temp = arr[j];
                arr[j] = arr[j + step];
                arr[j + step] = temp;
            }
        }
    }

    return true;
}


bool sort2(int* arr, int size)
{
    int step, i, j, temp;
    int ceil = 0;
    for (int num = 1; num <= size; num *= 2, ++ceil);
    --ceil;

    //int ceil = log2(size);
    int* steps = new int[ceil];
    for (int one = 1, index = ceil - 1; index >= 0; --index)
    {
        steps[index] = (one << ceil - index) - 1;
    }


    for (int index = 0; index < ceil; ++index)
    {
        step = steps[index];
        for (i = step; i < size; ++i)
        {
            for (j = i - step; j >= 0 && arr[j] > arr[j + step]; j -= step)
            {
                temp = arr[j];
                arr[j] = arr[j + step];
                arr[j + step] = temp;
            }
        }
    }

    delete[] steps;
    return true;
}


bool sort3(int* arr, int size)
{
    int step, i, j, temp;
    
    int ceil = 0;
    for (int num = 1; num <= size; num = 3 * num + 1, ++ceil);
    int* steps = new int[ceil];

    steps[ceil - 1] = 1;
    for (int i = ceil - 2; i >= 0; --i)
    {
        steps[i] = steps[i + 1] * 3 + 1;
    }

    for (int index = 0; index < ceil; ++index)
    {
        step = steps[index];
        for (i = step; i < size; ++i)
        {
            for (j = i - step; j >= 0 && arr[j] > arr[j + step]; j -= step)
            {
                temp = arr[j];
                arr[j] = arr[j + step];
                arr[j + step] = temp;
            }
        }
    }


    delete[] steps;
    return true;
}


int main()
{
    ofstream file;
    file.open("shellResult.txt");
    if (!file)
        return -1;

    
    int sizes[] = { 10000, 100000, 1000000 };
    int ranges[] = { 10, 10000, 100000 };

    for (auto& size : sizes)
    {
        for (auto& range : ranges)
        {
            int* arr = createArr(size, range);

            long long sTime1 = 0;
            long long sTime2 = 0;
            long long sTime3 = 0;

            for (int i = 0; i < ::attempts; ++i)
            {
                // Sort 1
                int* arr1 = copyArr(arr, size);
                auto start = high_resolution_clock::now();
                sort1(arr1, size);
                auto end = high_resolution_clock::now();

                sTime1 += duration_cast<milliseconds>(end - start).count();
                delete[] arr1;

                // Sort 2
                int* arr2 = copyArr(arr, size);
                auto start2 = high_resolution_clock::now();
                sort2(arr2, size);
                auto end2 = high_resolution_clock::now();

                sTime2 += duration_cast<milliseconds>(end2 - start2).count();
                delete[] arr2;

                // Sort 3
                int* arr3 = copyArr(arr, size);
                auto start3 = high_resolution_clock::now();
                sort3(arr3, size);
                auto end3 = high_resolution_clock::now();

                sTime3 += duration_cast<milliseconds>(end3 - start3).count();
                delete[] arr3;
            }

            sTime1 /= ::attempts;
            sTime2 /= ::attempts;
            sTime3 /= ::attempts;

            cout << "Size = " << size << ' ';
            cout << "Range (" << -range << " : " << range << ") Done.\n\n";

            file << "Size = " << size << '\n';
            file << "Range (" << -range << " : " << range << ")\n";

            file << "Sort(1) Time: " << sTime1 << "(ms)\n";
            file << "Sort(2) Time: " << sTime2 << "(ms)\n";
            file << "Sort(3) Time: " << sTime3 << "(ms)\n";
            file << "--------------------------------------------------------------------------------------------------------\n";
            
            delete[] arr;
        }
    }

    file.close();
    cout << "The program has finished.\n";
    return 0;   
}
