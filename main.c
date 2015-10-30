#include <stdint.h>
#include <immintrin.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define M_PI 3.14159265358979323846
#define n 128000000
#define SAMPLE 5

double compute_pi_baseline(size_t dt)
{
    double pi = 0.0;
    double delta = 1.0 / dt;
    for (size_t i = 0; i < dt; i++) {
        double x = (double) i / dt;
        pi += delta / (1.0 + x * x);
    }

    return pi * 4.0;
}

double compute_pi_avx_simd(size_t dt)
{
    double pi = 0.0;
    double delta = 1.0 / dt;
    register __m256d ymm0, ymm1, ymm2, ymm3, ymm4;
                             
    ymm0 = _mm256_set1_pd(1.0);
    ymm1 = _mm256_set1_pd(delta);
    ymm2 = _mm256_set_pd(delta * 3, delta * 2, delta * 1, 0.0);
    ymm4 = _mm256_setzero_pd();

    for (int i = 0; i <= dt - 4; i += 4) {
        ymm3 = _mm256_set1_pd(i * delta);
        ymm3 = _mm256_add_pd(ymm3, ymm2);
        ymm3 = _mm256_mul_pd(ymm3, ymm3);
        ymm3 = _mm256_add_pd(ymm0, ymm3);
        ymm3 = _mm256_div_pd(ymm1, ymm3);
        ymm4 = _mm256_add_pd(ymm4, ymm3);
    }

    double tmp[4] __attribute__((aligned(32)));
    _mm256_store_pd(tmp, ymm4);
    pi += tmp[0] + tmp[1] + tmp[2] + tmp[3];

    return pi * 4.0;
}

double compute_mean(double t[SAMPLE])
{
    double mean = 0.0;

//calculate mean value
    for (int i = 0; i < SAMPLE; i++) {
        mean = mean+t[i];
    }
    mean = mean/SAMPLE;

    return mean;
}

int main(int argc, char* argv[])
{
    int method = atoi(argv[1]);

    clock_t start, end;
    double time[SAMPLE];

    double (*compute_pi)(size_t);
    char file_time[32];
    char file_error[32];

    switch(method) {
        case 0:
            compute_pi = &compute_pi_baseline;
            strcpy(file_time, "baseline_time.txt");
            strcpy(file_error, "baseline_error.txt");
        default:
            break;
    }

    for (int i = 0; i < SAMPLE; i++) {
        start = clock();
        compute_pi(n);
        end = clock();
        time[SAMPLE] = (double)(end - start) / CLOCKS_PER_SEC;
    }
    double mean_time = compute_mean(time);

    double pi = 0.0;
    pi = compute_pi(n);
	double diff = pi - M_PI > 0 ? pi - M_PI : M_PI - pi;
	double error = diff / M_PI;

    FILE *ftime = fopen(file_time, "a");
	fprintf(ftime, "%lf\n", mean_time);
    fclose(ftime);

    FILE *ferror = fopen(file_error, "a");
	fprintf(ferror, "%.15lf\n", error);
    fclose(ferror);
    
    return 0;
}
