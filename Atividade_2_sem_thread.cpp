#include <iostream>
#include <vector>
#include <chrono>

// Tirei o locale pra português porque por alguma razão não tava pegando na minha máquina - Romulo

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;

        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // Se nenhum elemento foi trocado então já está tudo sortido
        if (!swapped) {
            break;
        }
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    const int size = 20000;
    std::vector<int> arr(size);

    // Inicializando assim para que esteja no pior caso
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }

    bubbleSort(arr);
    /*
    Isso aqui só existe pra abasiguar estranheza de alguns compiladores
    Ninguém vai realisticamente ler 20000 dígitos de um array
    - Romulo
    */
    std::cout << "Array sortido: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n\n";

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Tempo de execucao: " << duration.count() << " microsegundos" << std::endl;

    return 0;
}
