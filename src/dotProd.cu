#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BLOCK_SIZE 1024

__global__ void cu_dotProd (long *array, long arraySize)
{
    long x = blockIdx.x * BLOCK_SIZE + threadIdx.x;

    if (x < arraySize){
      long f = 0;
      long d = 0;

      if (x < arraySize/2){
         f = x + 1;
      } else {
        f = arraySize - x;
      }

      if (x%10 == 0) {
        d = 1;
      } else {
        d = (x%10) + 1;
      }
      array[x] = f * d;
    }
    //__syncthreads();
}

extern "C" void dotProduct(long *arr, long arraySize)
{
  long *arrayR_d;
	cudaError_t result;

	// allocate space in the device for arrayR_d
	result = cudaMalloc ((void**) &arrayR_d, sizeof(long) * arraySize);
	if (result != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed.");
		exit(1);
	}

  // printf ("dimgrid:\n");
  // printf ("%d %d %f ", arraySize, BLOCK_SIZE, ceil((float)arraySize/BLOCK_SIZE));
  // printf ("\n");

	// set execution configuration
	dim3 dimblock (BLOCK_SIZE);
	//dim3 dimgrid (arraySize/BLOCK_SIZE);
  dim3 dimgrid(ceil((float)arraySize/BLOCK_SIZE));

	// actual computation: Call the kernel
	cu_dotProd <<<dimgrid, dimblock>>> (arrayR_d, arraySize);

	// transfer result back to host
	result = cudaMemcpy (arr, arrayR_d, sizeof(long) * arraySize, cudaMemcpyDeviceToHost);
	if (result != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed.");
		exit(1);
	}

 //  printf ("Final state of the array arrayR_d:\n");
 //  int j;
 //  for (j=0; j < arraySize; j++) {
 //     //printf ("%d ", j);
 //     printf ("%d ", arr[j]);
 //  }
 // printf ("\n");

	result = cudaFree (arrayR_d);
	if (result != cudaSuccess) {
		fprintf(stderr, "cudaFree failed.");
		exit(1);
	}
}
