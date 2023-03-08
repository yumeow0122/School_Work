#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

#include <errno.h>
#include <pthread.h>

#define MAX 500
#define TEST_CASE 1000
#define THREAD_NUM 4

double matrix1[MAX][MAX], matrix2[MAX][MAX], matrix3[MAX][MAX];
int raw, col;

void *multiply(void *arg)
{
   int id = *((int *)arg);
   int st = id * col / THREAD_NUM;
   int ed = (id + 1) * col / THREAD_NUM - 1;
   int i, j, k;
   for (i = st; i <= ed; i++)
   {
      for (j = 0; j < col; j++)
      {
         double sum = 0;
         for (k = 0; k < raw; k++)
         {
            sum += matrix1[i][k] * matrix2[k][j];
         }
         matrix3[i][j] = sum;
      }
   }
   return NULL;
}

int main()
{
   int i = 0, j = 0, k = 0, x = 0;
   float val;

   char infilename[20], outfilename[20];
   FILE *infile, *outfile;

   struct timeval tp_s, tp_e;
   struct timezone tzp_s, tzp_e;

   for (i = 0; i < raw; i++)
   {
      for (j = 0; j < col; j++)
         matrix1[1][1] = matrix2[1][1] = matrix3[1][1] = 0;
   }

   gettimeofday(&tp_s, &tzp_s);
   for (x = 0; x < 1; x++)
   {
      sprintf(infilename, "./input/in%d.txt", x + 1);
      sprintf(outfilename, "./thread/out%d.txt", x + 1);
      //sprintf(infilename, "./test.in");
      //sprintf(outfilename, "./test.out");

      if ((infile = fopen(infilename, "r")) == NULL)
         perror("in open error~\n");

      fscanf(infile, "%d", &raw);
      fscanf(infile, "%d", &col);

      printf("%d\n", x);
      for (i = 0; i < raw; i++) // set matrix value
      {
         for (j = 0; j < col; j++)
         {
            fscanf(infile, "%f", &val);
            matrix1[i][j] = matrix2[i][j] = val;
         }
      }

      pthread_t *threads;
      threads = (pthread_t *) malloc(THREAD_NUM * sizeof(pthread_t));

      for (int i = 0; i < THREAD_NUM; i++) {
         int *p = (int *) malloc(sizeof(int));
         *p = i;
         pthread_create(&threads[i], NULL, multiply, (void *)(p));
      }
      for (i = 0; i < THREAD_NUM; i++) {
         pthread_join(threads[i], NULL);
      }

      if ((outfile = fopen(outfilename, "w")) == NULL)
         perror("out open error~\n");

      fprintf(outfile, "%d\n%d\n", raw, col);
      for (i = 0; i < raw; i++)
      {
         for (j = 0; j < col; j++, val++)
            fprintf(outfile, "%f\n", matrix3[i][j]);
      }

      fclose(infile);
      fclose(outfile);
   }

   gettimeofday(&tp_e, &tzp_e);
   if ((outfile = fopen("threadTime.txt", "w")) == NULL)
      perror("out open error~\n");

   fprintf(outfile, "Total execution time =%ld\n", tp_e.tv_sec - tp_s.tv_sec);
   fclose(outfile);
}
