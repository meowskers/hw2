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

int count_commands(char* line, char delim){
    int i = 0;
    int words = 0;
    while(line[i] != '\0'){
        if(line[i] == delim){
            words = words +1;
        }
        i = i+1 ;
    }
    words = words +1;
    return words;
}
char ** split_line(char* line, char* delim){
    int words = count_commands(line,delim[0]);
    char ** input = calloc(words,sizeof(char*));
    char * token = strtok(line, delim);
    int count = 0;
    while( token != NULL){
        input[count] = calloc(1024,sizeof(char));
        strcpy(input[count],token);
        token = strtok(NULL, delim);
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
    long size;
    char * buf;
    char * ptr;
    char * my_path;
    
    
    my_path = getenv("MYPATH");
    printf("%s",my_path);
    int path_free = count_commands(my_path,':');
    char ** paths = split_line(my_path,":");
    
    
    size = pathconf(".", _PC_PATH_MAX);
    while(1){
        if ((buf = (char *)malloc((size_t)size)) != NULL)
            ptr = getcwd(buf, (size_t)size);
        printf("%s$ ",ptr);
        input = get_line();
        int words = count_commands(input,' ');
        if(strcmp(input,"quit\n")==0){
            printf("bye\n");
            free(input);
            free(buf);
            break;
        }
        // 2D array of all commands 
        commands = split_line(input, " ");

        //~~~~~~~~~~~~~~~~
        for(int i = 0; i < words; i++){
            
            if(strcmp(commands[i],"|")==0){
                printf("PIPELINE");
            }
        }
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        //~~~~~~~~~~~~~~~~
        // Freeing inputs
        free(buf);
        free_2d(commands,words);
        free(input);



    }
    // Freeing MYPATH
    free_2d(paths, path_free);
    

    return EXIT_SUCCESS;
}