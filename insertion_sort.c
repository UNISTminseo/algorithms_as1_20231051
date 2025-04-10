#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* --- Insertion Sort Function --- */
// Sorts the array 'arr' of size 'n' in ascending order using insertion sort.
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];  // The element to be inserted.
        int j = i - 1;
        // Shift elements of arr[0..i-1], that are greater than key, to one position ahead.
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        // Insert the key element at the correct position.
        arr[j + 1] = key;
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
        arr[i] = i;
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
        arr[i] = n - i;
    }
    return arr;
}

// Generates an array with random integers (0 to 9999).
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
    // First half: sorted.
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    // Second half: random values.
    for (int i = mid; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Copies an array. This function creates a new copy of the source array.
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
// Runs the insertion sort experiment on a given dataset multiple times and prints the average execution time.
// - dataset_type: Description of the dataset (e.g., "Sorted array").
// - arr: The source array to be sorted (original data remains unchanged).
// - n: Size of the array.
// - iterations: Number of runs for averaging.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        // Create a copy of the dataset for each run.
        int* data = copy_array(arr, n);
        
        start = clock();
        insertionSort(data, n);
        end = clock();
        
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;
        
        free(data);
    }
    printf("Dataset: %s, Size: %d, Average execution time (%d runs): %.6f seconds\n", 
           dataset_type, n, iterations, total_time / iterations);
}

int main() {
    // Initialize random seed.
    srand(time(NULL));
    
    // Experiment parameters:
    // The assignment requires testing various input sizes (from 1K to 1M).
    // For demonstration purposes, we use 1000.
    int n = 1000;
    int iterations = 10; // Run each test 10 times to compute average execution time.
    
    // Generate input datasets.
    int* sorted_array = generate_sorted_array(n);
    int* reverse_sorted_array = generate_reverse_sorted_array(n);
    int* random_array = generate_random_array(n);
    int* partially_sorted_array = generate_partially_sorted_array(n);
    
    // Run experiments using insertion sort.
    run_experiment("Sorted array", sorted_array, n, iterations);
    run_experiment("Reverse sorted array", reverse_sorted_array, n, iterations);
    run_experiment("Random array", random_array, n, iterations);
    run_experiment("Partially sorted array", partially_sorted_array, n, iterations);
    
    // Free dynamically allocated memory.
    free(sorted_array);
    free(reverse_sorted_array);
    free(random_array);
    free(partially_sorted_array);
    
    return 0;
}