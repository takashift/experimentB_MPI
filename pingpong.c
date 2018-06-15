#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1024*1024*6
#define ITER 100

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	MPI_Status status;
	int tag = 0;
	int comm_data_num = 32;

	int dest_rank, source_rank;

	int ret, array_size, i, j;

	double start, end, time;

	if(world_rank == 0) {
		dest_rank = 1;
		source_rank = 1;
	}
	if(world_rank == 1) {
		dest_rank = 0;
		source_rank = 0;
	}

	if(world_rank == 0)
		printf("int_size = %d\n", sizeof(int));

	while(comm_data_num < MAX_SIZE) {

		comm_data_num *= 2;
		//int *recv_data = malloc(sizeof(int)*comm_data_num);
		//int *send_data = malloc(sizeof(int)*comm_data_num);
		int *recv_data = calloc(comm_data_num, sizeof(int));
		int *send_data = calloc(comm_data_num, sizeof(int));
		
		if(world_rank == 0) {
			for(i = 0; i < comm_data_num; i++)
				send_data[i] = i;
		}

		for(i=0; i<10; i++){
			if(world_rank == 0)
			{
				//send_data[0] = 2018;
				//printf("%d\n", comm_data_num);
				MPI_Send(send_data, comm_data_num, MPI_INT, dest_rank, 2, MPI_COMM_WORLD);
				MPI_Recv(recv_data, comm_data_num, MPI_INT, source_rank, 3, MPI_COMM_WORLD, &status);
			}
			else if(world_rank == 1) 
			{
				//send_data[0] = 20672;
				MPI_Recv(recv_data, comm_data_num, MPI_INT, source_rank, 2, MPI_COMM_WORLD, &status);
				MPI_Send(send_data, comm_data_num, MPI_INT, dest_rank, 3, MPI_COMM_WORLD);
			}
			// ここでバリアするのは無駄だしおかしい。
		}
		MPI_Barrier(MPI_COMM_WORLD);

		start = MPI_Wtime();
		for(j = 0; j < ITER; j++) {

			if(world_rank == 0)
			{
				//send_data[0] = 2018;
				//printf("%d\n", comm_data_num);
				MPI_Send(send_data, comm_data_num, MPI_INT, dest_rank, 2, MPI_COMM_WORLD);
				MPI_Recv(recv_data, comm_data_num, MPI_INT, source_rank, 3, MPI_COMM_WORLD, &status);
			}
			else if(world_rank == 1) 
			{
				//send_data[0] = 20672;
				MPI_Recv(recv_data, comm_data_num, MPI_INT, source_rank, 2, MPI_COMM_WORLD, &status);
				MPI_Send(send_data, comm_data_num, MPI_INT, dest_rank, 3, MPI_COMM_WORLD);
			}
			// ここでバリアするのは無駄だしおかしい。
		}
		MPI_Barrier(MPI_COMM_WORLD);
		end = MPI_Wtime();
		// 2つのメッセージパッシングを行っているので2で割る。
		time = (end - start)/ITER/2;
		if(world_rank == 0) {
			int size = comm_data_num * sizeof(int);
			double band = size / time;
			printf("size = %d time = %lf band = %lf\n", size, time, band);
		}
		free(send_data);
		free(recv_data);
	}

	MPI_Finalize();
	return 0;
}
