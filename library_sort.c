#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --------------------- Library Sort Implementation ---------------------

// librarySort sorts the array 'arr' of length n using a simplified Library Sort algorithm.
void librarySort(int arr[], int n) {
    int m = 2 * n;  // Allocate an auxiliary array with size = 2*n.
    int *T = (int*) malloc(m * sizeof(int));
    if (T == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    
    int *occupied = (int*) malloc(m * sizeof(int));  // 0 means empty; 1 means occupied.
    if (occupied == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < m; i++) {
        occupied[i] = 0;
    }
    
    // posList will store the indices in T that are occupied, in sorted order.
    int *posList = (int*) malloc(n * sizeof(int));
    if (posList == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    int posCount = 0;
    
    // Insert the first element into the middle of T.
    int start_index = m / 2;
    T[start_index] = arr[0];
    occupied[start_index] = 1;
    posList[0] = start_index;
    posCount = 1;
    
    // Insert each subsequent element.
    for (int i = 1; i < n; i++) {
        int x = arr[i];
        int insertIndex = 0;
        
        // Determine the proper position (in the sorted order of inserted elements) using binary search.
        if (x < T[posList[0]]) {
            insertIndex = 0;
        } else if (x >= T[posList[posCount - 1]]) {
            insertIndex = posCount;
        } else {
            int lo = 0, hi = posCount - 1;
            while (lo <= hi) {
                int mid = (lo + hi) / 2;
                if (T[posList[mid]] <= x) {
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
            insertIndex = lo;
        }
        
        // Compute the ideal index for insertion.
        int ideal = 0;
        if (insertIndex == 0) {
            ideal = posList[0] - 1;
            if (ideal < 0)
                ideal = posList[0] + 1;
        } else if (insertIndex == posCount) {
            ideal = posList[posCount - 1] + 1;
            if (ideal >= m)
                ideal = posList[posCount - 1] - 1;
        } else {
            ideal = (posList[insertIndex - 1] + posList[insertIndex]) / 2;
        }
        
        // Find the nearest empty slot starting at the ideal position (searching rightwards first).
        int pos = ideal;
        while (pos < m && occupied[pos]) {
            pos++;
        }
        if (pos >= m) {
            pos = ideal;
            while (pos >= 0 && occupied[pos]) {
                pos--;
            }
            if (pos < 0)
                pos = ideal;
        }
        
        // Insert x into T at the found position.
        T[pos] = x;
        occupied[pos] = 1;
        
        // Update posList by inserting 'pos' into the sorted list.
        for (int j = posCount; j > insertIndex; j--) {
            posList[j] = posList[j - 1];
        }
        posList[insertIndex] = pos;
        posCount++;
    }
    
    // Extract the sorted elements from T using posList.
    for (int i = 0; i < n; i++) {
        arr[i] = T[posList[i]];
    }
    
    free(T);
    free(occupied);
    free(posList);
}

// --------------------- Input Data Generation Functions ---------------------

// Generates a sorted array (ascending order).
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

// Generates a sorted array in descending order (this represents the "reverse sorted" case).
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

// --------------------- Experiment Function ---------------------

// run_experiment runs the library sort on the provided dataset 'iterations' times 
// and prints the average execution time.
// dataset_type: description of the dataset (e.g., "Sorted array (ascending)").
// arr: the source array.
// n: size of the array.
// iterations: number of runs to compute the average execution time.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        int* data = copy_array(arr, n);
        start = clock();
        librarySort(data, n);
        end = clock();
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;
        free(data);
    }
    printf("Dataset: %s, Size: %d, Average execution time (%d runs): %.6f seconds\n",
           dataset_type, n, iterations, total_time / iterations);
}

// --------------------- Main Function ---------------------

int main() {
    // Initialize random seed.
    srand(time(NULL));
    
    // Experiment parameters:
    // The assignment suggests testing with various input sizes (from 1K to 1M). Here we use 1000 for demonstration.
    int n = 1000;
    int iterations = 10;  // Run each test 10 times.
    
    // Generate input datasets.
    int* sorted_array = generate_sorted_array(n);
    int* reverse_sorted_array = generate_reverse_sorted_array(n);  // Already covers the "reverse" case.
    int* random_array = generate_random_array(n);
    int* partially_sorted_array = generate_partially_sorted_array(n);
    
    // Run experiments using Library Sort.
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