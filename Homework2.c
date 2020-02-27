#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>


int main()
{
   
    char * hi = getenv("MYPATH");
    printf("%s\n",hi);
    int size = pathconf(".", _PC_PATH_MAX);
    char * buf;
    char * ptr;
    //~~~~~~~~~~~~~
    
    
    
    char *text = calloc(1,1), buffer[10];
    printf("Enter a message: \n");
    while( fgets(buffer, 10 , stdin) ) 
    {
        text = realloc( text, strlen(text)+1+strlen(buffer) );
        if( !text ){
            printf("RABBLE RABBLE");
        }
       
        strcat( text, buffer ); 
        printf("%s\n", buffer);
    }
        printf("\ntext:\n%s",text);
        free(text);
    return 0;
    
    
    
    
    
    //~~~~~~~~~~~
    /*
    char input[10];
    int over = 1;
    while(over){
        if ((buf = (char *)malloc((size_t)size)) != NULL){
            ptr = getcwd(buf, (size_t)size);
        }
        printf("%s ",ptr);
        if(fgets(input,10,stdin)){
            if(strcmp(input,"exit\n")==0){
                printf("yes");
                over = 0;
                break;
            }else{
                
            }
        } else {
            printf("Something went wrong!!!\n");
        }
        
    }
    */
    return EXIT_SUCCESS;
}