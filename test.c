#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


int main()
{
   close(2);
   int fd = open( "output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600 );
   printf("ABCDE\n");
   
   int rc = fork();
   
   if (rc > 0){
       
       wait(NULL);
       dup2(1,2);
       
   }else{
       fprintf(stderr, "KLMNO");
   }
   printf("PQRST");
   fprintf(stderr, "UVWXY");
   close(fd);
   return EXIT_SUCCESS;	 

}