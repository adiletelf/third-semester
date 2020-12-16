#include <iostream>
#include <vector>
#include <algorithm>
#include "bvector.h"
#include "bmatrix.h"
// #include "bvector.cpp"
// #include "bmatrix.cpp"


// not a smart way to do it
void topologicalSort(BM& matrix)
{
    int size = matrix.getRows();
    if (size < 0)
    {
        std::cout << "Incorrect input value" << std::endl;
        size = 0;
    }

    std::vector<int> top_sorted; // return value

    std::vector<int> columns;
    for (int i = 0; i < size; ++i)
        columns.push_back(i);

    while (!columns.empty())
    {
        std::vector<int> empty_cols;

        //for (int col = 0; col < size; ++col)
        for (auto& col : columns)
        {
            int col_sum = 0;
            for (int row = 0; row < size; ++row)
            {
                col_sum += (bool)matrix[row][col];
            }
            if (col_sum == 0)
                empty_cols.push_back(col);
        }

        // for (size_t col = 0; col < empty_cols.size(); ++col)
        for (auto& col : empty_cols)
        {
            matrix.reset(col);
            // columns.erase(columns.begin() + col);
            columns.erase(std::find(columns.begin(), columns.end(), col));
            top_sorted.push_back(col);
        }
    }

    for (size_t i = 0; i < top_sorted.size(); ++i)
    {
        top_sorted[i] += 1;
        std::cout << top_sorted[i] << ' ';
    }
    std::cout << std::endl;

}


// smart way (using boolean operations on boolean vectors)
void top_sort(BM& matrix)
{
    std::vector<int> top_sorted;

    int size = matrix.getRows();
    BV handled_cols(size, false); // 0000000

    while (handled_cols.getWeight() != size) // till we handle all columns
    {
        BV empty_cols = ~(matrix.disjunctionAll());
        empty_cols &= ~handled_cols; // ignore handled_cols

        if (empty_cols.getWeight() == 0)
        {
            std::cout << "Graph has a cycle" << std::endl;
            return;
        }

        for (int col = 0; col < empty_cols.getLen(); ++col)
        {
            bool value = (bool)empty_cols[col];
            if (value) // if empty_col
            {
                handled_cols.set(col);
                matrix.reset(col); // reset a row
                top_sorted.push_back(col);
            }
        }

    }

    for (size_t i = 0; i < top_sorted.size(); ++i)
    {
        top_sorted[i] += 1;
        std::cout << top_sorted[i] << ' ';
    }
    std::cout << std::endl;
}



int main()
{
    int size = 7;
    BM matrix(size, size, false);

    //matrix.set(1, 0);
    //matrix.set(3, 4);
    //matrix.set(4, 0);
    //matrix.set(4, 2);
    //matrix.set(5, 1);
    //matrix.set(5, 2);
    //matrix.set(5, 4);
    //matrix.set(6, 0);
    matrix[1][0] = true;
    matrix[3][4] = true;
    matrix[4][0] = true;
    matrix[4][2] = true;
    //matrix[2][3] = true;
    matrix[5][1] = true;
    matrix[5][2] = true;
    matrix[5][4] = true;
    matrix[6][0] = true;

    std::cout << matrix << std::endl;


    top_sort(matrix);
    // topologicalSort(matrix);

    return 0;
}
