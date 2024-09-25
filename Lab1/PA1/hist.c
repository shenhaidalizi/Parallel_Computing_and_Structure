#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


void serial_histogram(float *array, int n, int *bins, int num_bins)
{
    int i;
    /* Initialize the bins as zero */
    for (i = 0; i < num_bins; i++) {
        bins[i] = 0; 
    }
    /* Counting */
    int idx;
    for (i = 0; i < n; i++) {
        int val = (int)array[i];
        if (val == num_bins) { /* Ensure 10 numbers go to the last bin */
            idx = num_bins - 1;
        } else {
            idx = val % num_bins;
        }
        bins[idx]++;
    }
    for(i = 0; i < num_bins; i++){
        printf("%d,", bins[i]);
    }
}

void parallel_histogram(float *array, int n, int *bins, int num_bins, int thread_count) {
    int i;
    
    /* Initialize bins to zero */
    
    for (i = 0; i < num_bins; i++) {
        bins[i] = 0;
    }

    /* Thread-private bins to avoid race conditions */
    int idx;

    /* Parallel counting */
    #pragma omp parallel for num_threads(thread_count) reduction(+:bins[:num_bins]) private(idx)
    for (i = 0; i < n; i++) {
        int val = (int)array[i];
        idx = (val == num_bins) ? num_bins - 1 : val % num_bins;
        bins[idx]++;
    }

}


void generate_random_numbers(float *array, int n) 
{
    int i;
    float a = 10.0;
    for(i=0; i<n; ++i)
        array[i] = ((float)rand()/(float)(RAND_MAX)) * a;
}

int main(int argc, char* argv[])
{    
     if (argc < 2) {
        printf("Usage: %s <number_of_elements>\n", argv[0]);
        return -1;
    }
    
    int n = strtol(argv[1], NULL, 10);  // Read 'n' from user input
    int thread_count = strtol(argv[2], NULL, 10);

    int num_bins = 10;
    float *array;
    int *bins_serial, *bins_parallel;
    
    array = (float *)malloc(sizeof(float) * n);
    bins_serial = (int *)malloc(sizeof(int) * num_bins);
    bins_parallel = (int *)malloc(sizeof(int) * num_bins);

    // Generate random numbers
    generate_random_numbers(array, n);

    // Serial Histogram
    double start = omp_get_wtime();
    serial_histogram(array, n, bins_serial, num_bins);
    double end = omp_get_wtime();
    printf("%f,", end - start);

    // Parallel Histogram
    start = omp_get_wtime();
    parallel_histogram(array, n, bins_parallel, num_bins, thread_count);
    end = omp_get_wtime();
    printf("%f", end - start);

    // Free allocated memory
    free(array);
    free(bins_serial);
    free(bins_parallel);

    return 0;
}

