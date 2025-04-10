#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

// Swap two integers.
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Returns the median-of-three pivot by ordering arr[low], arr[mid], arr[high].
// After ordering, it swaps the median into arr[high] (pivot position).
void medianOfThree(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid])
        swap(&arr[low], &arr[mid]);
    if (arr[low] > arr[high])
        swap(&arr[low], &arr[high]);
    if (arr[mid] > arr[high])
        swap(&arr[mid], &arr[high]);
    // Now, arr[low] <= arr[mid] <= arr[high]; use arr[mid] as pivot by swapping it with arr[high].
    swap(&arr[mid], &arr[high]);
}

// Partition function using the Lomuto scheme with median-of-three pivot selection.
int partition(int arr[], int low, int high) {
    medianOfThree(arr, low, high);  // Pivot is moved to arr[high]
    int pivot = arr[high];  
    int i = low - 1;        
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Quick Sort function with tail recursion elimination.
// This version sorts arr[low...high] in ascending order.
void quickSort(int arr[], int low, int high) {
    while (low < high) {
        int pivotIndex = partition(arr, low, high);
        // Recurse on the smaller subarray first to limit stack depth.
        if (pivotIndex - low < high - pivotIndex) {
            quickSort(arr, low, pivotIndex - 1);
            low = pivotIndex + 1;  // Tail recursion for the larger subarray.
        } else {
            quickSort(arr, pivotIndex + 1, high);
            high = pivotIndex - 1;
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
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
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
    srand(time(NULL));
    
    // 다양한 입력 크기 (1K ~ 1M)로 테스트
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int iterations = 10;  // 각 입력 크기마다 최소 10회 실행
    
    for (int k = 0; k < numSizes; k++) {
        int n = sizes[k];
        printf("\n----- Testing with input size: %d -----\n", n);
        
        int* sorted_array = generate_sorted_array(n);
        int* reverse_sorted_array = generate_reverse_sorted_array(n);
        int* random_array = generate_random_array(n);
        int* partially_sorted_array = generate_partially_sorted_array(n);
        
        run_experiment("Sorted array (ascending)", sorted_array, n, iterations);
        run_experiment("Sorted array (descending)", reverse_sorted_array, n, iterations);
        run_experiment("Random array", random_array, n, iterations);
        run_experiment("Partially sorted array", partially_sorted_array, n, iterations);
        
        free(sorted_array);
        free(reverse_sorted_array);
        free(random_array);
        free(partially_sorted_array);
    }
    
    return 0;
}