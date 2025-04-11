#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define EPS 0.5  // ε 값: 여유 공간 비율 (필요에 따라 조정)

// -------------------- 함수 프로토타입 --------------------

// 입력 데이터 생성 함수 프로토타입
int* generate_sorted_array(int n);
int* generate_reverse_sorted_array(int n);
int* generate_random_array(int n);
int* generate_partially_sorted_array(int n);
int* copy_array(const int* source, int n);

// Library Sort 수정 함수 (논문 아이디어 반영)
void librarySort(int A[], int n);

// -------------------- Library Sort 수정 함수 --------------------

// 이 함수는 배열 A의 원소들을 라이브러리 정렬 방식(간격을 남기고 삽입 후, 라운드별 리밸런싱)을 이용해 정렬합니다.
// A 배열의 원소들은 삽입 순서에 따라 S에 배치되며, 최종적으로 S에서 정렬된 순서를 A에 복사합니다.
void librarySort(int A[], int n) {
    if(n <= 0) return;
    
    // 초기 정렬 배열 S의 크기는 (1+ε)n로 설정합니다.
    int cap = (int)ceil((1 + EPS) * n);
    int *S = (int*) malloc(cap * sizeof(int));
    int *occupied = (int*) calloc(cap, sizeof(int)); // 0: gap, 1: occupied
    if (!S || !occupied) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    
    // r: 현재까지 삽입된 원소 수.
    int r = 0;
    // 첫 원소는 S의 중앙에 삽입 (논문과 유사하게)
    int pos = cap / 2;
    S[pos] = A[0];
    occupied[pos] = 1;
    r = 1;
    
    // 각 라운드의 삽입 목표; 첫 라운드는 1개부터 시작합니다.
    int roundTarget = 1;
    
    // A 배열의 두 번째 원소부터 삽입 시작
    for (int i = 1; i < n; i++) {
        // 라운드 목표에 도달하면 리밸런싱 수행.
        if (r == roundTarget) {
            // 새로운 배열 크기는 (2+2ε)*r로 설정합니다.
            int newCap = (int)ceil((2 + 2 * EPS) * r);
            int *T = (int*) malloc(newCap * sizeof(int));
            int *occT = (int*) calloc(newCap, sizeof(int));
            if(!T || !occT){
                printf("Memory allocation error during rebalance!\n");
                exit(EXIT_FAILURE);
            }
            // S에 있는 r개의 원소를 균등하게 재분포하기 위한 간격을 계산.
            int spacing = newCap / r;
            int index = 0;
            for (int j = 0; j < cap && index < r; j++){
                if (occupied[j]) {
                    int newPos = index * spacing;
                    T[newPos] = S[j];
                    occT[newPos] = 1;
                    index++;
                }
            }
            free(S); free(occupied);
            S = T;
            occupied = occT;
            cap = newCap;
            // 다음 라운드 목표: 현재 r의 두 배, 단 n을 초과하지 않도록.
            roundTarget = (r * 2 <= n) ? r * 2 : n;
        }
        
        // A[i] 삽입 준비:
        // 현재 S에서 occupied된 인덱스를 배열 indices에 저장.
        int *indices = (int*) malloc(r * sizeof(int));
        if(!indices){
            printf("Memory allocation error!\n");
            exit(EXIT_FAILURE);
        }
        int count = 0;
        for (int j = 0; j < cap && count < r; j++){
            if (occupied[j])
                indices[count++] = j;
        }
        
        // 이진 검색으로 A[i] 삽입 위치(지원 위치)를 찾습니다.
        int lo = 0, hi = r - 1, posIndex = 0;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (S[indices[mid]] <= A[i]) {
                lo = mid + 1;
                posIndex = lo;
            } else {
                hi = mid - 1;
                posIndex = lo;
            }
        }
        // target 위치 결정: 
        int target;
        if (posIndex == 0) {
            target = indices[0] - 1;
            if (target < 0)
                target = indices[0] + 1;
        } else if (posIndex == r) {
            target = indices[r - 1] + 1;
            if (target >= cap)
                target = indices[r - 1] - 1;
        } else {
            target = (indices[posIndex - 1] + indices[posIndex]) / 2;
        }
        free(indices);
        
        // target 주변에서 빈 칸(gap)을 탐색 (먼저 오른쪽 탐색).
        int posInsert = target;
        while (posInsert < cap && occupied[posInsert])
            posInsert++;
        if (posInsert >= cap) {
            posInsert = target;
            while (posInsert >= 0 && occupied[posInsert])
                posInsert--;
            if (posInsert < 0)
                posInsert = target;
        }
        
        // A[i]를 S에 삽입.
        S[posInsert] = A[i];
        occupied[posInsert] = 1;
        r++;
    }
    
    // S 배열을 순회하면서 occupied된 위치의 원소를 정렬 결과로 A에 복사.
    int idx = 0;
    for (int j = 0; j < cap; j++){
        if (occupied[j]) {
            A[idx++] = S[j];
        }
    }
    
    free(S);
    free(occupied);
}

// -------------------- 입력 데이터 생성 함수들 --------------------

// 정렬된(오름차순) 배열을 생성합니다.
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

// 정렬된(내림차순) 배열을 생성합니다.
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;  // 내림차순.
    }
    return arr;
}

// 랜덤 값을 가진 배열(0~9999)를 생성합니다.
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

// 부분적으로 정렬된 배열 (앞 절반은 정렬, 뒤 절반은 랜덤)을 생성합니다.
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

// 소스 배열을 복사하여 새로운 배열을 반환합니다.
int* copy_array(const int* source, int n) {
    int* copy = (int*) malloc(n * sizeof(int));
    if (copy == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}

// -------------------- 실험 함수 --------------------
// 주어진 데이터셋에 대해 Library Sort를 iterations 회 실행하여 평균 실행 시간을 출력합니다.
void run_experiment(const char* dataset_type, int* arr, int n, int iterations) {
    clock_t start, end;
    double total_time = 0.0;
    for (int i = 0; i < iterations; i++){
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

// -------------------- Main 함수 --------------------
int main() {
    srand(time(NULL));
    
    // 다양한 입력 크기를 테스트 (1K, 5K, 10K, 50K, 100K, 500K, 1M)
    int sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);
    int iterations = 10;  // 각 입력 크기마다 10회 실행하여 평균 실행 시간 산출
    
    for (int k = 0; k < numSizes; k++) {
        int n = sizes[k];
        printf("\n----- Testing with input size: %d -----\n", n);
        
        // 각 입력 크기에 대해 다양한 데이터셋 생성
        int* sorted_array = generate_sorted_array(n);
        int* reverse_sorted_array = generate_reverse_sorted_array(n);
        int* random_array = generate_random_array(n);
        int* partially_sorted_array = generate_partially_sorted_array(n);
        
        // 각 데이터셋에 대해 Library Sort 수행 및 평균 시간 측정
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