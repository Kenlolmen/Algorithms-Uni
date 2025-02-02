#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>


template <typename V>
struct HashEntry {
    std::string key;
    V value;

    HashEntry(std::string k, V v) : key(k), value(v) {}
};

template <typename V>
class HashTable {
private:
    std::vector<std::list<HashEntry<V>>> table;
    int current_size;
    int max_size;
    float load_factor;

    int hash_function(const std::string& key) {
        int hash_value = 0;
        for (size_t i = 0; i < key.size(); ++i) {
            hash_value = (hash_value * 31 + key[i]) % max_size;
        }
        return hash_value;
    }

    void rehash() {
        int old_max_size = max_size;
        max_size *= 2;
        std::vector<std::list<HashEntry<V>>> new_table(max_size);


        for (int i = 0; i < old_max_size; ++i) {
            for (auto& entry : table[i]) {
                int new_index = hash_function(entry.key);
                new_table[new_index].push_back(entry);
            }
        }
        table = std::move(new_table);
    }

public:
    HashTable(int initial_size = 16, float load_factor = 0.75) : max_size(initial_size), load_factor(load_factor), current_size(0) {
        table.resize(max_size);
    }

    void put(const std::string& key, V value) {
        if (current_size >= max_size * load_factor) {
            rehash();
        }
        int index = hash_function(key);

        for (auto& entry : table[index]) {
            if (entry.key == key) {
                entry.value = value;
                return;
            }
        }

        table[index].push_back(HashEntry<V>(key, value));
        ++current_size;
    }

    HashEntry<V>* get(const std::string& key) {
        int index = hash_function(key);
        for (auto& entry : table[index]) {
            if (entry.key == key) {
                return &entry;
            }
        }
        return nullptr;  // If not found
    }

    bool remove(const std::string& key) {
        int index = hash_function(key);
        auto& list = table[index];
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (it->key == key) {
                list.erase(it);
                --current_size;
                return true;
            }
        }
        return false;
    }

    void clear() {
        for (int i = 0; i < max_size; ++i) {
            table[i].clear();
        }
        current_size = 0;
    }

    void print_stats() {
        int non_empty_lists = 0;
        int min_size = INT_MAX;
        int max_size = 0;
        int total_size = 0;

        for (const auto& list : table) {
            if (!list.empty()) {
                ++non_empty_lists;
                int list_size = list.size();
                min_size = std::min(min_size, list_size);
                max_size = std::max(max_size, list_size);
                total_size += list_size;
            }
        }

        std::cout << "Stats:\n";
        std::cout << "Non-null lists: " << non_empty_lists << "\n";
        std::cout << "List min size: " << min_size << "\n";
        std::cout << "List max size: " << max_size << "\n";
        std::cout << "List avg size: " << (non_empty_lists ? (float)total_size / non_empty_lists : 0) << "\n";
    }

    void print_table() {
        for (int i = 0; i < max_size; ++i) {
            if (!table[i].empty()) {
                std::cout << i << ": ";
                for (auto& entry : table[i]) {
                    std::cout << entry.key << " -> " << entry.value << "; ";
                }
                std::cout << "\n";
            }
        }
    }
};


std::string random_key(int length) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string key = "";
    for (int i = 0; i < length; ++i) {
        key += chars[rand() % chars.size()];
    }
    return key;
}

int main() {
    srand(time(0));
    const int MAX_ORDER = 7;
    HashTable<int> ht;

    for (int o = 1; o <= MAX_ORDER; ++o) {
        const int n = std::pow(10, o);


        std::vector<std::string> keys;

        // Add elements
        clock_t t1 = clock();
        for (int i = 0; i < n; ++i) {
            std::string key = random_key(6);
            ht.put(key, i);
            keys.push_back(key);
        }
        clock_t t2 = clock();


        std::cout << "Added " << n << " elements in " << ((t2 - t1) / (double)CLOCKS_PER_SEC) << " seconds\n";


        const int m = 10000;
        int hits = 0;
        t1 = clock();
        for (int i = 0; i < m; ++i) {
            if (ht.get(keys[rand() % n]) != nullptr) {
                ++hits;
            }
        }
        t2 = clock();

        std::cout << "Searched for " << m << " elements, hits: " << hits << " in " << ((t2 - t1) / (double)CLOCKS_PER_SEC) << " seconds\n";
        ht.print_stats();
        ht.clear();
    }

    return 0;
}
