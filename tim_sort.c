#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MINRUN 32

// -------------------- 구조체 및 함수 프로토타입 --------------------
typedef struct {
    int start;
    int length;
} Run;

void insertionSort(int arr[], int left, int right);
void merge(int arr[], int l, int m, int r);
void reverseArray(int arr[], int n);
void timSort(int arr[], int n);

int* generate_sorted_array(int n);
int* generate_reverse_sorted_array(int n);
int* generate_random_array(int n);
int* generate_partially_sorted_array(int n);
int* copy_array(const int* source, int n);
void run_experiment(const char* dataset_type, int* arr, int n, int iterations);
int is_sorted(int *A, int n);

// qsort용 비교 함수
int cmp_int(const void *a, const void *b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return int_a - int_b;
}

// -------------------- 정렬 관련 함수 --------------------

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

// Merge 함수: arr[l..m]와 arr[m+1..r] 두 정렬된 부분 배열을 병합하여 arr[l..r]을 정렬
void merge(int arr[], int l, int m, int r) {
    int len1 = m - l + 1;
    int len2 = r - m;
    int *leftArr = (int*) malloc(len1 * sizeof(int));
    int *rightArr = (int*) malloc(len2 * sizeof(int));
    if (!leftArr || !rightArr) {
        fprintf(stderr, "Memory allocation error in merge\n");
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

// reverseArray: 배열 A의 원소들을 거꾸로 뒤집음.
void reverseArray(int arr[], int n) {
    for (int i = 0; i < n/2; i++) {
        int temp = arr[i];
        arr[i] = arr[n-1-i];
        arr[n-1-i] = temp;
    }
}

// TimSort: 자연 run을 검출하고, run 길이가 MINRUN보다 작으면 확장한 후, run들을 병합하여 전체 배열을 정렬.
void timSort(int arr[], int n) {
    // runStack: 자연 run들의 시작과 길이를 저장하는 배열.
    int runCapacity = 64;
    Run* runStack = (Run*) malloc(runCapacity * sizeof(Run));
    if (!runStack) {
        fprintf(stderr, "Memory allocation error in timSort (runStack)\n");
        exit(EXIT_FAILURE);
    }
    int runCount = 0;
    
    int i = 0;
    while (i < n) {
        int runStart = i;
        i++;
        // Detect ascending or descending run.
        if (i < n) {
            if (arr[i] < arr[i-1]) {  // Descending run
                while (i < n && arr[i] < arr[i-1])
                    i++;
                reverseArray(arr + runStart, i - runStart);
            } else {  // Ascending run
                while (i < n && arr[i] >= arr[i-1])
                    i++;
            }
        }
        int runLen = i - runStart;
        // If run length is less than MINRUN, extend it using insertion sort.
        if (runLen < MINRUN) {
            int end = (runStart + MINRUN < n) ? runStart + MINRUN : n;
            insertionSort(arr, runStart, end - 1);
            runLen = end - runStart;
            i = end;
        }
        // Push run onto runStack.
        if (runCount >= runCapacity) {
            runCapacity *= 2;
            runStack = (Run*) realloc(runStack, runCapacity * sizeof(Run));
            if (!runStack) {
                fprintf(stderr, "Memory allocation error in timSort (realloc runStack)\n");
                exit(EXIT_FAILURE);
            }
        }
        runStack[runCount].start = runStart;
        runStack[runCount].length = runLen;
        runCount++;
    }
    
    // Merge runs from the stack until only one run remains.
    while (runCount > 1) {
        // 간단하게, 가장 마지막 두 run을 merge.
        int start1 = runStack[runCount - 2].start;
        int len1 = runStack[runCount - 2].length;
        int start2 = runStack[runCount - 1].start;
        int len2 = runStack[runCount - 1].length;
        merge(arr, start1, start1 + len1 - 1, start2 + len2 - 1);
        runStack[runCount - 2].length += runStack[runCount - 1].length;
        runCount--;
    }
    free(runStack);
}

// -------------------- 데이터셋 생성 함수들 --------------------
int* generate_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) { fprintf(stderr, "Memory allocation error!\n"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n; i++)
        arr[i] = i;
    return arr;
}

int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) { fprintf(stderr, "Memory allocation error!\n"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n; i++)
        arr[i] = n - i;
    return arr;
}

int* generate_random_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) { fprintf(stderr, "Memory allocation error!\n"); exit(EXIT_FAILURE); }
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;
    return arr;
}

int* generate_partially_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (!arr) { fprintf(stderr, "Memory allocation error!\n"); exit(EXIT_FAILURE); }
    int mid = n / 2;
    for (int i = 0; i < mid; i++)
        arr[i] = i;
    for (int i = mid; i < n; i++)
        arr[i] = rand() % 10000;
    return arr;
}

int* copy_array(const int* source, int n) {
    int* copy = (int*) malloc(n * sizeof(int));
    if (!copy) { fprintf(stderr, "Memory allocation error!\n"); exit(EXIT_FAILURE); }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}

// -------------------- 정렬 여부 확인 함수 --------------------
int is_sorted(int *A, int n) {
    for (int i = 1; i < n; i++) {
        if (A[i-1] > A[i])
            return 0;
    }
    return 1;
}

// -------------------- 실험 함수 --------------------
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

// -------------------- main() --------------------
int main() {
    srand(time(NULL));
    
    // 테스트할 다양한 입력 크기: 1K, 5K, 10K, 50K, 100K, 500K, 1M
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int iterations = 10;
    
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