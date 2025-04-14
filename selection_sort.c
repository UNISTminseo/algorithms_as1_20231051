#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* --- Selection Sort Function --- */
// Sorts the array 'arr' of size 'n' in ascending order using selection sort.
void selectionSort(int arr[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        // Assume the minimum is the first element.
        int min_index = i;
        // Find the minimum element in the unsorted part.
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_index]) {
                min_index = j;
            }
        }
        // Swap the found minimum element with the first element.
        if (min_index != i) {
            int temp = arr[i];
            arr[i] = arr[min_index];
            arr[min_index] = temp;
        }
    }
}

/* --- Input Data Generation Functions --- */

// Generates a sorted (ascending) array.
int* generate_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = i;  // Sorted ascending: 0, 1, 2, ..., n-1
    }
    return arr;
}

// Generates a reverse sorted (descending) array.
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;  // Descending: n, n-1, ..., 1
    }
    return arr;
}

// Generates an array filled with random integers between 0 and 9999.
int* generate_random_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Generates a partially sorted array: first half sorted, second half random.
int* generate_partially_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    int mid = n / 2;
    // First half is sorted.
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    // Second half is filled with random integers.
    for (int i = mid; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Creates and returns a copy of the source array.
int* copy_array(const int* source, int n) {
    int* copy = (int*) malloc(n * sizeof(int));
    if (copy == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}

/* --- Experiment Function --- */
// Runs the selection sort on a given dataset multiple times and prints the average execution time.
// - dataset_type: Description of the dataset (e.g., "Sorted array").
// - arr: The source array for testing (original data remains unchanged).
// - n: Size of the array.
// - iterations: Number of runs to compute the average time.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        // Create a copy of the array for each run.
        int* data = copy_array(arr, n);
        
        start = clock();
        selectionSort(data, n);
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
    
    // 테스트할 다양한 입력 크기를 배열에 저장합니다.
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int iterations = 10;  // 각 입력 크기마다 최소 10회 실행
    
    for (int k = 0; k < numSizes; k++) {
        int n = sizes[k];
        printf("\n----- Testing with input size: %d -----\n", n);
        
        // 각 입력 크기에 대해 다양한 데이터셋 생성
        int* sorted_array         = generate_sorted_array(n);
        int* reverse_sorted_array = generate_reverse_sorted_array(n);
        int* random_array         = generate_random_array(n);
        int* partially_sorted_array = generate_partially_sorted_array(n);
        
        // 각 데이터셋에 대해 실험 실행
        run_experiment("Sorted array (ascending)", sorted_array, n, iterations);
        run_experiment("Sorted array (descending)", reverse_sorted_array, n, iterations);
        run_experiment("Random array", random_array, n, iterations);
        run_experiment("Partially sorted array", partially_sorted_array, n, iterations);
        
        // 동적 할당한 메모리 해제
        free(sorted_array);
        free(reverse_sorted_array);
        free(random_array);
        free(partially_sorted_array);
    }

    return 0;
}