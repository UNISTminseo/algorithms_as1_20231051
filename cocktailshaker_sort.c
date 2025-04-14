#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* --- Cocktail Shaker Sort Function (Knuth-style) --- */
// 이 알고리즘은 왼쪽에서 오른쪽으로 한 번 훑어보며 큰 값을 뒤로 보내고,
// 다시 오른쪽에서 왼쪽으로 훑어보며 작은 값을 앞으로 보내는 과정을 반복합니다.
// 매 회전마다 범위를 좁혀가면서 전체 배열이 정렬될 때까지 반복합니다.
void cocktailShakerSort(int arr[], int n) {
    int left = 0;
    int right = n - 1;
    int lastSwapPos;      // 직전 교환이 발생한 위치
    int swapped = 1;      // 교환 발생 여부 (0: 없음, 1: 있음)
    
    while (swapped) {
        swapped = 0;
        
        // 1) 왼쪽에서 오른쪽으로 스캔
        lastSwapPos = left;  // 교환 발생 위치 추적
        for (int i = left; i < right; i++) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = 1;
                lastSwapPos = i;
            }
        }
        // 오른쪽 범위를 직전 교환 위치까지 줄인다.
        right = lastSwapPos;
        
        if (!swapped) {
            break;  // 교환이 없었다면 이미 정렬됨
        }
        
        swapped = 0;
        
        // 2) 오른쪽에서 왼쪽으로 스캔
        lastSwapPos = right;
        for (int i = right; i > left; i--) {
            if (arr[i - 1] > arr[i]) {
                int temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = temp;
                swapped = 1;
                lastSwapPos = i;
            }
        }
        // 왼쪽 범위를 직전 교환 위치까지 좁힌다.
        left = lastSwapPos;
        
    }
}

/* --- Input Data Generation Functions --- */

// Generates a sorted (ascending) array.
int* generate_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = i;  // 0, 1, 2, ..., n-1
    }
    return arr;
}

// Generates a reverse sorted array (descending).
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;  // n, n-1, ..., 1
    }
    return arr;
}

// Generates an array with random integers (0~9999).
int* generate_random_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Generates a partially sorted array: first half sorted in ascending order, second half random.
int* generate_partially_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    int mid = n / 2;
    // First half: ascending order
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    // Second half: random
    for (int i = mid; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Creates and returns a copy of the source array.
int* copy_array(const int* source, int n) {
    int* copy = (int*) malloc(n * sizeof(int));
    if (!copy) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}

/* --- Experiment Function --- */
// Runs the cocktail shaker sort for 'iterations' times on the dataset 'arr' of size 'n',
// and prints the average execution time (seconds).
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        // Make a copy so the original array is unchanged
        int* data = copy_array(arr, n);
        
        start = clock();
        cocktailShakerSort(data, n);
        end = clock();
        
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;
        
        free(data);
    }
    printf("Dataset: %s, Size: %d, Average execution time (%d runs): %.6f seconds\n",
           dataset_type, n, iterations, total_time / iterations);
}

int main() {
    srand(time(NULL));
    
    // Various input sizes
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int iterations = 10;  // each input size is tested 10 times
    
    for (int k = 0; k < numSizes; k++) {
        int n = sizes[k];
        printf("\n----- Testing with input size: %d -----\n", n);
        
        // Generate data sets
        int* sorted_array          = generate_sorted_array(n);
        int* reverse_sorted_array  = generate_reverse_sorted_array(n);
        int* random_array          = generate_random_array(n);
        int* partially_sorted_array= generate_partially_sorted_array(n);
        
        // Run experiments on each data set
        run_experiment("Sorted array (ascending)",     sorted_array,         n, iterations);
        run_experiment("Sorted array (descending)",    reverse_sorted_array, n, iterations);
        run_experiment("Random array",                 random_array,         n, iterations);
        run_experiment("Partially sorted array",       partially_sorted_array, n, iterations);
        
        // Free allocated memory
        free(sorted_array);
        free(reverse_sorted_array);
        free(random_array);
        free(partially_sorted_array);
    }
    
    return 0;
}