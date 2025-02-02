#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <chrono> 

struct some_object {
    double field_1;
    char field_2;
};

bool some_objects_cmp(const some_object& a, const some_object& b) {
    return a.field_1 < b.field_1;
}

double some_object_key_double(const some_object& obj) { // klucz sortowania
    return obj.field_1;
}

bool int_cmp(const int& a, const int& b) {
    return a > b;
}

template <typename T>
void bucket_sort(T* array, int n, double m, double (*key_func)(const T&), bool (*comparator)(const T&, const T&)) {
    int b = n;
    std::vector<std::vector<T>> buckets(b);

    for (int i = 0; i < n; ++i) {
        int idx = static_cast<int>(key_func(array[i]) * b / m);
        if (idx >= b) idx = b - 1; // Zapobieganie przekroczeniu zakresu
        buckets[idx].push_back(array[i]);
    }

    for (int i = 0; i < b; ++i) {
        std::sort(buckets[i].begin(), buckets[i].end(), comparator);
    }

    int idx = 0;
    for (int i = 0; i < b; ++i) {
        for (auto& obj : buckets[i]) {
            array[idx++] = obj; // kopiujemy element z kubelka do tablicy array
        }
    }
}

void bucket_sort_int(int* array, int n, int max_val) {
    int num_buckets = n;
    std::vector<std::vector<int>> buckets(num_buckets);

    for (int i = 0; i < n; ++i) {
        int idx = static_cast<int>(static_cast<double>(array[i]) * (num_buckets - 1) / max_val);
        if (idx == num_buckets) idx--;
        buckets[idx].push_back(array[i]);
    }

    for (int i = 0; i < num_buckets; ++i) {
        std::sort(buckets[i].begin(), buckets[i].end(), int_cmp);
    }

    int idx = 0;
    for (int i = 0; i < num_buckets; ++i) {
        for (int val : buckets[i]) {
            array[idx++] = val;
        }
    }
}



template <typename T>
void counting_sort(T* array, int n, double (*key_func)(const T&), double max_value) {
    int num_buckets = 100;
    std::vector<int> count(num_buckets, 0);
    std::vector<T> output(n);

    for (int i = 0; i < n; ++i) {
        int idx = static_cast<int>(key_func(array[i]) * num_buckets / max_value);
        if (idx == num_buckets) idx--;
        count[idx]++;
    }

    for (int i = 1; i < num_buckets; ++i) {
        count[i] += count[i - 1];
    }


    for (int i = n - 1; i >= 0; --i) {
        int idx = static_cast<int>(key_func(array[i]) * num_buckets / max_value);
        if (idx == num_buckets) idx--;
        output[count[idx] - 1] = array[i];
        count[idx]--;
    }

    // Kopiowanie posortowanych danych z powrotem 
    for (int i = 0; i < n; ++i) {
        array[i] = output[i];
    }
}

// klucz z obiektu int
double key_func_int(const int& val) {
    return static_cast<double>(val);
}

template <typename T>
class binary_heap {
public:
    T* array;
    int size;
    bool (*cmp)(const T&, const T&);

    binary_heap(T* arr, int n, bool (*comparator)(const T&, const T&), bool top_down = true) {
        array = arr;
        size = n;
        cmp = comparator;
        if (top_down) {
            for (int i = n / 2 - 1; i >= 0; --i) {
                heapify_top_down(i);
            }
        }
        else {
            for (int i = n - 1; i >= 0; --i) {
                heapify_bottom_up(i);
            }
        }
    }

    void heapify_top_down(int i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && cmp(array[left], array[largest])) largest = left;
        if (right < size && cmp(array[right], array[largest])) largest = right;

        if (largest != i) {
            std::swap(array[i], array[largest]);
            heapify_top_down(largest);
        }
    }

    void heapify_bottom_up(int i) {
        while (i > 0 && cmp(array[i], array[(i - 1) / 2])) {
            std::swap(array[i], array[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    void sort() {
        for (int i = size - 1; i >= 0; --i) {
            std::swap(array[0], array[i]);
            --size;
            heapify_top_down(0);
        }
    }
};


int main_objects() {
    const int MAX_ORDER = 7;
    const double m_double = 1.0; // Maksymalna wartoœæ klucza
    for (int o = 1; o <= MAX_ORDER; ++o) {
        const int n = pow(10, o);
        some_object* array1 = new some_object[n];
        for (int i = 0; i < n; ++i) {
            array1[i].field_1 = static_cast<double>(rand()) / RAND_MAX;
            array1[i].field_2 = 'a' + rand() % 26;
        }

        some_object* array2 = new some_object[n];
        for (int i = 0; i < n; ++i) {
            array2[i] = array1[i]; // Skopiuj dane
        }

        some_object* array3 = new some_object[n];
        for (int i = 0; i < n; ++i) {
            array3[i] = array1[i]; // Skopiuj dane
        }


        const int REPEATS = 5;

        // Pomiar czasu dla Heapsort
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < REPEATS; ++i) {
            binary_heap<some_object> bh(array1, n, some_objects_cmp);
            bh.sort();
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start; // Czas w sekundach
        std::cout << "Heapsort time for " << n << " elements: " << duration.count() / REPEATS << " seconds" << std::endl;

        // Pomiar czasu dla Bucket Sort
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < REPEATS; ++i) {
            bucket_sort(array2, n, m_double, some_object_key_double, some_objects_cmp);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "Bucket Sort time for " << n << " elements: " << duration.count() / REPEATS << " seconds" << std::endl << std::endl;
        delete[] array1;
        delete[] array2;
    }
    return 0;
}

void main_ints() {
    srand(0);
    const int MAX_ORDER = 7;
    const int m = 10000000;

    for (int o = 1; o <= MAX_ORDER; ++o) {
        int n = static_cast<int>(pow(10, o));
        int* array1 = new int[n];  //Counting Sort
        int* array2 = new int[n];  //Heap Sort
        int* array3 = new int[n];  //Bucket Sort


        for (int i = 0; i < n; ++i) {
            int rand_val = rand() % m;
            array1[i] = rand_val;
            array2[i] = rand_val;
            array3[i] = rand_val;
        }


        auto start = std::chrono::high_resolution_clock::now();
        counting_sort(array1, n, key_func_int, m);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "Counting Sort time for " << n << " elements: " << duration.count() << " seconds" << std::endl;


        start = std::chrono::high_resolution_clock::now();
        binary_heap<int>* bh = new binary_heap<int>(array2, n, int_cmp, true);
        bh->sort();
        delete bh;
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "Heap Sort time for " << n << " elements: " << duration.count() << " seconds" << std::endl;


        start = std::chrono::high_resolution_clock::now();
        bucket_sort_int(array3, n, m);
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "Bucket Sort time for " << n << " elements: " << duration.count() << " seconds" << std::endl << std::endl;

        delete[] array1;
        delete[] array2;
        delete[] array3;
    }
}

int main() {
    main_ints();
    //main_objects();
    return 0;
}
