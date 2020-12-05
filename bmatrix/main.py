from matrix_class import Matrix

size = 7
matrix = Matrix(
    [
        [0 for _ in range(size)]
        for row in range(size)
    ]
)

matrix.rows[1][0] = 1
matrix.rows[3][4] = 1
matrix.rows[4][0] = 1
matrix.rows[4][2] = 1
matrix.rows[5][1] = 1
matrix.rows[5][2] = 1
matrix.rows[5][4] = 1
matrix.rows[6][0] = 1

print(matrix)

def topological_sort(rows):
    size = len(rows[0])
    columns = [col for col in range(size)]
    top_sorted = []

    while len(columns) != 0:
        empty_cols = []
        for col in columns:
            col_sum = sum( [rows[row][col] for row in range(size)] )
            if col_sum == 0:
                empty_cols.append(col)

        for col in empty_cols:
            rows[col] = [0] * size
            columns.remove(col)
            top_sorted.append(col)


    return [num + 1 for num in top_sorted]


result = topological_sort(matrix.rows)
print(result)
