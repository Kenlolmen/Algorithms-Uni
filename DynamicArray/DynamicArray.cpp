#include <iostream>
#include <string>
#include <ctime>
#include <random>

int DefaultSize = 8;
int Multiplier = 2;

template<typename T>
class DynamicArray {
private:
    T* elements;
    int capacity;
    int current_size;

public:
    DynamicArray() : capacity(DefaultSize), current_size(0) {
        elements = new T[capacity];
    }

    ~DynamicArray() {
        delete[] elements;
    }

    int GetCapacity() const { return capacity; }

    int GetCurrentSize() const { return current_size; }

    void Set(int index, T value) {
        if (index > current_size) {
            throw std::out_of_range("Index out of bounds");
        }
        elements[index] = value;
    }

    T Get(int index) const {
        if (index > current_size) {
            throw std::out_of_range("Index out of bounds");
        }
        return elements[index];
    }

    void Add(T value) {
        if (current_size >= capacity) {
            uint32_t new_capacity = capacity * Multiplier;
            T* new_elements = new T[new_capacity];

            for (uint32_t i = 0; i < current_size; i++) {
                new_elements[i] = elements[i];
            }

            delete[] elements;
            elements = new_elements;
            capacity = new_capacity;
        }
        elements[current_size++] = value;
    }

    void Delete() {
        delete[] elements;
        current_size = 0;
        elements = new T[capacity];
    }

    void BubbleSort() { // O( n^2 )
        for (uint32_t i = 0; i < current_size - 1; i++) {
            for (uint32_t j = 0; j < current_size - i - 1; j++) {
                if (elements[j] > elements[j + 1]) {
                    T temp = elements[j];
                    elements[j] = elements[j + 1];
                    elements[j + 1] = temp;
                }
            }
        }
    }

    std::string ToString(uint32_t number_to_print) const {
        if (number_to_print == 0 || current_size == 0) {
            return "[]";
        }
        else if (current_size < number_to_print) {
            throw std::out_of_range("Index out of bounds");
        }

        std::ostringstream output_stream;
        output_stream << "[" << elements[0];

        for (uint32_t i = 1; i < number_to_print; i++) {
            output_stream << ", " << elements[i];
        }
        output_stream << "]\n";

        return output_stream.str();
    }
};

int main() {
    std::default_random_engine generator(time(nullptr));
    std::uniform_int_distribution<int> distribution(-5, 5);

    DynamicArray<int>* dynamic_array = new DynamicArray<int>;
    int n = pow(10, 7);

    clock_t start_time = clock();
    double max_time_per_element = 0.0;

    for (int i = 0; i < n; i++) {
        clock_t element_start_time = clock();
        dynamic_array->Add(distribution(generator));
        clock_t element_end_time = clock();

        double time_per_element = (element_end_time - element_start_time) * 1000.0 / CLOCKS_PER_SEC;
        if (time_per_element > max_time_per_element) {
            max_time_per_element = time_per_element;
            std::cout << "Max time so far: " << max_time_per_element << " ms\n";
        }
    }
    clock_t end_time = clock();

    std::cout << "Array capacity: " << dynamic_array->GetCapacity() << "\n";
    std::cout << "Current size of array: " << dynamic_array->GetCurrentSize() << "\n";
    std::cout << "Total time of appending = " << ((end_time - start_time) * 1000.0 / CLOCKS_PER_SEC) << " ms\n";
    std::cout << "Average time of appending = " << ((end_time - start_time) * 1000.0 / CLOCKS_PER_SEC) / n << " ms\n";
    std::cout << "Worst time of appending = " << max_time_per_element << " ms\n";

    dynamic_array->Delete();
    delete dynamic_array;

    return 0;
}
