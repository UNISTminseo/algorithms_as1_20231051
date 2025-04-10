#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* --- Utility Functions --- */

// Swap two values
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* --- Bubble Sort Function --- */
// Sorts the array arr of size n in ascending order using bubble sort.
void bubbleSort(int arr[], int n) {
    int i, j;
    int swapped;  // Flag to check if any swap occurred in the inner loop.
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }
        // If no two elements were swapped by inner loop, then break
        if (!swapped) {
            break;
        }
    }
}

/* --- Input Data Generation Functions --- */

// Generates a sorted (ascending) array - Sorted Data.
int* generate_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = i;  // Sorted in ascending order: 0 to n-1
    }
    return arr;
}

// Generates a reverse sorted (descending) array - Reverse Sorted Data.
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;  // Descending order: n to 1
    }
    return arr;
}

// Generates an array with random numbers (range 0 to 9999) - Random Data.
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

// Generates a partially sorted array (first half sorted, second half random) - Partially Sorted Data.
int* generate_partially_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    int mid = n / 2;
    // First half is sorted in ascending order.
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    // Second half contains random numbers.
    for (int i = mid; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Copies an array. Used so the original dataset remains unchanged for each run.
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
// Runs the bubble sort experiment on a given dataset.
// dataset_type: Description of the dataset.
// arr: The source array to test.
// n: Size of the array.
// iterations: Number of runs for averaging.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        // Create a copy of the source array for each run.
        int* data = copy_array(arr, n);
        
        start = clock();
        bubbleSort(data, n);
        end = clock();
        
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;
        
        free(data);
    }
    printf("Dataset: %s, Size: %d, Average execution time (%d runs): %.6f seconds\n", dataset_type, n, iterations, total_time / iterations);
}

int main() {
    // Initialize random seed
    srand(time(NULL));
    
    // Experiment parameters: size of array and number of iterations.
    // The assignment suggests varying sizes between 1K to 1M, but here we use 1000 for demonstration.
    int n = 1000;
    int iterations = 10;  // Run each test at least 10 times to calculate the average time.
    
    // Generate input data for experiments.
    int* sorted_array = generate_sorted_array(n);
    int* reverse_sorted_array = generate_reverse_sorted_array(n);
    int* random_array = generate_random_array(n);
    int* partially_sorted_array = generate_partially_sorted_array(n);
    
    // Run experiments on each dataset.
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