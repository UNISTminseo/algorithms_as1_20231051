#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>  // For INT_MAX

// Compute the next power of 2 greater than or equal to n.
int nextPowerOf2(int n) {
    int p = 1;
    while(p < n) p *= 2;
    return p;
}

// Tournament Sort implementation.
// This function sorts the array 'arr' of length 'n' in ascending order.
void tournamentSort(int arr[], int n) {
    // Determine the required number of leaves.
    int k = nextPowerOf2(n);
    
    // Create a temporary array 'temp' to store input values and pad with INT_MAX.
    int *temp = (int*) malloc(k * sizeof(int));
    if (temp == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        temp[i] = arr[i];
    }
    for (int i = n; i < k; i++) {
        temp[i] = INT_MAX;
    }
    
    // Allocate the tournament tree as an array of size 2*k.
    // The tree will be represented in a typical heap style (1-indexed), 
    // but we use 0-indexed C arrays by considering leaves at indices [k, 2*k-1].
    int treeSize = 2 * k;
    int *tree = (int*) malloc(treeSize * sizeof(int));
    if (tree == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize leaves: for each i from 0 to k-1, 
    // the corresponding leaf node is at index (i + k) and stores i.
    for (int i = 0; i < k; i++) {
        tree[k + i] = i;
    }
    
    // Build the tournament tree in a bottom-up fashion.
    for (int i = k - 1; i >= 1; i--) {
        int left = tree[2 * i];
        int right = tree[2 * i + 1];
        tree[i] = (temp[left] <= temp[right]) ? left : right;
    }
    
    // Allocate an array to store the sorted output.
    int *sorted = (int*) malloc(n * sizeof(int));
    if (sorted == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    
    // Repeatedly extract the minimum element from the tournament tree.
    for (int i = 0; i < n; i++) {
        int winner = tree[1];  // The index in 'temp' holding the smallest element.
        sorted[i] = temp[winner];
        
        // Set the winner's value to INT_MAX so it will not be chosen again.
        temp[winner] = INT_MAX;
        
        // Update the tournament tree from the leaf corresponding to the winner up to the root.
        int j = winner + k;  // Leaf index in the tree.
        tree[j] = winner;    // (Leaf nodes remain unchanged in terms of stored index.)
        for (j /= 2; j >= 1; j /= 2) {
            int left = tree[2 * j];
            int right = tree[2 * j + 1];
            tree[j] = (temp[left] <= temp[right]) ? left : right;
        }
    }
    
    // Copy the sorted output back to the original array.
    memcpy(arr, sorted, n * sizeof(int));
    
    free(temp);
    free(tree);
    free(sorted);
}

/* --- Input Data Generation Functions --- */

// Generates a sorted (ascending) array of size n.
int* generate_sorted_array(int n) {
    int *arr = (int*) malloc(n * sizeof(int));
    if(arr == NULL){
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < n; i++){
        arr[i] = i;
    }
    return arr;
}

// Generates a sorted (descending) array of size n.
// This serves as the "reverse sorted" case.
int* generate_reverse_sorted_array(int n) {
    int *arr = (int*) malloc(n * sizeof(int));
    if(arr == NULL){
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++){
        arr[i] = n - i;  // Descending order.
    }
    return arr;
}

// Generates an array of size n with random integers between 0 and 9999.
int* generate_random_array(int n) {
    int *arr = (int*) malloc(n * sizeof(int));
    if(arr == NULL){
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++){
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Generates a partially sorted array of size n: 
// first half sorted (ascending) and second half containing random integers.
int* generate_partially_sorted_array(int n) {
    int *arr = (int*) malloc(n * sizeof(int));
    if(arr == NULL){
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    int mid = n / 2;
    for (int i = 0; i < mid; i++){
        arr[i] = i;
    }
    for (int i = mid; i < n; i++){
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Creates and returns a copy of the source array of size n.
int* copy_array(const int* source, int n) {
    int *copy = (int*) malloc(n * sizeof(int));
    if(copy == NULL){
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}

/* --- Experiment Function --- */
// Runs the Tournament Sort algorithm on the provided dataset for a given number of iterations,
// and prints the average execution time.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    for (int i = 0; i < iterations; i++){
        int *data = copy_array(arr, n);
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