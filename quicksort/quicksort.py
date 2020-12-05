def quicksort(arr, l, r, min_value, max_value):
    if l >= r or min_value >= max_value:
        return

    i = l
    j = r
    piv = (min_value + max_value) / 2  # float
    while i <= j:
        while i <= j and arr[i] < piv:
            i += 1

        while j >= i and arr[j] >= piv:
            j -= 1
            
        if i < j:
            arr[i], arr[j] = arr[j], arr[i]
            i += 1
            j -= 1

    quicksort(arr, l, j, min_value, piv - 0.5);
    quicksort(arr, i, r, piv + 0.5, max_value);


arr = [56, 1, -1, 0, 0, 5, 3, -5, 16, 8]
print(arr)
quicksort(arr, 0, len(arr) - 1, min(arr), max(arr))
print(arr)

