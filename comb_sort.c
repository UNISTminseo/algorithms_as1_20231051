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

// Comb Sort: 의사코드에 따라 구현
void combSort(int arr[], int n) {
    int gap = n;
    const float shrink = 1.3f; // gap 감소 비율
    int sorted = 0;          // 0: false, 1: true

    // 정렬이 완료되면 sorted가 true가 됨.
    while (!sorted) {
        // gap 업데이트: floor(gap / shrink)
        gap = (int)(gap / shrink);
        if (gap > 1)
            sorted = 0;  // gap이 1보다 크면 sorted는 false로 유지 (comb pass를 반드시 수행)
        else {
            gap = 1;
            sorted = 1;  // gap이 1이면, 일단 sorted로 가정하고 swap이 있으면 false로 변경
        }
        // 각 인덱스 i에 대해 gap만큼 떨어진 요소와 비교
        for (int i = 0; i + gap < n; i++) {
            if (arr[i] > arr[i + gap]) {
                swap(&arr[i], &arr[i + gap]);
                sorted = 0;  // swap 발생 시 정렬되지 않았으므로 sorted = false
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
        arr[i] = n - i;
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
    // 첫 절반은 정렬
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    // 나머지 절반은 랜덤 값
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

// Runs the comb sort algorithm on a given dataset 'iterations' times and prints the average execution time.
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
    
    // 테스트할 다양한 입력 크기: 1K, 5K, 10K, 50K, 100K, 500K, 1M
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int iterations = 10;  // 각 입력 크기마다 10회 실행
    
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