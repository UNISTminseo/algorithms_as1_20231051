#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>  // INT_MAX 사용

// -------------------- Utility Function --------------------
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Compute the next power of 2 greater than or equal to n.
int nextPowerOf2(int n) {
    int p = 1;
    while (p < n)
        p *= 2;
    return p;
}

// -------------------- Tournament Sort Function --------------------
// 이 구현은 문헌에서 설명하는 토너먼트 트리(primitives: LISTIFY!, TOURNAMENT!, 등)의 주요 아이디어를 반영합니다.
// 먼저 입력 배열을 적절한 길이(k, 2의 거듭제곱)로 확장한 temp 배열을 만들고,
// leaves에 해당하는 인덱스들을 tree 배열의 하위 절반(인덱스 [k, 2*k-1])에 저장합니다.
// 그 후 내부 노드들을 채워 전체 트리(토너먼트)를 구성하고,
// tree[1] (루트)에 저장된 인덱스를 사용해 sorted 배열에 순서대로 최소 원소들을 추출합니다.
void tournamentSort(int arr[], int n) {
    int k = nextPowerOf2(n);
    
    // temp 배열: 입력 배열 값을 담고, 나머지는 INT_MAX로 채움.
    int *temp = (int*) malloc(k * sizeof(int));
    if (!temp) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        temp[i] = arr[i];
    }
    for (int i = n; i < k; i++) {
        temp[i] = INT_MAX;
    }
    
    // tree 배열: 2*k 크기의 배열로, 리프 노드 (인덱스 k부터 2*k-1)에 temp 인덱스를 저장.
    int treeSize = 2 * k;
    int *tree = (int*) malloc(treeSize * sizeof(int));
    if (!tree) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < k; i++) {
        tree[k + i] = i;  // 각 리프는 자신을 가리킴.
    }
    
    // 내부 노드를 채워, 각 노드에 두 자식 중 값이 작은 쪽(우승자)의 인덱스를 저장.
    for (int i = k - 1; i >= 1; i--) {
        int left = tree[2 * i];
        int right = tree[2 * i + 1];
        tree[i] = (temp[left] <= temp[right]) ? left : right;
    }
    
    // sorted 배열: 추출된 최소 원소들을 저장.
    int *sorted = (int*) malloc(n * sizeof(int));
    if (!sorted) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    
    // 전체 토너먼트에서 최소 원소를 한 번씩 추출.
    for (int i = 0; i < n; i++) {
        int winner = tree[1];  // 루트에 있는 인덱스가 최소 원소의 인덱스.
        sorted[i] = temp[winner];
        // 추출한 원소는 더 이상 선택되지 않도록 INT_MAX로 설정.
        temp[winner] = INT_MAX;
        
        // 리프 노드에 해당하는 위치를 찾고, 상위 경로를 따라 트리 재조정.
        int j = winner + k;  // 리프 위치.
        tree[j] = winner;    // 리프 노드 값은 변경하지 않아도 되지만, 업데이트.
        for (j /= 2; j >= 1; j /= 2) {
            int left = tree[2 * j];
            int right = tree[2 * j + 1];
            tree[j] = (temp[left] <= temp[right]) ? left : right;
        }
    }
    
    // sorted 결과를 arr에 복사.
    memcpy(arr, sorted, n * sizeof(int));
    
    free(temp);
    free(tree);
    free(sorted);
}

/* -------------------- Input Data Generation Functions --------------------*/

// Generates a sorted (ascending) array.
int* generate_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if(arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
    return arr;
}

// Generates a reverse sorted (descending) array.
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if(arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
    return arr;
}

// Generates a random array with integers between 0 and 9999.
int* generate_random_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if(arr == NULL) {
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
    if(arr == NULL) {
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

// Copies an array; returns a new array dynamically allocated as a copy of source.
int* copy_array(const int* source, int n) {
    int* copy = (int*) malloc(n * sizeof(int));
    if(copy == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}

/* -------------------- Experiment Function --------------------*/
// Runs the Tournament Sort algorithm on a given dataset 'iterations' times 
// and prints the average execution time.
// dataset_type: Description of the dataset.
// arr: the source array (unchanged for each run).
// n: size of the array.
// iterations: number of test runs for averaging.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    for (int i = 0; i < iterations; i++){
        int* data = copy_array(arr, n);
        start = clock();
        tournamentSort(data, n);
        end = clock();
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        total_time += time_taken;
        free(data);
    }
    printf("Dataset: %s, Size: %d, Average execution time (%d runs): %.6f seconds\n", 
           dataset_type, n, iterations, total_time / iterations);
}

/* -------------------- Main Function --------------------*/
int main() {
    srand(time(NULL));
    
    // Test input sizes.
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int iterations = 10;  // Each size is tested 10 times.
    
    for (int k = 0; k < numSizes; k++) {
        int n = sizes[k];
        printf("\n----- Testing with input size: %d -----\n", n);
        
        // Generate various datasets.
        int* sorted_array = generate_sorted_array(n);
        int* reverse_sorted_array = generate_reverse_sorted_array(n);
        int* random_array = generate_random_array(n);
        int* partially_sorted_array = generate_partially_sorted_array(n);
        
        // Run experiments.
        run_experiment("Sorted array (ascending)", sorted_array, n, iterations);
        run_experiment("Sorted array (descending)", reverse_sorted_array, n, iterations);
        run_experiment("Random array", random_array, n, iterations);
        run_experiment("Partially sorted array", partially_sorted_array, n, iterations);
        
        // Free allocated memory.
        free(sorted_array);
        free(reverse_sorted_array);
        free(random_array);
        free(partially_sorted_array);
    }
    
    return 0;
}