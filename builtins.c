#include "my_shell.h"

int command_cd(char** args, char* init_dir)
{
    (void)init_dir;
    if (args[1] == NULL) {
        printf("cd: expected arguments\n");
    } else if (chdir(args[1]) == 0) {
        // printf("CD worked\n");
    }
    else {
        perror("CD");
    }
    return 0;
}

int command_pwd()
{
    char* cwd = NULL;

    // Use dynamic allocation
    cwd = getcwd(NULL, 0);
    if (cwd != NULL) {
        printf("%s\n", cwd);
        free(cwd);
    }
    else {
        perror("getcwd");
    }
    return 0;
}

int command_cd(char** args, char* initial_directory);
int command_pwd();
int command_echo(char** args, char** env);
int command_env(char** env);
int command_which(char** args, char** env);