#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
#include <cmath>

template<typename T,
         typename Compare = std::less<T> >
class BatcherSort
{
public:
    void operator()(std::vector<T>& arr, size_t l, size_t r);

private:
    void shuffle(std::vector<T>& arr, size_t l, size_t r);
    void unshuffle(std::vector<T>& arr, size_t l, size_t r);

    void merge(std::vector<T>& arr, size_t l, size_t r);
    bool compexch(T& a, T& b);  // if exchnge, then  return true, else false

private:
    Compare comp;
};

template<typename T, typename Compare>
void BatcherSort<T, Compare>::operator()(std::vector<T>& arr, size_t l, size_t r)
{
    merge(arr, l, r);
}

template<typename T, typename Compare>
void BatcherSort<T, Compare>::shuffle(std::vector<T>& arr, size_t l, size_t r)
{
    size_t m = std::ceil((l + r)/2);
    static std::vector<T> aux(arr.size());

    for (size_t i = l, j = 0; i < r; i += 2, ++j)
    {
        aux[i] = arr[l + j];
        aux[i + 1] = arr[m + 1 + j];
    }

    for (size_t i = l; i <= r; ++i)
    {
        arr[i] = aux[i];
    }
}

template<typename T, typename Compare>
void BatcherSort<T, Compare>::unshuffle(std::vector<T>& arr, size_t l, size_t r)
{
    size_t m = std::ceil((l + r)/2);
    static std::vector<T> aux(arr.size());

    for (size_t i = l, j = 0; i < r; i += 2, ++j)
    {
        aux[l + j] = arr[i];
        aux[m + 1 + j] = arr[i + 1];
    }

    for (size_t i = l; i <= r; ++i)
    {
        arr[i] = aux[i];
    }
}

template<typename T, typename Compare>
void BatcherSort<T, Compare>::merge(std::vector<T>& arr, size_t l, size_t r)
{
    if (r == l + 1)
    {
        compexch(arr[l], arr[r]);
    }

    if (r < l + 2)
    {
        return;
    }

    size_t m = std::ceil((l + r) / 2);

    unshuffle(arr, l, r);
    merge(arr, l, m);
    merge(arr, m + 1, r);
    shuffle(arr, l, r);

    for (size_t i = l + 1; i <= r; ++i)
    {
        compexch(arr[i - 1], arr[i]);
    }
    /*
    for (size_t i = l + 1; i <= r; i += 2)
    {
        if (!compexch(arr[i - 1], arr[i]) && ++i <= r)
        {
            // not exchange
            if (!compexch(arr[i - 1], arr[i]))
            {
                // not exchange too
                --i;    // because step == 2
            }
        }
    }
    */
}

template<typename T, typename Compare>
bool BatcherSort<T, Compare>::compexch(T& a, T& b)
{
    if (!comp(a, b))
    {
        std::swap(a, b);

        return true;
    }

    return false;
}



int main(int argc, char *argv[])
{
    BatcherSort<double> sorter;
    int size = 16;

    std::vector<double> forSort;

    std::cout << "Array:\n";
    for (int i = 0; i < size; ++i)
    {
        forSort.push_back(rand()%1000);
        std::cout << forSort[i] << '\t';
    }

    sorter(forSort, 0, size - 1);

    std::cout << "\nSorted array:\n";
    for (int i = 0; i < size; ++i)
    {
        std::cout << forSort[i] << '\t';
    }

    return 0;
}
