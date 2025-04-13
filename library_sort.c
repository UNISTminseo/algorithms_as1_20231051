#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define EPSILON 0.5  // 추가 공간 비율 (ε)

// 함수 프로토타입
void librarySort(int *A, int n);
int* generate_sorted_array(int n);
int* generate_reverse_sorted_array(int n);
int* generate_random_array(int n);
int* generate_partially_sorted_array(int n);
void run_experiment(const char *description, int *array, int n, int iterations);
int is_sorted(int *A, int n);

// 로컬 리밸런스: S에 삽입된 count개의 원소들을 전체 capacity 범위 내에서 균등 분포 시킴
void local_rebalance(int *S, int *occupied, int *pos, int count, int capacity) {
    // 전체 capacity를 span으로 사용
    int new_span = capacity;
    int gap = new_span / count;
    if (gap < 1) gap = 1;
    int *newS = (int *)malloc(sizeof(int) * capacity);
    int *newOccupied = (int *)calloc(capacity, sizeof(int));
    
    // pos에 저장된 순서대로 재배치: 인덱스 i에 대해
    for (int i = 0; i < count; i++) {
        int new_position = i * gap + gap / 2;
        if (new_position >= capacity)
            new_position = capacity - 1;
        newS[new_position] = S[pos[i]];
        newOccupied[new_position] = 1;
        pos[i] = new_position;
    }
    // 재배치 결과를 기존 배열에 복사
    for (int i = 0; i < capacity; i++) {
        S[i] = newS[i];
        occupied[i] = newOccupied[i];
    }
    free(newS);
    free(newOccupied);
}

// insert_element: 새 원소를 S에 삽입하는 함수
// S: 정렬 배열, occupied: S의 사용 여부, pos: S 내에서 정렬되어 저장된 원소들의 인덱스를 오름차순으로 저장
// count: 현재 S에 삽입된 원소의 개수, ins_index: pos 배열에서 새 원소가 들어갈 위치 (0 ~ count)
// value: 새로 삽입할 값, capacity: S의 크기
void insert_element(int *S, int *occupied, int *pos, int *count, int ins_index, int value, int capacity) {
    int p;
    int Lpos = (ins_index == 0 ? -1 : pos[ins_index - 1]);
    int Rpos = (ins_index == *count ? capacity : pos[ins_index]);
    int gap = Rpos - Lpos - 1;
    
    if (gap > 0) {
        // 빈 공간이 존재하면, 왼쪽 위치부터 gap 중간으로 선택
        if (ins_index == 0) {
            p = Lpos + 1;  // 일반적으로 0
        } else if (ins_index == *count) {
            p = pos[*count - 1] + 1;
        } else {
            p = Lpos + gap/2 + 1;
        }
        S[p] = value;
        occupied[p] = 1;
    } else {
        // 빈 공간이 없으므로, 두 방향으로의 shift 가능 여부 확인
        int canShiftLeft = (ins_index > 0 && pos[0] > 0);
        int canShiftRight = (*count > ins_index && pos[*count - 1] < capacity - 1);
        
        // 만약 양쪽 모두 이동 불가능하다면, 로컬 리밸런스를 수행한 후 다시 시도한다.
        if (!canShiftLeft && !canShiftRight) {
            local_rebalance(S, occupied, pos, *count, capacity);
            // 재배치 후 gap가 생겼을 것이므로 재귀적으로 삽입을 시도
            insert_element(S, occupied, pos, count, ins_index, value, capacity);
            return;
        }
        
        // 이동 가능한 방향을 선택 (이동 비용이 적은 쪽 선택)
        int costLeft = ins_index;              // 왼쪽 이동 비용: ins_index개의 원소 이동
        int costRight = (*count - ins_index);    // 오른쪽 이동 비용
        if ((canShiftRight && (!canShiftLeft || costRight <= costLeft))) {
            // 오른쪽 블록 shift: ins_index부터 count-1까지 오른쪽으로 한 칸씩 이동
            for (int i = *count - 1; i >= ins_index; i--) {
                int oldPos = pos[i];
                int newPos = oldPos + 1;
                if (newPos >= capacity) {
                    fprintf(stderr, "Error: Cannot shift right, capacity exceeded.\n");
                    exit(EXIT_FAILURE);
                }
                S[newPos] = S[oldPos];
                occupied[newPos] = 1;
                occupied[oldPos] = 0;
                pos[i] = newPos;
            }
            p = pos[ins_index] - 1;
            S[p] = value;
            occupied[p] = 1;
        } else if (canShiftLeft) {
            // 왼쪽 블록 shift: 0부터 ins_index-1까지 왼쪽으로 한 칸씩 이동
            for (int i = 0; i < ins_index; i++) {
                int oldPos = pos[i];
                int newPos = oldPos - 1;
                if (newPos < 0) {
                    fprintf(stderr, "Error: Cannot shift left, index below 0.\n");
                    exit(EXIT_FAILURE);
                }
                S[newPos] = S[oldPos];
                occupied[newPos] = 1;
                occupied[oldPos] = 0;
                pos[i] = newPos;
            }
            p = (ins_index == 0 ? 0 : pos[ins_index - 1] + 1);
            S[p] = value;
            occupied[p] = 1;
        } else {
            // 이 경우는 위에서 이미 처리했으므로 도달하지 않음.
            fprintf(stderr, "Unexpected error in insert_element\n");
            exit(EXIT_FAILURE);
        }
    }
    // pos 배열에 새 원소의 위치를 삽입 순서를 유지하도록 추가
    for (int i = *count; i > ins_index; i--) {
        pos[i] = pos[i - 1];
    }
    pos[ins_index] = p;
    (*count)++;
}

// Library Sort 구현 (A: 입력 배열, n: 원소 수)
void librarySort(int *A, int n) {
    double epsilon = EPSILON;
    // S 배열의 크기는 (1+ε)*n + 1
    int capacity = (int)((1.0 + epsilon) * n) + 1;
    int *S = (int *)malloc(sizeof(int) * capacity);
    int *occupied = (int *)malloc(sizeof(int) * capacity);
    for (int i = 0; i < capacity; i++)
        occupied[i] = 0;
    
    // pos 배열: S 내 정렬된 원소들의 위치 (오름차순)
    int *pos = (int *)malloc(sizeof(int) * n);
    int count = 0;         // 현재 S에 삽입된 원소 수
    int nextRound = 1;     // 라운드 기준 (삽입된 개수가 nextRound*2가 되면 재배치)
    
    // 첫 번째 원소는 S 중앙에 삽입
    int start_pos = capacity / 2;
    S[start_pos] = A[0];
    occupied[start_pos] = 1;
    pos[0] = start_pos;
    count = 1;
    
    // 두 번째 원소부터 순차적으로 삽입
    for (int i = 1; i < n; i++) {
        // binary search: pos 배열에 있는 S의 원소들과 비교하여 삽입 순서를 결정
        int low = 0, high = count - 1;
        int ins_index = 0;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (A[i] < S[pos[mid]])
                high = mid - 1;
            else
                low = mid + 1;
        }
        ins_index = low;
        
        // gap이 없으면 insert_element에서 로컬 리밸런스를 수행하게 됨
        insert_element(S, occupied, pos, &count, ins_index, A[i], capacity);
        
        // 라운드 조건에 도달하면 전체 재배치(rebalance)
        if (count == nextRound * 2) {
            int new_span = (int)((2.0 + 2.0 * epsilon) * count);
            if (new_span > capacity)
                new_span = capacity;
            int gap = new_span / count;
            int *newS = (int *)malloc(sizeof(int) * capacity);
            int *newOccupied = (int *)malloc(sizeof(int) * capacity);
            for (int j = 0; j < capacity; j++)
                newOccupied[j] = 0;
            for (int j = 0; j < count; j++) {
                int new_position = j * gap + gap / 2;
                if (new_position >= capacity)
                    new_position = capacity - 1;
                newS[new_position] = S[pos[j]];
                newOccupied[new_position] = 1;
                pos[j] = new_position;
            }
            for (int j = 0; j < capacity; j++) {
                S[j] = newS[j];
                occupied[j] = newOccupied[j];
            }
            free(newS);
            free(newOccupied);
            nextRound *= 2;
        }
    }
    
    // 최종 정렬 결과를 pos 배열 순서대로 A에 복사
    for (int j = 0; j < n; j++) {
        A[j] = S[pos[j]];
    }
    
    free(S);
    free(occupied);
    free(pos);
}

// -------------------- 데이터셋 생성 함수 --------------------

// 오름차순 정렬된 배열 생성
int* generate_sorted_array(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        arr[i] = i;
    return arr;
}

// 내림차순 정렬된 배열 생성
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        arr[i] = n - i;
    return arr;
}

// 난수 배열 생성
int* generate_random_array(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        arr[i] = rand() % (n * 10);
    return arr;
}

// 부분 정렬 배열 생성 (오름차순 배열에서 일부 원소 swap)
int* generate_partially_sorted_array(int n) {
    int* arr = generate_sorted_array(n);
    int swaps = n / 20;  // 약 5% swap
    for (int i = 0; i < swaps; i++) {
        int idx1 = rand() % n;
        int idx2 = rand() % n;
        int temp = arr[idx1];
        arr[idx1] = arr[idx2];
        arr[idx2] = temp;
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

// -------------------- 실험 실행 함수 --------------------

// 정렬 여부 확인: 올바르게 정렬되었으면 1, 아니면 0 반환
int is_sorted(int *A, int n) {
    for (int i = 1; i < n; i++) {
        if (A[i-1] > A[i])
            return 0;
    }
    return 1;
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

// -------------------- main() --------------------
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