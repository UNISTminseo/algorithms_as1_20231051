#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MINRUN 32

// Insertion sort for subarray arr[left...right]
void insertionSort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int temp = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > temp) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = temp;
    }
}

// Merge function: Merges two sorted subarrays of arr.
// The first subarray is arr[l..m] and the second is arr[m+1..r]
void merge(int arr[], int l, int m, int r) {
    int len1 = m - l + 1;
    int len2 = r - m;
    int *leftArr = (int*) malloc(len1 * sizeof(int));
    int *rightArr = (int*) malloc(len2 * sizeof(int));
    if (!leftArr || !rightArr) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < len1; i++)
        leftArr[i] = arr[l + i];
    for (int j = 0; j < len2; j++)
        rightArr[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < len1 && j < len2) {
        if (leftArr[i] <= rightArr[j])
            arr[k++] = leftArr[i++];
        else
            arr[k++] = rightArr[j++];
    }
    while (i < len1)
        arr[k++] = leftArr[i++];
    while (j < len2)
        arr[k++] = rightArr[j++];
    free(leftArr);
    free(rightArr);
}

// Tim Sort implementation.
void timSort(int arr[], int n) {
    // Sort individual runs using insertion sort.
    for (int i = 0; i < n; i += MINRUN) {
        int right = (i + MINRUN - 1 < n) ? i + MINRUN - 1 : n - 1;
        insertionSort(arr, i, right);
    }
    // Merge runs: start from size = MINRUN and double the run size each iteration.
    for (int size = MINRUN; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = (left + 2 * size - 1 < n) ? (left + 2 * size - 1) : n - 1;
            if (mid < right)
                merge(arr, left, mid, right);
        }
    }
}

/* --- Input Data Generation Functions --- */

// Generates a sorted array in ascending order.
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

// Generates a sorted array in descending order (serving as reverse sorted).
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

// Generates a partially sorted array: first half sorted and second half random.
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

// Copies an array; returns a new dynamically allocated copy.
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
// Runs the Tim Sort experiment on a given dataset 'iterations' times and prints the average execution time.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    for (int i = 0; i < iterations; i++) {
        int* data = copy_array(arr, n);
        start = clock();
        timSort(data, n);
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