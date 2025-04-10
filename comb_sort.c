#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// -------------------- Comb Sort Implementation --------------------

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Comb Sort: a variant of bubble sort that compares elements at a gap, which decreases over time.
void combSort(int arr[], int n) {
    int gap = n;
    const float shrink = 1.3f; // Common shrink factor
    int swapped = 1;
    
    while (gap > 1 || swapped) {
        // Update gap for next comb
        gap = (int)(gap / shrink);
        if (gap < 1)
            gap = 1;
        
        swapped = 0;
        // Compare elements with the current gap
        for (int i = 0; i + gap < n; i++) {
            if (arr[i] > arr[i + gap]) {
                swap(&arr[i], &arr[i + gap]);
                swapped = 1;
            }
        }
    }
}

// -------------------- Input Data Generation Functions --------------------

// Generates a sorted array (ascending order).
int* generate_sorted_array(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    if(arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
    return arr;
}

// Generates a reverse sorted array (descending order).
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    if(arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = n - i; // n, n-1, ..., 1
    }
    return arr;
}

// Generates a random array with values between 0 and 9999.
int* generate_random_array(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    if(arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Generates a partially sorted array: first half is sorted, second half is random.
int* generate_partially_sorted_array(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    if(arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    int mid = n / 2;
    // First half sorted in ascending order.
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    // Second half random.
    for (int i = mid; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Copies an array so that the original remains unchanged for each run.
int* copy_array(const int* source, int n) {
    int* copy = (int*)malloc(n * sizeof(int));
    if(copy == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}

// -------------------- Experiment Function --------------------

// Runs the comb sort algorithm on a given dataset 'iterations' times.
// dataset_type: A description of the dataset (e.g., "Sorted array (ascending)").
// arr: The source array to sort.
// n: Size of the array.
// iterations: Number of runs used to compute the average execution time.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        int* data = copy_array(arr, n);
        start = clock();
        combSort(data, n);
        end = clock();
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;
        free(data);
    }
    printf("Dataset: %s, Size: %d, Average execution time (%d runs): %.6f seconds\n", 
           dataset_type, n, iterations, total_time / iterations);
}

// -------------------- Main Function --------------------

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