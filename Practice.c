#include<mpi.h>
#include<stdio.h>

int main(int argc,char** argv){
     MPI_Init(&argc,&argv);
     int world_rank;
     MPI_Comm_rank(MPI_COMM_WORLD,&world_rank);
     int world_size;
     MPI_Comm_size(MPI_COMM_WORLD,&world_size);
     
     if(world_size<2){
      fprintf(stderr,"World size must be greater than 1 for this demonstration\n");
     MPI_Abort(MPI_COMM_WORLD,1);
  
   }
    int send_data=world_rank;
    int recv_data;
    
    int left=(world_rank-1+world_size)%world_size;
    int right=(world_rank+1)%world_size;
    
    MPI_Send(&send_data,1,MPI_INT,right,0,MPI_COMM_WORLD);
    MPI_Recv(&recv_data,1,MPI_INT,left,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    printf("Process %d received data %d from process %d\n",world_rank,recv_data,left);

    MPI_Finalize();
    return 0;
}
