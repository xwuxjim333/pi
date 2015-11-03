#include <stdint.h>
#include <immintrin.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

#define M_PI 3.14159265358979323846

double compute_pi(size_t dt)
{
    double pi = 0.0;
    double delta = 1.0 / dt;
    register __m256d ymm0, ymm1, ymm2, ymm3, ymm4;
    
    ymm0 = _mm256_set1_pd(1.0);
    ymm1 = _mm256_set1_pd(delta);
    ymm2 = _mm256_set_pd(delta * 3, delta * 2, delta * 1, 0.0);
    ymm4 = _mm256_setzero_pd();
 
    for (int i = 0; i <= dt - 16; i += 16) {
        ymm3 = _mm256_set1_pd(i * delta);
        ymm3 = _mm256_add_pd(ymm3, ymm2);
        ymm3 = _mm256_fmadd_pd(ymm3, ymm3, ymm0);
        ymm3 = _mm256_div_pd(ymm1, ymm3);
        ymm4 = _mm256_add_pd(ymm4, ymm3);
    }

    double tmp[4] __attribute__((aligned(32)));
    _mm256_store_pd(tmp, ymm4);
    pi += tmp[0] + tmp[1] + tmp[2] + tmp[3];

    return pi * 4.0;
} 

void flops(int tds, size_t interval)
{
    double *sum = (double*)malloc(tds * sizeof(double));
    double start = omp_get_wtime();
    
#pragma omp parallel num_threads(tds)
    {
        double ret = compute_pi(interval*1000000);
        sum[omp_get_thread_num()] = ret;
    }
    
    double secs = omp_get_wtime() - start;
    FILE *ftime = fopen("avxflops_time.txt", "a");
    fprintf(ftime, "%zu %lf\n", interval , secs);
    fclose(ftime);

    double pi = 0;
    int c = 0;
    while (c < tds) {
        pi += sum[c++];
    }
    double diff = pi - M_PI > 0 ? pi - M_PI : M_PI - pi;
    double error = diff / M_PI;
    FILE *ferror = fopen("avxflops_error.txt", "a");
    fprintf(ferror, "%zu %.15lf\n", interval, error);
    fclose(ferror);

    free(sum);
}

int main(int argc, char* argv[])
{
    int interval = atoi(argv[1]);
	flops(omp_get_max_threads(), interval);
	return 0;
}
