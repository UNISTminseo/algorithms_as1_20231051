#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* --- Cocktail Shaker Sort Function --- */
// Sorts the array 'arr' of length 'n' in ascending order using Cocktail Shaker Sort.
void cocktailShakerSort(int arr[], int n) {
    int start = 0;
    int end = n - 1;
    int swapped = 1;
    
    while (swapped) {
        swapped = 0;
        
        // Traverse from left to right.
        for (int i = start; i < end; i++) {
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = 1;
            }
        }
        
        // If no elements were swapped, then the array is sorted.
        if (!swapped)
            break;
        
        // Otherwise, reset swapped flag for the next phase.
        swapped = 0;
        
        // Move the end point back by one because the item at the end is in its correct place.
        end--;
        
        // Traverse from right to left.
        for (int i = end; i > start; i--) {
            if (arr[i - 1] > arr[i]) {
                int temp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = temp;
                swapped = 1;
            }
        }
        
        // Increase the starting point because the item at the start is in its correct place.
        start++;
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
        arr[i] = i;  // 0, 1, 2, ..., n-1 in ascending order.
    }
    return arr;
}

// Generates a sorted (descending) array.
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;  // n, n-1, ..., 1 in descending order.
    }
    return arr;
}

// Generates an array with random integers between 0 and 9999.
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

// Generates a partially sorted array: first half sorted, second half random.
int* generate_partially_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    int mid = n / 2;
    // First half: sorted in ascending order.
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    // Second half: random values.
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
// Runs the cocktail shaker sort on the provided dataset 'iterations' times 
// and prints the average execution time.
// dataset_type: A description of the dataset (e.g., "Sorted array (ascending)").
// arr: The original dataset to be sorted.
// n: The size of the array.
// iterations: Number of test runs for averaging.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        // For each run, create a copy of the dataset so the original remains unchanged.
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
    // Initialize random seed.
    srand(time(NULL));
    
    // Experiment parameters:
    // The assignment suggests testing with varying sizes (from 1K to 1M). For demonstration, we use 1000.
    int n = 1000;
    int iterations = 10;  // Run each test 10 times.
    
    // Generate input datasets.
    int* sorted_array = generate_sorted_array(n);
    int* reverse_sorted_array = generate_reverse_sorted_array(n);  // Reverse sorted (descending order).
    int* random_array = generate_random_array(n);
    int* partially_sorted_array = generate_partially_sorted_array(n);
    
    // Run experiments using Cocktail Shaker Sort.
    run_experiment("Sorted array (ascending)", sorted_array, n, iterations);
    run_experiment("Sorted array (descending)", reverse_sorted_array, n, iterations);
    run_experiment("Random array", random_array, n, iterations);
    run_experiment("Partially sorted array", partially_sorted_array, n, iterations);
    
    // Free dynamically allocated memory.
    free(sorted_array);
    free(reverse_sorted_array);
    free(random_array);
    free(partially_sorted_array);
    
    return 0;
}