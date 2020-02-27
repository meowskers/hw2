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
char ** split_line(char* line){
    char ** input = (char**)calloc(100,sizeof(char*));
    char * token = strtok(line, " ");
    int count = 0;
    while( token != NULL){
        printf("%s\n",token);
        input[count] = (char *)malloc(1024*sizeof(char));
        strcpy(input[count],token);
        token = strtok(NULL, " ");
        count = count + 1;
    }
    return input;
}
int main()
{
    char * input;
    char ** commands;
    while(1){
        input = get_line();
        
        if(strcmp(input,"quit\n")==0){
            free(input);
            break;
        }
        commands = split_line(input);
        int i = 0;
        int trip = 1;
        while(i<99){
            if(commands[i] == NULL){
                printf("ENDING\n");
                break;
            }
            free(commands[i]);
            printf("%s\n",commands[i]);
            i = i +1;
        }
        free(commands);
        free(input);
    }


    return EXIT_SUCCESS;
}