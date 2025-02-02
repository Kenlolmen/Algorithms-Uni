#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <chrono> 

template <typename T>
class BinaryHeap {
private:
    std::vector<T> heap;

    // Funkcja pomocnicza do naprawiania kopca w górę
    void heapifyUp(int index, std::function<bool(const T&, const T&)> comparator) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (comparator(heap[parent], heap[index])) {
                std::swap(heap[parent], heap[index]);
                index = parent;
            }
            else {
                break;
            }
        }
    }

    // Funkcja pomocnicza do naprawiania kopca w dół
    void heapifyDown(int index, std::function<bool(const T&, const T&)> comparator) {
        int size = heap.size();
        while (index < size) {
            int leftChild = 2 * index + 1;
            int rightChild = 2 * index + 2;
            int largest = index;

            if (leftChild < size && comparator(heap[largest], heap[leftChild])) {
                largest = leftChild;
            }
            if (rightChild < size && comparator(heap[largest], heap[rightChild])) {
                largest = rightChild;
            }

            if (largest != index) {
                std::swap(heap[index], heap[largest]);
                index = largest;
            }
            else {
                break;
            }
        }
    }

public:
    // Dodanie nowego elementu
    void add(const T& element, std::function<bool(const T&, const T&)> comparator) {
        heap.push_back(element);
        heapifyUp(heap.size() - 1, comparator);
    }

    // Pobranie i usunięcie elementu maksymalnego
    T removeMax(std::function<bool(const T&, const T&)> comparator) {
        if (heap.empty()) {
            throw std::out_of_range("Heap is empty");
        }
        T maxElement = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) {
            heapifyDown(0, comparator);
        }
        return maxElement;
    }

    // Czyszczenie kopca
    void clear() {
        heap.clear();
    }

    bool empty() const {
        return heap.empty();
    }
};

int main() {
    srand(time(NULL));
    auto comparator = [](const int& a, const int& b) { return a < b; };
    const int MAX_ORDER = 7;
    BinaryHeap<int>* bh = new BinaryHeap<int>();

    for (int o = 1; o <= MAX_ORDER; o++) {
        const int n = pow(10, o); // rozmiar danych
        std::cout << "Sample size = " << n << "\n";

        // Dodawanie elementów do kopca
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) {
            int so = rand() % 10000000 + 1;
            bh->add(so, comparator);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start; // Obliczanie czasu trwania
        std::cout << "Total time of inserting = " << duration.count() * 1000 << "ms\n";  // Przemnażamy przez 1000, aby uzyskać milisekundy
        std::cout << "Average time of inserting = " << (duration.count() * 1000) / n << "ms\n";

        // Pobieranie i usuwanie elementu maksymalnego
        start = std::chrono::high_resolution_clock::now();
        while (!bh->empty()) {
            bh->removeMax(comparator);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << "Total time of polling = " << duration.count() * 1000 << "ms\n";
        std::cout << "Average time of polling = " << (duration.count() * 1000) / n << "ms\n\n";

        bh->clear(); // czyszczenie kopca
    }

    delete bh;
    return 0;
}