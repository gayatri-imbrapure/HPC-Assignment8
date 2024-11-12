#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Check if there are exactly two processes
    if (world_size != 2) {
        fprintf(stderr, "This program requires exactly two processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int n = 8; // Size of the array (must be even)
    int A[n];

    // Initialize the array only in the root process (P0)
    if (world_rank == 0) {
        for (int i = 0; i < n; i++) {
            A[i] = i + 1; // Example array: [1, 2, 3, 4, 5, 6, 7, 8]
        }
    }

    // Broadcast the array to both processes
    MPI_Bcast(A, n, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate local sums
    int local_sum = 0;

    // Each process computes the sum of its part
    if (world_rank == 0) {
        printf("Process P0 is summing the first half:\n");
        // First half for process P0
        for (int i = 0; i < n / 2; i++) {
            local_sum += A[i];
            printf("P0: Adding A[%d] = %d, local_sum = %d\n", i, A[i], local_sum);
        }
    } else if (world_rank == 1) {
        printf("Process P1 is summing the second half:\n");
        // Second half for process P1
        for (int i = n / 2; i < n; i++) {
            local_sum += A[i];
            printf("P1: Adding A[%d] = %d, local_sum = %d\n", i, A[i], local_sum);
        }
    }

    // Reduce the local sums to the root process
    int total_sum;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Only root process (P0) prints the final result
    if (world_rank == 0) {
        printf("The total sum of all elements in the array is: %d\n", total_sum);
    }

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}

