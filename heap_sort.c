#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* --- Utility Function --- */

// Swap two integer values.
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* --- Heap Sort Functions --- */

// Heapify: Ensures the subtree rooted at index i is a max heap.
// n: size of the heap (or subarray), i: index of the root of subtree.
void heapify(int arr[], int n, int i) {
    int largest = i;          // Initialize largest as root.
    int left = 2 * i + 1;       // left child index.
    int right = 2 * i + 2;      // right child index.

    // If left child exists and is larger than root.
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    // If right child exists and is larger than the largest so far.
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    // If the largest element is not the root, swap and continue heapifying.
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

// Heap Sort function: sorts an array arr of size n using the heap sort algorithm.
void heapSort(int arr[], int n) {
    // Build max heap.
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    // Extract elements one by one from the heap.
    for (int i = n - 1; i >= 0; i--) {
        // Move current root (largest) to the end.
        swap(&arr[0], &arr[i]);
        // Heapify the reduced heap.
        heapify(arr, i, 0);
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
        arr[i] = i;  // 0, 1, 2, ..., n-1
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
        arr[i] = n - i;  // n, n-1, ..., 1
    }
    return arr;
}

// Generates an array with random integers (0 ~ 9999).
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

// Generates a partially sorted array (first half sorted, second half random).
int* generate_partially_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    int mid = n / 2;
    // First half sorted.
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    // Second half random.
    for (int i = mid; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Copies an array. Returns a new array dynamically allocated as a copy of source.
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
// Runs the heap sort on a given dataset multiple times and prints the average execution time.
// dataset_type: description string of the dataset.
// arr: the source array (unchanged for each run).
// n: size of the array.
// iterations: number of repeated runs.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        int* data = copy_array(arr, n);
        
        start = clock();
        heapSort(data, n);
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