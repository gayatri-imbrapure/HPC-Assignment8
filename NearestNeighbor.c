#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Ensure at least 2 processes for nearest neighbor exchange
    if (world_size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Initialize data to send
    int send_data = world_rank;
    int recv_data;

    // Determine neighbors in a ring topology
    int left_neighbor = (world_rank - 1 + world_size) % world_size;
    int right_neighbor = (world_rank + 1) % world_size;

    // Blocking send to the right neighbor, receive from the left neighbor
    MPI_Send(&send_data, 1, MPI_INT, right_neighbor, 0, MPI_COMM_WORLD);
    MPI_Recv(&recv_data, 1, MPI_INT, left_neighbor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("Process %d received data %d from process %d\n", world_rank, recv_data, left_neighbor);

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}
