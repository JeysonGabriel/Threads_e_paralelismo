#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>

const int matrix_size = 20;
const int num_threads = 4; // Especificar threads aqui

// Função para gerar uma matriz com valores float aleatórios
std::vector<std::vector<float>> generateRandomMatrix() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    std::vector<std::vector<float>> matrix(matrix_size, std::vector<float>(matrix_size));

    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            matrix[i][j] = dis(gen);
        }
    }

    return matrix;
}

// Função para multiplicação matricial
void matrixMultiply(const std::vector<std::vector<float>>& matrixA, const std::vector<std::vector<float>>& matrixB, std::vector<std::vector<float>>& result, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            result[i][j] = 0.0;
            for (int k = 0; k < matrix_size; ++k) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
}

// Função para multiplicação posicional
void positionMultiply(const std::vector<std::vector<float>>& matrixA, const std::vector<std::vector<float>>& matrixB, std::vector<std::vector<float>>& result, int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            result[i][j] = matrixA[i][j] * matrixB[i][j];
        }
    }
}

int main() {
    // Copiado e colado de outro projeto meu
    auto start_time = std::chrono::high_resolution_clock::now();

    // Gere as matrizes aleatórias
    std::vector<std::vector<float>> matrixA = generateRandomMatrix();
    std::vector<std::vector<float>> matrixB = generateRandomMatrix();

    // Inicialize as matrizes de resultado
    std::vector<std::vector<float>> matrixResultMultiply(matrix_size, std::vector<float>(matrix_size));
    std::vector<std::vector<float>> matrixResultPosition(matrix_size, std::vector<float>(matrix_size));

    // Divida o trabalho entre as threads para multiplicação matricial

    /*
    Eu quase esqueci no meio como isso funcionava, então fiz umas notinhas pra mim
    -Romulo

    1. Esse loop roda pela quantia de threads determinadas anteriormente e forma um vetor
    2. startRow e endRow definem as "filas" que cada thread vai processar
    3. startRow é calculado de forma que cada thread fique com uma quantia de dados iguais pra cada thread

    Isso permite que código seja re-usado pra processar as matrizes em só uma thread também, como com mais!
    */
    std::vector<std::thread> threadsMultiply;
    for (int i = 0; i < num_threads; ++i) {
        int startRow = i * (matrix_size / num_threads);
        int endRow = (i + 1) * (matrix_size / num_threads);
        threadsMultiply.emplace_back(matrixMultiply, std::ref(matrixA), std::ref(matrixB), std::ref(matrixResultMultiply), startRow, endRow);
    }

    // Divida o trabalho entre as threads para multiplicação posicional
    std::vector<std::thread> threadsPosition;
    for (int i = 0; i < num_threads; ++i) {
        int startRow = i * (matrix_size / num_threads);
        int endRow = (i + 1) * (matrix_size / num_threads);
        threadsPosition.emplace_back(positionMultiply, std::ref(matrixA), std::ref(matrixB), std::ref(matrixResultPosition), startRow, endRow);
    }

    // Aguarde todas as threads de multiplicação matricial terminarem
    for (auto& thread : threadsMultiply) {
        thread.join();
    }

    // Aguarde todas as threads de multiplicação posicional terminarem
    for (auto& thread : threadsPosition) {
        thread.join();
    }

    // Imprima as matrizes de resultado (ninguém vai ler é só pro compilador não ter aneurisma)

    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            std::cout << matrixResultMultiply[i][j] << " ";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < matrix_size; ++i) {
        for (int j = 0; j < matrix_size; ++j) {
            std::cout << matrixResultPosition[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Igualmente, copiado e colado de outro projeto meu
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Tempo de execucao: " << duration.count() << " microsegundos" << std::endl;

    return 0;
}
