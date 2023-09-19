#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// Tirei o locale pra português porque por alguma razão não tava pegando na minha máquina - Romulo

// Função para realizar o bubblesort em uma função só da array
void bubbleSort(std::vector<int>& arr, int start, int end) {
    for (int i = start; i < end; i++) {
        for (int j = start; j < end - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Função para juntar as duas metades do array
void merge(std::vector<int>& arr, int start, int mid, int end) {
    std::vector<int> temp(end - start + 1);
    int i = start, j = mid + 1, k = 0;

    while (i <= mid && j <= end) {
        if (arr[i] < arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= end) {
        temp[k++] = arr[j++];
    }

    for (i = 0; i < k; i++) {
        arr[start + i] = temp[i];
    }
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();

    const int size = 20000;
    std::vector<int> arr(size);

    // Inicializando no pior caso possível para o bubblesort
    for (int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
    /*
    Procura o meio certinho e então separa cada metade em duas threads diferentes, bem simples!
    Tem um outro esquema pra separar dados certinhos pra cada thread feito no de matriz
    Mas bubblesort é um bagulho tão besta que eu fiz assim, com 2 threads ele já mostra diferença massiva
    */
    int mid = size / 2;

    std::thread thread1(bubbleSort, std::ref(arr), 0, mid);
    std::thread thread2(bubbleSort, std::ref(arr), mid, size);

    thread1.join();
    thread2.join();

    merge(arr, 0, mid - 1, size - 1);

    std::cout << "Sorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
