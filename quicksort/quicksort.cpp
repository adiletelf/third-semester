#include <iostream>
#include <fstream>
#include <chrono>
const int attempts = 3;


using namespace std;
using namespace chrono;


void swapInts(int& a, int& b)
{
  int temp = a;
  a = b;
  b = temp;
}

int min(int* arr, int size)
{
  int min = arr[0];
  for (int i = 1; i < size; ++i)
  {
    if (arr[i] < min)
      min = arr[i];
  }
  return min;
}

int max(int* arr, int size)
{
  int max = arr[0];
  for (int i = 1; i < size; ++i)
  {
    if (arr[i] > max)
      max = arr[i];
  }
  return max;
}


void printArr(int* arr, int size)
{
  for (int i = 0; i < size; ++i)
    std::cout << arr[i] << ' ';
  std::cout << std::endl;
}

int* createArr(int size)
{
  int* arr = new int[size];
  for (int i = 0; i < size; ++i)
  {
    arr[i] = rand() % size;
    if (rand() % 2)
      arr[i] *= -1;
  }
  return arr;
}


void quicksort(int* arr, int l, int r, double minValue, double maxValue)
{
  if (l >= r || minValue >= maxValue)
    return;

  int i = l, j = r;
  double piv = (minValue + maxValue) / 2;
  while (i <= j)
  {
    for (; i <= j && arr[i] < piv; ++i);
    for (; j >= i && arr[j] >= piv; --j);
    if (i < j)
    {
      swapInts(arr[i++], arr[j--]);
    }
  }

  quicksort(arr, l, j, minValue, piv - 0.5);
  quicksort(arr, i, r, piv + 0.5, maxValue);
}





long long test(int* arr, int size, int range)
{
  long long totalTime = 0;
  for (int attempt = 0; attempt < ::attempts; ++attempt)
  {
    int* newArr = new int[size];
    for (int i = 0; i < size; ++i)
        newArr[i] = arr[i];

    auto start = high_resolution_clock::now();
    quicksort(newArr, 0, size - 1, -range, range);
    auto end = high_resolution_clock::now();
    auto time = duration_cast<milliseconds>(end - start).count();
    totalTime += time;

    delete[] newArr;
  }

  totalTime /= ::attempts;
  return totalTime;
}



int main()
{
  //srand(time(0));
  std::ifstream inf("arr6.txt");
  if (!inf)
    return 1;
  std::ofstream outf("quicksortResult.txt");
  if (!outf)
    return 1;


  int size, range;
  outf << "Quick Sort\n";
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
      }

      long long totalTime = test(arr, size, range);

      outf << "Size = " << size << " Range = (" << -range << ") : (" << range << ")\n";
      outf << "Average time: " << totalTime << "(ms)\n\n";
  }

  return 0;
}

