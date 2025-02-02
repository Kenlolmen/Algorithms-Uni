#include <iostream>
#include <string>
#include <random>
#include <functional>
#include <chrono>

template<typename T>
struct node
{
    T data;
    node* parent;
    node* left;
    node* right;

    node(const T& data, node* parent = nullptr, node* left = nullptr, node* right = nullptr)
        : data(data), parent(parent), left(left), right(right)
    {
    }

    ~node()
    {
        if (parent != nullptr)
        {
            parent = nullptr;
        }
        if (left != nullptr)
        {
            delete left;
            left = nullptr;
        }
        if (right != nullptr)
        {
            delete right;
            right = nullptr;
        }
    }
};

template<typename T>
class BST
{
private:
    uint32_t size;
    node<T>* root;

public:
    BST() noexcept : root(nullptr), size(0) {}


    uint32_t get_size() const noexcept
    {
        return size;
    }

    node<T>* getRoot() const noexcept
    {
        return root;
    }

    void Insert(T new_data, std::function<bool(const T&, const T&)> comparator)
    {
        if (root == nullptr) //czy puste
        {
            root = new node<T>(new_data);
        }
        else
        {
            node<T>* new_node = new node<T>(new_data);
            node<T>* current_node = root;
            node<T>* previous_node = nullptr;

            while (current_node != nullptr)
            {
                previous_node = current_node;
                if (comparator(new_data, current_node->data)) // - true
                    current_node = current_node->left; // idzie w lewo
                else
                    current_node = current_node->right; // else idzie w prawo
            }

            new_node->parent = previous_node; // nowy wezel
            if (comparator(new_data, previous_node->data))
                previous_node->left = new_node;
            else
                previous_node->right = new_node;
        }
        size++;
    }

    node<T>* Search(const T& search_data, std::function<bool(const T&, const T&)> comparator)
    {
        node<T>* current_node = root;
        while (current_node != nullptr)
        {
            if (current_node->data == search_data)
            {
                return current_node;
            }
            else if (comparator(search_data, current_node->data))
            {
                current_node = current_node->left;
            }
            else
            {
                current_node = current_node->right;
            }
        }
        return nullptr;
    }

    node<T>* findMin(node<T>* root)
    {
        while (root && root->left != nullptr)
        {
            root = root->left;
        }
        return root;
    }

    void RemoveNode(node<T>* target)
    {
        if (target == nullptr) return;

        if (target->left == nullptr && target->right == nullptr) // nie ma dzieci
        {
            if (target->parent != nullptr) //jesli ma rodzica
            {
                if (target->parent->left == target)
                    target->parent->left = nullptr;
                else
                    target->parent->right = nullptr;
            }
            else
            {
                root = nullptr;
            }
            delete target;
        }
        else if (target->left == nullptr || target->right == nullptr) // jedno dzieko
        {
            node<T>* child = (target->left != nullptr) ? target->left : target->right;

            if (target->parent != nullptr)
            {
                if (target->parent->left == target)
                    target->parent->left = child;
                else
                    target->parent->right = child;
            }
            else
            {
                root = child;
            }

            child->parent = target->parent;
            delete target;
        }
        else // dwojka dzieci
        {
            node<T>* successor = findMin(target->right); //nastepnik (najmniejszy element w prawym poddrzewie)
            target->data = successor->data; // Przypisujemy dane następcy do węzła
            RemoveNode(successor);
        }
    }

    void inOrderTraversal(node<T>* node, std::ostream& out) const { // najmniejszy srodkowy wiekszy
        if (node) {
            inOrderTraversal(node->left, out);
            out << node->data << " ";
            inOrderTraversal(node->right, out);
        }
    }

    void preOrderTraversal(node<T>* node, std::ostream& out) const { // srodkowy mniejszy wiekszy
        if (node) {
            out << node->data << " ";
            preOrderTraversal(node->left, out);
            preOrderTraversal(node->right, out);
        }
    }


    int findHeight(node<T>* node) const {
        if (!node) {
            return -1;
        }
        return std::max(findHeight(node->left), findHeight(node->right)) + 1;
    }

    std::string getLevel(node<T>* root, int level)
    {
        std::ostringstream output_stream;
        if (root == nullptr)
        {
            return "( )";
        }
        if (level == 1)
        {
            if (root->parent != nullptr)
            {
                output_stream << "(" << root->data << "," << root->parent->data << ")";
            }
            else
            {
                output_stream << "(" << root->data << "," << "NULL" << ")";
            }
        }
        else if (level > 1)
        {
            output_stream << getLevel(root->left, level - 1);
            output_stream << getLevel(root->right, level - 1);
        }
        return output_stream.str();
    }

    std::string to_string1(node<T>* root)
    {
        std::ostringstream output_stream;
        output_stream << "Struktura BST \nNotacja: (dane,dane rodzica)\n";
        int height = findHeight(root);
        if (height > 5)
        {
            output_stream << "(Pokazuje 5 poziomow )\n";
            height = 5;
        }
        for (int i = 1; i <= height; i++)
        {
            output_stream << "Level " << i << ": ";
            output_stream << getLevel(root, i);
            output_stream << "\n";
        }
        return output_stream.str();
    }

    void Clear(node<T>* node)
    {
        if (node == nullptr)
            return;

        Clear(node->left);
        Clear(node->right);

        delete node;
    }

    void ClearTree()
    {
        Clear(root);
        root = nullptr;
    }

    void inOrder(std::ostream& out) const {
        inOrderTraversal(root, out);
    }

    void preOrder(std::ostream& out) const {
        preOrderTraversal(root, out);
    }
};

int main()
{
    auto comparator = [](const int& a, const int& b) { return a < b; };
    // Zmienna do losowania większych liczb
    std::random_device rd;
    std::mt19937 gen(rd());

    const int MAX_ORDER = 7;
    for (int o = 1; o <= MAX_ORDER; o++)
    {
        const int n = pow(10, o);
        BST<int> bst;

        std::uniform_int_distribution<int64_t> dis(1, 10 * n);

        auto start_add = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < n; i++)
        {
            int64_t number = dis(gen); // Wygeneruj liczbę
            bst.Insert(number, comparator); // Dodaj do drzewa
        }
        auto end_add = std::chrono::high_resolution_clock::now();

        auto start_search = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; i++)
        {
            int64_t number = dis(gen);
            bst.Search(number, comparator);
        }
        auto end_search = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration_add = end_add - start_add;
        std::chrono::duration<double> duration_search = end_search - start_search;

        uint32_t height = bst.findHeight(bst.getRoot());

        double log_n = log2(n);

        double average_add = duration_add.count() / n;
        double average_search = duration_search.count() / n;

        std::cout << bst.to_string1(bst.getRoot()) << std::endl;
        std::cout << "Drzewo BST z " << n << " elementami\n";
        std::cout << "Czas dodawania elementow: " << duration_add.count() * 1000 << " milisekund\n";
        std::cout << "Sredni czas dodawania: " << average_add * 1000 << " milisekund\n";
        std::cout << "Czas wyszukiwania elementow: " << duration_search.count() * 1000 << " milisekund\n";
        std::cout << "Sredni czas wyszukiwania: " << average_search * 1000 << " milisekund\n";
        std::cout << "Wysokosc drzewa: " << height << "\n";
        std::cout << "Stosunek (wysokosc / rozmiar): " << (double)height / n << "\n";
        std::cout << "Log2(rozmiar): " << log_n << "\n";
        std::cout << "Stosunek (wysokosc / log2(rozmiar)): " << (double)height / log_n << "\n\n";
    }

    return 0;
}
