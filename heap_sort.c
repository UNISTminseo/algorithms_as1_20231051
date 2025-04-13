#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* --- Utility Function --- */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* --- Heap Sort Implementation --- */

// heapify: 문헌의 "INHEAP" 또는 "SWOPHEAP"의 기본 아이디어를 반영하여,
// 배열의 인덱스 i를 루트로 하는 서브트리가 최대 힙이 되도록 조정합니다.
// n: 힙(또는 배열)의 크기, i: 서브트리의 루트 인덱스.
void heapify(int arr[], int n, int i) {
    int largest = i;         // 현재 루트를 최댓값으로 가정
    int left = 2 * i + 1;      // 왼쪽 자식 인덱스
    int right = 2 * i + 2;     // 오른쪽 자식 인덱스

    // 왼쪽 자식이 존재하고, 왼쪽 자식이 현재 최대값보다 크면 업데이트
    if (left < n && arr[left] > arr[largest])
        largest = left;
    // 오른쪽 자식도 동일하게 검사
    if (right < n && arr[right] > arr[largest])
        largest = right;
    // 최대값이 루트가 아니라면 교환 후, 교환한 서브트리 재조정 (재귀 호출)
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

// setHeap: 주어진 배열 전체를 최대 힙으로 구성합니다.
// 문헌에서는 "SETHEAP" 절차를 사용한다고 되어 있으나, 여기서는 표준의 bottom-up heap construction을 사용합니다.
void setHeap(int arr[], int n) {
    // n/2-1부터 0까지, 각각에 대해 heapify 수행 (즉, 모든 내부 노드에 대해)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

// heapSort: 문헌의 HEAPSORT 알고리즘을 반영한 구현.
// 먼저 배열을 최대 힙으로 구성하고, 그 후 SWOPHEAP(여기서는 swap 후 재heapify) 과정을 반복하여 정렬합니다.
void heapSort(int arr[], int n) {
    // 배열 전체를 최대 힙으로 구성: SETHEAP(A, n)
    setHeap(arr, n);
    
    // 문헌의 SWOPHEAP 방식과 유사하게, 루트(최대값)를 마지막 원소와 교환한 후, 힙 크기를 줄여가며 힙 재조정.
    for (int i = n - 1; i >= 0; i--) {
        // SWOPHEAP: 현재 배열의 루트(최대 원소)를 배열 끝으로 보내고,
        swap(&arr[0], &arr[i]);
        // 그 후, 남은 배열(크기 i)에 대해 힙 특성을 복원합니다.
        heapify(arr, i, 0);
    }
}

/* --- Input Data Generation Functions --- */

// Generates a sorted array (ascending order).
int* generate_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
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
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
        arr[i] = n - i;
    return arr;
}

// Generates a random array with values between 0 and 9999.
int* generate_random_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;
    return arr;
}

// Generates a partially sorted array: first half is sorted, second half is random.
int* generate_partially_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
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

// Copies an array so that the original remains unchanged for each run.
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
// Runs the heap sort on a given dataset 'iterations' times and prints the average execution time.
// dataset_type: description string of the dataset.
// arr: the source array (unchanged for each run).
// n: size of the array.
// iterations: number of repeated runs.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    
    for (int i = 0; i < iterations; i++) {
        int* data = copy_array(arr, n);
        start = clock();
        heapSort(data, n);
        end = clock();
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;
        free(data);
    }
    printf("Dataset: %s, Size: %d, Average execution time (%d runs): %.6f seconds\n", 
           dataset_type, n, iterations, total_time / iterations);
}

/* --- Main Function --- */
int main() {
    srand(time(NULL));
    
    // 테스트할 다양한 입력 크기를 배열에 저장
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int iterations = 10;  // 각 입력 크기마다 최소 10회 실행
    
    for (int k = 0; k < numSizes; k++) {
        int n = sizes[k];
        printf("\n----- Testing with input size: %d -----\n", n);
        
        // 각 입력 크기에 대해 다양한 데이터셋 생성
        int* sorted_array = generate_sorted_array(n);
        int* reverse_sorted_array = generate_reverse_sorted_array(n);
        int* random_array = generate_random_array(n);
        int* partially_sorted_array = generate_partially_sorted_array(n);
        
        // 각 데이터셋에 대해 실험 실행
        run_experiment("Sorted array (ascending)", sorted_array, n, iterations);
        run_experiment("Sorted array (descending)", reverse_sorted_array, n, iterations);
        run_experiment("Random array", random_array, n, iterations);
        run_experiment("Partially sorted array", partially_sorted_array, n, iterations);
        
        // 동적 할당된 메모리 해제
        free(sorted_array);
        free(reverse_sorted_array);
        free(random_array);
        free(partially_sorted_array);
    }
    
    return 0;
}