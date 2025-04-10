#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Swap two integers.
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function using the Lomuto partition scheme.
int partition(int arr[], int low, int high) {
    int pivot = arr[high];  // Choose the last element as pivot.
    int i = low - 1;        // Index of smaller element.
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Quick Sort function: sorts arr[low...high] in ascending order.
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
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

// Generates a sorted (descending) array.
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;  // Descending order.
    }
    return arr;
}

// Generates an array with random integers between 0 and 9999.
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

// Creates a copy of the source array.
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
// Runs the Quick Sort experiment on the given dataset multiple times and prints the average execution time.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        // Use a copy of the dataset for each run so that the original remains unchanged.
        int* data = copy_array(arr, n);
        
        start = clock();
        quickSort(data, 0, n - 1);
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
    // Although the assignment requires testing with varying sizes (1K to 1M), here we use 1000 for demonstration.
    int n = 1000;
    int iterations = 10;  // Run each test 10 times to compute the average execution time.
    
    // Generate input datasets.
    int* sorted_array = generate_sorted_array(n);
    int* reverse_sorted_array = generate_reverse_sorted_array(n); // This represents descending order.
    int* random_array = generate_random_array(n);
    int* partially_sorted_array = generate_partially_sorted_array(n);
    
    // Run experiments using Quick Sort.
    run_experiment("Sorted array (ascending)", sorted_array, n, iterations);
    run_experiment("Sorted array (descending)", reverse_sorted_array, n, iterations);
    run_experiment("Random array", random_array, n, iterations);
    run_experiment("Partially sorted array", partially_sorted_array, n, iterations);
    
    // Free allocated memory.
    free(sorted_array);
    free(reverse_sorted_array);
    free(random_array);
    free(partially_sorted_array);
    
    return 0;
}