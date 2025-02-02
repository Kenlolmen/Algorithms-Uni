# Algorytmy ze studiow

## Struktura katalogów
Każdy folder zawiera plik `.cpp` o tej samej nazwie co folder.

### Przykład:
```plaintext
/Algorytmy
│── BST
│   ├── BST.cpp
│── LinkedList
│   ├── LinkedList.cpp
│── Kruskal
│   ├── Kruskal.cpp
```
## Opis
Repozytorium zawiera implementacje różnych algorytmów w języku C++.  
Każdy folder odpowiada jednemu algorytmowi i zawiera jego implementację w pliku `.cpp`.

## Jak używać?
1. Przejdź do folderu z interesującym Cię algorytmem.
2. Skompiluj plik `.cpp` za pomocą kompilatora, np. `g++`:
   ```sh
   g++ NazwaAlgorytmu/NazwaAlgorytmu.cpp -o NazwaAlgorytmu.out
