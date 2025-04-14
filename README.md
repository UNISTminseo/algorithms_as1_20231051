**|EVERY CODE HAS ITS OWN DATA GENERATION FUNCTIONS, AND IT LOOKS SIMILAR TO THE CODE BELOW|**

/* --- Input Data Generation Functions --- */

// Generates a sorted (ascending) array - Sorted Data.
int* generate_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = i;  // Sorted in ascending order: 0 to n-1
    }
    return arr;
}

// Generates a reverse sorted (descending) array - Reverse Sorted Data.
int* generate_reverse_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;  // Descending order: n to 1
    }
    return arr;
}

// Generates an array with random numbers (range 0 to 9999) - Random Data.
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

// Generates a partially sorted array (first half sorted, second half random) - Partially Sorted Data.
int* generate_partially_sorted_array(int n) {
    int* arr = (int*) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    int mid = n / 2;
    // First half is sorted in ascending order.
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    // Second half contains random numbers.
    for (int i = mid; i < n; i++) {
        arr[i] = rand() % 10000;
    }
    return arr;
}

// Copies an array. Used so the original dataset remains unchanged for each run.
int* copy_array(const int* source, int n) {
    int* copy = (int*) malloc(n * sizeof(int));
    if (copy == NULL) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    memcpy(copy, source, n * sizeof(int));
    return copy;
}
