#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include <random>
#include <cmath>
#include <chrono>

enum class Color { RED, BLACK };

template<typename T>
struct node {
    T data;
    Color color;
    node* parent;
    node* left;
    node* right;

    node(const T& data, Color color = Color::RED, node* parent = nullptr, node* left = nullptr, node* right = nullptr)
        : data(data), color(color), parent(parent), left(left), right(right) {
    }
};

template<typename T>
class RedBlackTree {
private:
    node<T>* root;
    uint32_t size;
public:
    RedBlackTree() : root(nullptr), size(0) {}

    node<T>* getRoot() const { return root; }

    uint32_t getSize() const { return size; }

    void rotateLeft(node<T>* x) {
        node<T>* y = x->right;
        x->right = y->left;

        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rotateRight(node<T>* x) {
        node<T>* y = x->left;
        x->left = y->right;

        if (y->right != nullptr)
            y->right->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void fixInsertion(node<T>* z) {
        // Pętla trwa, dopóki wstawiony węzeł ma rodzica czerwonego
        while (z->parent != nullptr && z->parent->color == Color::RED) {
            // Jeśli rodzic węzła z jest lewym dzieckiem swojego rodzica
            if (z->parent == z->parent->parent->left) {
                node<T>* y = z->parent->parent->right;
                // Jeśli wujek jest czerwony, wykonuje zmianę kolorów 
                if (y != nullptr && y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateRight(z->parent->parent);
                }
            }
            else {
                node<T>* y = z->parent->parent->left;
                if (y != nullptr && y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
    }

    node<T>* search(node<T>* node, const T& data, std::function<bool(const T&, const T&)> comparator) const {
        if (node == nullptr || node->data == data)
            return node;
        if (comparator(data, node->data))
            return search(node->left, data, comparator);
        else
            return search(node->right, data, comparator);
    }

    int findHeight(node<T>* node) const {
        if (!node) return -1;
        return std::max(findHeight(node->left), findHeight(node->right)) + 1;
    }

    void traverseTree(node<T>* node, std::ostringstream& out) const {
        if (node) {
            out << "(" << node->data << ", "
                << (node->parent ? std::to_string(node->parent->data) : "NULL") << ", "
                << (node->color == Color::RED ? "RED" : "BLACK") << ") ";
            traverseTree(node->left, out);
            traverseTree(node->right, out);
        }
    }

    void insert(const T& data, std::function<bool(const T&, const T&)> comparator) {
        node<T>* newNode = new node<T>(data);
        if (root == nullptr) {
            root = newNode;
            root->color = Color::BLACK;
        }
        else {
            node<T>* current = root;
            node<T>* parent = nullptr;

            while (current != nullptr) {
                parent = current;
                if (comparator(data, current->data))
                    current = current->left;
                else
                    current = current->right;
            }

            newNode->parent = parent;
            if (comparator(data, parent->data))
                parent->left = newNode;
            else
                parent->right = newNode;

            fixInsertion(newNode);
        }
        size++;
    }

    node<T>* search(const T& data, std::function<bool(const T&, const T&)> comparator) const {
        return search(root, data, comparator);
    }

    std::string to_string(node<T>* node) const {
        std::ostringstream out;
        out << "Struktura drzewa czerwono-czarnego:\n(Dane, Rodzic, Kolor)\n";
        traverseTree(node, out);
        return out.str();
    }


    int getHeight() const {
        return findHeight(root);
    }
};

int main() {
    auto comparator = [](const int& a, const int& b) { return a < b; };
    std::random_device rd;
    std::mt19937 gen(rd());

    const int MAX_ORDER = 7;
    for (int o = 1; o <= MAX_ORDER; o++) {
        const int n = std::pow(10, o);
        RedBlackTree<int> bst;

        std::uniform_int_distribution<int64_t> dis(1, 10 * n);

        auto start_add = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++) {
            int64_t number = dis(gen);
            bst.insert(number, comparator);
        }
        auto end_add = std::chrono::high_resolution_clock::now();

        auto start_search = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++) {
            int64_t number = dis(gen);
            bst.search(number, comparator);
        }
        auto end_search = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration_add = end_add - start_add;
        std::chrono::duration<double> duration_search = end_search - start_search;

        int height = bst.getHeight();

        double log_n = std::log2(n);

        double average_add = duration_add.count() / n;
        double average_search = duration_search.count() / n;

        //  std::cout << bst.to_string(bst.getRoot()) << std::endl;
        std::cout << "Drzewo RBT z " << n << " elementami\n";
        std::cout << "Czas dodawania elementow: " << duration_add.count() * 1000 << " ms\n";
        std::cout << "Sredni czas dodawania: " << average_add * 1000 << " ms\n";
        std::cout << "Czas wyszukiwania elementow: " << duration_search.count() * 1000 << " ms\n";
        std::cout << "Sredni czas wyszukiwania: " << average_search * 1000 << " ms\n";
        std::cout << "Wysokosc drzewa: " << height << "\n\n";
    }

    return 0;
}
