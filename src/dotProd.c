#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZEOFARRAY 28000000

long arrayR[SIZEOFARRAY];

struct timeval startSerial, finishSerial, startParallel, finishParallel;

// The function fillArray is in the file simple.cu
extern void dotProduct(long *arr, long size);

long calculateDotProd_serial(long size){
  long i;
  long dotProdSerial = 0;

  //long arrayF[size];
  //long arrayD[size];

  // If n is the size of the vector
  // let the Force vector range from 1..n/2 and back down to 1.
  // Let the distance vector repeat distance cycles from 1..10 mm.
  // Creation of the vectors was inpired by p. wolffe's code :)
  for (i=0; i<size; i++){
    long f = 0;
    long d = 0;

    if (i < size/2){
       f = i + 1;
    } else {
      f = size - i;
    }
    //arrayF[i] = f;

    if (i%10 == 0) {
      d = 1;
    } else {
      d = (i%10) + 1;
    }
    //arrayD[i] = d;
    dotProdSerial += f * d;
  }

  // Print the initial array
  // printf ("Initial state of the array F:\n");
  // for (i=0; i < size; i++) {
  //    printf ("%d ", arrayF[i]);
  // }
  // printf ("\n");
  //
  // printf ("Initial state of the array D:\n");
  // for (i=0; i < size; i++) {
  //    printf ("%d ", arrayD[i]);
  // }
  // printf ("\n");
  return dotProdSerial;
}

void printTimeResults() {
  printf("\n\nTime Results:\n");
  printf("Serial \t%ld microseconds\n",
          ((finishSerial.tv_sec - startSerial.tv_sec)*1000000L
          +finishSerial.tv_usec) - startSerial.tv_usec);
  printf("GPU \t%ld microseconds\n",
          ((finishParallel.tv_sec - finishParallel.tv_sec)*1000000L
          +finishParallel.tv_usec) - startParallel.tv_usec);
}

int main (int argc, char *argv[])
{
  long resultSerial = 0;
  long resultParallel = 0;

  long i;
  // for (i=0; i < SIZEOFARRAY; i++) {
  //    arrayR[i]=0;
  // }

  printf ("Array Size:\n");
  printf ("%u ", SIZEOFARRAY);
  printf ("\n");

   //Call the function that runs serial
   printf ("Start timer - Serial version:\n");
   gettimeofday (&startSerial, NULL);
   resultSerial = calculateDotProd_serial(SIZEOFARRAY);
   gettimeofday (&finishSerial, NULL);

   //print result of serial processing
   printf ("Final result of dot product - Serial version:\n");
   printf ("%u ", resultSerial);
   printf ("\n");

   // Call the function that runs parallel
   printf ("Start timer - parallel version:\n");
   gettimeofday (&startParallel, NULL);
   dotProduct(arrayR, SIZEOFARRAY);

   //Do reduction of arrayR
   for (i=0; i < SIZEOFARRAY; i++) {
     resultParallel += arrayR[i];
   }
   gettimeofday (&finishParallel, NULL);

   // Print the array
  //  printf ("Final state of the array arrayR:\n");
  //  for (i=0; i < SIZEOFARRAY; i++) {
  //     printf ("%d ", arrayR[i]);
  //  }
  // printf ("\n");

   //print result of parallel processing
   printf ("Final result of dot product:\n");
   printf ("%u ", resultParallel);
   printf ("\n");

   printTimeResults();

   return 0;
}
