def counting_sort(arr, exp):
    size = len(arr)
    count_arr = [0] * 10 # 0-9 digits
    output_arr = [0] * size
    
    for i in range(size):
        index = arr[i] // exp
        count_arr[index % 10] += 1

    for i in range(1, 10):
        count_arr[i] += count_arr[i - 1]

    for i in range(size - 1, -1, -1):
        index = arr[i] // exp
        output_arr[ count_arr[index % 10] - 1 ] = arr[i]
        count_arr[index % 10] -= 1

    for i in range(size):
        arr[i] = output_arr[i]


def radix_sort(arr):
    max_value = max(arr)
    exp = 1

    while max_value // exp > 0:
        counting_sort(arr, exp)
        exp *= 10

arr = [5, 4, 3, 3, 0, 10, 42, 0, 1]
print(arr)
radix_sort(arr)
print(arr)

