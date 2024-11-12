#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, size;
    
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    
    // Get the rank of the process (its ID)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size < 2) {
        printf("This program requires at least 2 processes.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        return 1;
    }

    int send_data = rank;  // Each process will send its rank
    int recv_data;

    if (rank == 0) {
        // Process 0 is sending first, then receiving
        printf("Process 0 attempting to send...\n");
        MPI_Ssend(&send_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 attempting to receive...\n");
        MPI_Recv(&recv_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received data %d from process 1\n", recv_data);
    } else if (rank == 1) {
        // Process 1 is sending first, then receiving (this causes the deadlock)
        printf("Process 1 attempting to send...\n");
        MPI_Ssend(&send_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Process 1 attempting to receive...\n");
        MPI_Recv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data %d from process 0\n", recv_data);
    }

    // This message will not be reached if there's a deadlock
    printf("Both processes were able to proceed, no deadlock occurred.\n");

    // Finalize the MPI environment
    MPI_Finalize();

    // If a deadlock occurs, print a message after MPI_Finalize
    if (rank == 0 || rank == 1) {
        printf("Deadlock detected: None of the processes were able to proceed.\n");
    }

    return 0;
}

