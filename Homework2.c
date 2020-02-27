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
int count_commands(char* line){
    int i = 0;
    int words = 0;
    while(line[i] != '\0'){
        if(line[i] == ' '){
            words = words +1;
        }
        i = i+1 ;
    }
    words = words +1;
    return words;
}
char ** split_line(char* line){
    int words = count_commands(line);
    char ** input = calloc(words,sizeof(char*));
    char * token = strtok(line, " ");
    int count = 0;
    while( token != NULL){
        printf("%s\n",token);
        input[count] = calloc(1024,sizeof(char));
        strcpy(input[count],token);
        token = strtok(NULL, " ");
        count = count + 1;
    }
    
    return input;
}
void free_2d(char ** commands, int words){
    for(int i = 0; i < words; i++){
        free(commands[i]);
    }
    free(commands);

}
int main()
{
    char * input;
    char ** commands;
    while(1){
        input = get_line();
        int words = count_commands(input);
        if(strcmp(input,"quit\n")==0){
            free(input);
            break;
        }
        commands = split_line(input);
        free_2d(commands,words);
        free(input);
        /*
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
        */

    }


    return EXIT_SUCCESS;
}