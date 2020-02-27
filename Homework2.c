#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

char * get_line(){
    char buffer[10];
    char *input = 0;
    size_t cur_len = 0;
    char * yo;
    while (fgets(buffer, sizeof(buffer), stdin) != 0 )
    {
        size_t size = sizeof(buffer) / sizeof(buffer[0]);
        ///printf("%c",buffer[size-10]);
        size_t buf_len = strlen(buffer);
        char *extra = realloc(input, buf_len + cur_len + 1);
        if (extra == 0){
            break;
        }
        input = extra;
        strcpy(input + cur_len, buffer);
        cur_len += buf_len;
        yo  = strpbrk(buffer,"\n");
        if(yo!=NULL){
            break;
        }
    }
    return input;
}
int main()
{
   


    
    char * ptr;
    char * buf;
    long size;
    size = pathconf(".", _PC_PATH_MAX);
    
    char input[10];
    int over = 1;
    while(over){
        if ((buf = (char *)malloc((size_t)size)) != NULL){
            ptr = getcwd(buf, (size_t)size);
        }
        printf("%s$ ",ptr);
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
    free(buf);
    return EXIT_SUCCESS;
}