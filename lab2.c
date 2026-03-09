#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

typedef struct {
    int* array;
    int size;
} ThreadData;

DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    int* arr = data->array;
    int n = data->size;

    if (n <= 0) return 1;

    int min = arr[0];
    int max = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
        Sleep(7);
        if (arr[i] > max) {
            max = arr[i];
        }
        Sleep(7);
    }

    printf("Поток min_max: минимальный элемент = %d, максимальный элемент = %d\n", min, max);
    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    int* arr = data->array;
    int n = data->size;

    if (n <= 0) return 1;

    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        Sleep(12);
    }

    double average = (double)sum / n;
    printf("Поток average: среднее арифметическое = %.2f\n", average);
    return 0;
}

int main() {
        SetConsoleCP(1251);       
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL,"Russian"); 
    int size;
    printf("Введите размерность массива: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("Размер массива должен быть положительным числом.\n");
        return 1;
    }

    int* array = (int*)malloc(sizeof(int) * size);
    if (array == NULL) {
        printf("Ошибка выделения памяти.\n");
        return 1;
    }

    printf("Введите %d целых чисел:\n", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &array[i]);
    }

    ThreadData data;
    data.array = array;
    data.size = size;

    DWORD min_maxThreadId;
    HANDLE hMinMax = CreateThread(NULL, 0, MinMaxThread, &data, 0, &min_maxThreadId);
    if (hMinMax == NULL) {
        printf("Ошибка создания потока min_max.\n");
        free(array);
        return 1;
    }

    DWORD averageThreadId;
    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, &data, 0, &averageThreadId);
    if (hAverage == NULL) {
        printf("Ошибка создания потока average.\n");
        CloseHandle(hMinMax);
        free(array);
        return 1;
    }

    printf("Ожидание завершения потоков...\n");
    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    long long sum = 0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    double average = (double)sum / size;

    int min_val = array[0];
    int max_val = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] < min_val) min_val = array[i];
        if (array[i] > max_val) max_val = array[i];
    }

    for (int i = 0; i < size; i++) {
        if (array[i] == min_val || array[i] == max_val) {
            array[i] = (int)average;
        }
    }

    printf("Массив после замены минимального и максимального значений на среднее (%.2f):\n", average);
    for (int i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    free(array);
    printf("Программа завершена.\n");
    return 0;
}