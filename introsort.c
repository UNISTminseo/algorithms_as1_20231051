#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define THRESHOLD 16

/* -------------------- Utility Functions -------------------- */

// Swap two integer values.
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for quicksort.
// Uses the last element as pivot.
int partition(int arr[], int low, int high) {
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

// Insertion sort for subarray arr[left...right].
void insertionSort_sub(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Heapify for subarray arr[left...right] treated as a heap.
// n: number of elements (right - left + 1), i: index in [0, n-1]
void heapify_range(int arr[], int left, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    
    if (l < n && arr[left + l] > arr[left + largest])
        largest = l;
    if (r < n && arr[left + r] > arr[left + largest])
        largest = r;
    if (largest != i) {
        swap(&arr[left + i], &arr[left + largest]);
        heapify_range(arr, left, n, largest);
    }
}

// Heapsort on subarray arr[left...right].
void heapsort_range(int arr[], int left, int right) {
    int n = right - left + 1;
    // Build max heap.
    for (int i = n/2 - 1; i >= 0; i--) {
        heapify_range(arr, left, n, i);
    }
    // Extract elements from heap.
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[left], &arr[left + i]);
        heapify_range(arr, left, i, 0);
    }
}

/* -------------------- Introsort Implementation -------------------- */

// Recursive utility for introsort.
void introsort_util(int arr[], int left, int right, int depthLimit) {
    int size = right - left + 1;
    if (size < THRESHOLD) {
        insertionSort_sub(arr, left, right);
        return;
    }
    if (depthLimit == 0) {
        heapsort_range(arr, left, right);
        return;
    }
    int pivot = partition(arr, left, right);
    introsort_util(arr, left, pivot - 1, depthLimit - 1);
    introsort_util(arr, pivot + 1, right, depthLimit - 1);
}

// Introsort: sorts array arr of length n.
void introsort(int arr[], int n) {
    int depthLimit = 2 * (int)log(n);
    introsort_util(arr, 0, n - 1, depthLimit);
}

/* -------------------- Input Data Generation Functions -------------------- */

// Generates a sorted array (ascending order).
int* generate_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
        arr[i] = i;
    return arr;
}

// Generates a reverse sorted array (descending order).
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
        arr[i] = n - i;
    return arr;
}

// Generates a random array with integers between 0 and 9999.
int* generate_random_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;
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
    for (int i = 0; i < mid; i++)
        arr[i] = i;
    for (int i = mid; i < n; i++)
        arr[i] = rand() % 10000;
    return arr;
}

// Copies an array; returns a new copy of the source.
int* copy_array(const int* source, int n) {
    int* copy = (int*) malloc(n * sizeof(int));
    if (!copy) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}

/* -------------------- Experiment Function -------------------- */

// Runs the introsort experiment on a given dataset for 'iterations' times 
// and prints the average execution time.
// dataset_type: Description of the dataset (e.g., "Sorted array (ascending)").
// arr: The source array to sort (remains unchanged for each run).
// n: The size of the array.
// iterations: Number of runs for averaging.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        int* data = copy_array(arr, n);
        start = clock();
        introsort(data, n);
        end = clock();
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;
        free(data);
    }
    printf("Dataset: %s, Size: %d, Average execution time (%d runs): %.6f seconds\n",
           dataset_type, n, iterations, total_time / iterations);
}

/* -------------------- Main Function -------------------- */
int main() {
    srand(time(NULL));
    
    // 테스트할 다양한 입력 크기: 1000, 5000, 10000, 50000, 100000, 500000, 1000000
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int iterations = 10; // 각 입력 크기마다 최소 10회 실행
    
    for (int k = 0; k < numSizes; k++) {
        int n = sizes[k];
        printf("\n----- Testing with input size: %d -----\n", n);
        
        // 각 데이터셋 생성
        int* sorted_array          = generate_sorted_array(n);
        int* reverse_sorted_array  = generate_reverse_sorted_array(n);
        int* random_array          = generate_random_array(n);
        int* partially_sorted_array= generate_partially_sorted_array(n);
        
        // 각 데이터셋에 대해 실험 실행
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