#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if (world_size != 2) {
        fprintf(stderr, "This program requires exactly 2 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int message;
    MPI_Request request;
    if (world_rank == 0) {
        message = 42;  // Process 0 sends a message
        MPI_Isend(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);  // Non-blocking send
        MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Blocking receive
        MPI_Wait(&request, MPI_STATUS_IGNORE);  // Wait for the non-blocking send to complete
    } else if (world_rank == 1) {
        message = 21;  // Process 1 sends a message
        MPI_Isend(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);  // Non-blocking send
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Blocking receive
        MPI_Wait(&request, MPI_STATUS_IGNORE);  // Wait for the non-blocking send to complete
    }

    printf("Process %d received message %d\n", world_rank, message);

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
