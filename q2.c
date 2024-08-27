#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

void execute_command(char **args);

int main(){
    char *input;
    char *args[20];
    int i;
    // Get current working deirectory
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    while (1){
        // printf("\033[2J\033[1;1H");
        // system("clear");
        printf("\nPress ctrl + c to exit from bash window\n\n");
        printf("%s>", cwd);
        input=readline(" ");
	    if(strlen(input)>0)add_history(input);
        args[0] = strtok(input, " \n");
        for (i = 1; i < 20; i++){
            args[i] = strtok(NULL, " \n");
            if (args[i] == NULL)
                break;
        }

        execute_command(args);
    }

    return 0;
}

void execute_command(char **args){
    pid_t pid;

// Copy Command
    if (strcmp(args[0], "cp") == 0){
        if (args[1] == NULL || args[2] == NULL){
            printf("Usage: cp <source_file> <destination_file>\n");
            return;
        }
        FILE *source = fopen(args[1], "r");
        if (source == NULL){
            printf("cp: %s: No such file or directory\n", args[1]);
            return;
        }
        FILE *dest = fopen(args[2], "w");
        if (dest == NULL){
            printf("cp: %s: No such file or directory\n", args[2]);
            return;
        }
        char c;
        while ((c = fgetc(source)) != EOF){
            fputc(c, dest);
        }
        fclose(source);
        fclose(dest);
    }

    // Move Command
    else if (strcmp(args[0], "mv") == 0){
        if (args[1] == NULL || args[2] == NULL){
            printf("Usage: mv <source_file> <destination_file>\n");
            return;
        }
        int result = rename(args[1], args[2]);
        if (result != 0){
            printf("mv: %s: No such file or directory\n", args[1]);
            return;
        }
    }

    // Remove Command
    else if (strcmp(args[0], "rm") == 0){
        if (args[1] == NULL){
            printf("Usage: rm <file>\n");
            return;
        }
        int result = unlink(args[1]);
        if (result != 0){
            printf("rm: %s: No such file or directory\n", args[1]);
            return;
        }
    }

    // List Command
    else if (strcmp(args[0], "ls") == 0){
        pid = fork();
        if (pid == 0){
            if (args[1] != NULL){
                execlp("ls", "ls", args[1], NULL);
            }
            else{
                execlp("ls", "ls", NULL);
            }
        }
        else{
            wait(NULL);
        }
    }

    // Process Status Command
    else if (strcmp(args[0], "ps") == 0){
        pid = fork();
        if (pid == 0){
            execlp("ps", "ps", NULL);
        }
        else{
            wait(NULL);
        }
    }

    // Global Regular Expression Print Command
    else if (strcmp(args[0], "grep") == 0){
        if (args[1] == NULL || args[2] == NULL){
            printf("Usage: grep <pattern> <file>\n");
            return;
        }
        FILE *file = fopen(args[2], "r");
        if (file == NULL){
            printf("grep: %s: No such file or directory\n", args[2]);
            return;
        }
        char line[100];
        while (fgets(line, 100, file) != NULL){
            if (strstr(line, args[1]) != NULL){
                printf("%s", line);
            }
        }
        fclose(file);
    }

    // Visual Display Editor Command
    else if (strcmp(args[0], "vi") == 0){
        pid = fork();
        if (pid == 0){
            execlp("vi", "vi", args[1], NULL);
        }
        else{
            wait(NULL);
        }
    }
    else{
        printf("%s: command not found\n", args[0]);
    }
}
