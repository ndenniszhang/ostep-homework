#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>

#define ITERATIONS 10000000

int main() {
    struct timespec start, end;
    long long elapsed_ns;

    // Start the timer
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Run the system call 10 million times
    for (int i = 0; i < ITERATIONS; i++) {
        // We use syscall(SYS_getpid) instead of getpid() directly
        // to bypass glibc's vDSO optimizations and force a real kernel context switch.
        syscall(SYS_getpid); 
    }

    // Stop the timer
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate total time in nanoseconds
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000LL + 
                 (end.tv_nsec - start.tv_nsec);

    // Calculate the average time per system call
    double avg_ns = (double)elapsed_ns / ITERATIONS;

    printf("Total iterations: %d\n", ITERATIONS);
    printf("Total time: %f seconds\n", (double)elapsed_ns / 1e9);
    printf("Average time per system call: %.2f nanoseconds\n", avg_ns);

    return 0;
}