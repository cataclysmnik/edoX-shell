#include "my_shell.h"

// Shell loop
// Input Parsing
// Command Execution
// Built-in commands: cd, pwd, echo, env, setenv, which, exit
// Handle external commands
// Manage environment variables
// Manage path
// Error handling


int shell_builts(char** args,char** env, char* initial_directory)
{
    (void) env;
    (void) initial_directory;
    if (my_strcmp(args[0], "cd") == 0)
    {
        command_cd(args, initial_directory);
    }
    else if (my_strcmp(args[0], "pwd") == 0)
    {
        command_pwd();
    }
    else if (my_strcmp(args[0], "echo") == 0)
    {
        // command_echo(args, env);
    }
    else if (my_strcmp(args[0], "env") == 0)
    {
        // command_env(env);
    }
    else if (my_strcmp(args[0], "which") == 0)
    {
        // command_which(args, env);
    }
    else if (my_strcmp(args[0], "exit") == 0 || my_strcmp(args[0], "quit") == 0)
    {
        exit(EXIT_SUCCESS);
    }
    else {
        // executor();
    }
    return 0;
}

void shell_loop(char **env) 
{
    char* input=NULL;
    size_t input_size = 0;

    char** args;

    char* initial_directory = getcwd(NULL, 0);

    while(1) {
        printf("[my_shell]> ");
        if(getline(&input, &input_size, stdin) == -1) // EOF, ctrl + D
        {
            perror("getline");
            break;
        }
        // printf("Input: %s", input);

        args = parse_input(input);

        // for (size_t i = 0; args[i]; i++)
        // {
        //     printf("Args: %s\n", args[i]);
        // }

        if(args[0])
        {
            shell_builts(args, env, initial_directory);
        }
    }

    free_tokens(args);
}

int main(int argc, char** argv, char** env)
{
    (void)argc;
    (void)argv;

    shell_loop(env);

    return 0;
}