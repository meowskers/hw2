#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>

char * get_line(){
    char buffer[10];
    char *input = 0;
    size_t cur_len = 0;
    char * yo;
    while (fgets(buffer, sizeof(buffer), stdin) != 0 )
    {
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
char ** split_pipe(char*line){
    line[strcspn(line,"\n")] = '\0';
    char ** input = calloc(2,sizeof(char*));
    input[0] = calloc(1024,sizeof(char));
    input[1] = calloc(1024,sizeof(char));
    char * pch;
    pch = strchr(line,'|');
    strncpy(input[0],line,pch-line-1);
    int len = strlen(line)-strlen(pch);
    int temp = pch-line+2;
    strncpy(input[1],line+temp,len);
    /*
    printf("found at %ld\n",pch-line+1);
    printf("FIRST: %s\n",input[0]);
    printf("SECOND: %s\n",input[1]);
    */
    return input;
}
char ** split_line(char* line, char* delim){
    line[strcspn(line,"\n")] = '\0';
    int words = count_commands(line,delim[0]);
    char ** input = calloc(words+1,sizeof(char*));
    char * token = strtok(line, delim);
    int count = 0;
    while( token != NULL){
        input[count] = calloc(1024,sizeof(char));
        strcpy(input[count],token);
        token = strtok(NULL, delim);
        count = count + 1;
    }
    input[count] = NULL;
    
    return input;
}
void free_2d(char ** commands, int words){
    for(int i = 0; i < words; i++){
        free(commands[i]);
    }
    free(commands);

}
char * triconcat(char*first, char* second, char* third){
    char * temp = calloc(1024,sizeof(char));
    strcpy(temp,first);
    strcat(temp, second);
    char* token = strtok(third,"\n");
    strcat(temp, token);
    return temp;
    
}
int get_commands(char ** commands, char ** paths,int path_free){
    struct stat buffer;
    int status;
    for(int i = 0; i < path_free; i++){
        char* temp = triconcat(paths[i],"/",commands[0]);
        status = lstat(temp, &buffer);
        if(status==0){
            strcpy(commands[0], temp);
            free(temp);
            return 1;
        }
        free(temp);
    }     
    return 0;
}
int main()
{
    setvbuf(stdout,NULL,_IONBF,0);
    char * input;
    char ** commands;
    char ** temp_p;
    char ** pipeline_commands1;
    char ** pipeline_commands2;
    
    long size;
    char * buf;
    char * ptr;
    char * my_path;
    char * home;
    
    
    my_path = getenv("MYPATH");
    home = getenv("HOME");
    int path_free = count_commands(my_path,':');
    char ** paths = split_line(my_path,":");
    
    
    size = pathconf(".", _PC_PATH_MAX);
    while(1){
        int PIPELINE = 0;
        int BACKGROUND = 0;
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
        // Check if it a Pipeline
        for(int i = 0; input[i]; i++){
            if(input[i]=='|'){
                PIPELINE = 1;
            }
        }
        if(PIPELINE){
            temp_p = split_pipe(input);
        }
        
        // 2D array of all commands 
        commands = split_line(input, " ");
        
        // If no commands return Error
        if(commands[0]==NULL){
            fprintf(stderr,"ERROR: \"%s\" is not a command!\n",commands[0]);
            continue;
        }

        // Check if it is a background 

        if(strcmp(commands[words-1],"&")==0){
            BACKGROUND = 1;
        }
        if(strcmp(commands[0],"cd")==0){
            if(words == 1){
                chdir(home);
            }else{
                if(strcmp(commands[1],"..")==0){
                    chdir(commands[1]);
                }else{
                    char * directory = calloc(1024,sizeof(char));
                    strcpy(directory,ptr);
                    strcat(directory,commands[1]);
                    chdir(directory);
                    free(directory);
                }
            }
            
        }else if(PIPELINE && BACKGROUND){
            printf("ITS A BACKGROUND PIPELINE\n");
            
            
            free(temp_p[0]);
            free(temp_p[1]);
            free(temp_p);
        }else if(PIPELINE){
            int words1 = count_commands(temp_p[0],' ');
            int words2 = count_commands(temp_p[1],' ');
            pipeline_commands1 = split_line(temp_p[0], " ");
            pipeline_commands2 = split_line(temp_p[1], " ");


            // YOU NEED TO WRITE TWO PIPES
            
            /*
            
            pid_t child_pid;
            int is_command1 = get_commands(commands,paths,path_free);
            if(is_command){
                 child_pid = fork();
                 if(child_pid<0){
                     printf("FAILED TO FORK");
                     return -1;
                 }
                 if(child_pid == 0){
                     // Child
                     execv(commands[0],commands);
                 }else{
                     // Parent
                     pid_t w;
                     int child_status;
                     do {
                         w = waitpid(child_pid, &child_status,0);
                         if (w == -1) { perror("waitpid"); exit(EXIT_FAILURE); }
                         
                     } while( !WIFEXITED(child_status)&&!WIFSIGNALED(child_status));
                 }
            }else{
                fprintf(stderr,"ERROR: '%s' is not a command!\n",commands[0]); 
            }
            */
            
            free(temp_p[0]);
            free(temp_p[1]);
            free(temp_p);
            free_2d(pipeline_commands1,words1);
            free_2d(pipeline_commands2,words2);
        }else if(BACKGROUND){
        
            int is_command = get_commands(commands,paths,path_free);
            free(commands[words-1]);
            commands[words-1] = NULL;           
            if(is_command){
                pid_t child_pid;
                int kid_status;
                child_pid = fork();
                if(child_pid<0){
                    printf("FAILED TO FORK");
                    return -1;
                }
                if(child_pid == 0){
                    // Child
                    
                    printf("[running background process \"%s\"]\n",commands[0]);
                    
                    int pid, q;
                    pid= fork();
                    if(pid == 0){
                        execv(commands[0],commands);
                        printf("[process %d terminated abnormally]",pid);
                        free(buf);
                        free_2d(commands,words);
                        free(input);
                        free_2d(paths, path_free);
                        return -1;
                    }else{
                      // Parent
                        do {
                            q = waitpid(pid, &kid_status,0);
                            if (q == -1) { perror("waitpid"); exit(EXIT_FAILURE); }
                        } while( !WIFEXITED(kid_status)&&!WIFSIGNALED(kid_status));
                        printf("[process %d terminated with exit status 0]\n",pid);
                        free(buf);
                        free_2d(commands,words);
                        free(input);
                        free_2d(paths, path_free);
                        return EXIT_SUCCESS;
                    }
              
                }
            }else{
                fprintf(stderr,"ERROR: \"%s\" is not a command!\n",commands[0]); 
            }
            
            
        }else{
            // NORMAL COMMAND 
            int is_command = get_commands(commands,paths,path_free);
            if(is_command){
                 pid_t child_pid, w;
                 int child_status;
                 child_pid = fork();
                 if(child_pid<0){
                     printf("FAILED TO FORK");
                     return -1;
                 }
                 if(child_pid == 0){
                     // Child
                     execv(commands[0],commands);
                 }else{
                     // Parent
                     do {
                         w = waitpid(child_pid, &child_status,0);
                         if (w == -1) { perror("waitpid"); exit(EXIT_FAILURE); }
                         
                     } while( !WIFEXITED(child_status)&&!WIFSIGNALED(child_status));
                 }
            }else{
                fprintf(stderr,"ERROR: \"%s\" is not a command!\n",commands[0]); 
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