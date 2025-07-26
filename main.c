#include "my_shell.h"

// Shell loop
// Input Parsing
// Command Execution
// Built-in commands: cd, pwd, echo, env, setenv, which, exit
// Handle external commands
// Manage environment variables
// Manage path
// Error handling

void shell_loop(char **env) 
{
    char* input=NULL;
    size_t input_size = 0;

    char** args;

    while(1) {
        printf("[my_shell]> ");
        if(getline(&input, &input_size, stdin) == -1) // EOF, ctrl + D
        {
            perror("getline");
            break;
        }
        // printf("Input: %s", input);

        args = parse_input(input);
        for (size_t i = 0; args[i]; i++)
        {
            printf("Args: %s\n", args[i]);
        }
    }
}

int main(int argc, char** argv, char** env)
{
    (void)argc;
    (void)argv;

    shell_loop(env);

    return 0;
}