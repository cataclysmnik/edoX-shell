#include "my_shell.h"
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

// Shell loop
// Input Parsing
// Command execution 
// Handle Built-in commands exp. cd, pwd, echo, env, setenv, unsetenv, which, exit
// Execute external commands
// Manage environment variables
// Manage Path
// Error Handling

void display_help() {
    printf("Available commands:\n");
    printf("\tcd <directory>      - Change the current directory.\n");
    printf("\tpwd                 - Print the current working directory.\n");
    printf("\techo <text>         - Print the given text.\n");
    printf("\tenv                 - Display all environment variables.\n");
    printf("\tsetenv VAR=value    - Set an environment variable.\n");
    printf("\tunsetenv <variable> - Remove an environment variable.\n");
    printf("\twhich <command>     - Locate an executable in the system's PATH.\n");
    printf("\t.help               - Display this help message.\n");
    printf("\texit or quit        - Exit the shell.\n");
}

// Built-ins: cd, pwd, echo, env, setenv, unsetenv, which, exit
// Binary: ls, cat.. we'll use executor
int shell_builts(char** args, char** env, char* initial_directory)
{
    if (!args || !args[0]) return 0;

    if (my_strcmp(args[0], "cd") == 0) {
        return command_cd(args, initial_directory);
    } else if (my_strcmp(args[0], "pwd") == 0) {
        return command_pwd();
    } else if (my_strcmp(args[0], "echo") == 0) {
        return command_echo(args, env);
    } else if (my_strcmp(args[0], "env") == 0) {
        return command_env(env);
    } else if (my_strcmp(args[0], "which") == 0) {
        return command_which(args, env);
    } else if (my_strcmp(args[0], ".help") == 0) {
        display_help();
        return 0;
    } else if (my_strcmp(args[0], "help") == 0) {
        return command_help(args, env);
    } else if (my_strcmp(args[0], "exit") == 0 || my_strcmp(args[0], "quit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        /* Intercept ls and add -F if user didn't provide it so directories are suffixed with '/' */
        if (my_strcmp(args[0], "ls") == 0) {
            int has_flag = 0;
            for (size_t i = 1; args[i]; i++) {
                if (my_strcmp(args[i], "-F") == 0 ||
                    my_strcmp(args[i], "-p") == 0 ||
                    my_strcmp(args[i], "--classify") == 0) {
                    has_flag = 1;
                    break;
                }
            }
            if (!has_flag) {
                /* count existing args (including final NULL) */
                size_t count = 0;
                while (args[count]) count++;

                /* new_args: original args plus one flag and the NULL terminator */
                char** new_args = malloc((count + 1 + 1) * sizeof(char*));
                if (!new_args) {
                    perror("malloc");
                    return executor(args, env);
                }

                new_args[0] = args[0];
                new_args[1] = my_strdup("-F");
                for (size_t i = 1; i <= count; i++) { /* copy args[1..count] where args[count] == NULL */
                    new_args[i + 1] = args[i];
                }

                int ret = executor(new_args, env);

                /* free only the array and the added flag string; original token strings remain owned by parse_input/free_tokens */
                free(new_args[1]);
                free(new_args);
                return ret;
            }
        }

        /* default: execute external command */
        return executor(args, env);
    }
    return 0;
}

/* flag set by handler to indicate an interrupt occurred */
static volatile sig_atomic_t sigint_received = 0;

/* async-signal-safe handler: set flag and write a newline */
void sigint_handler(int signo)
{
    (void)signo;
    sigint_received = 1;
    /* write is async-signal-safe */
    write(STDOUT_FILENO, "\n", 1);
}

void shell_loop(char** env)
{
    char* input = NULL;
    size_t input_size = 0;

    char** args;
    char* initial_directory = getcwd(NULL, 0);

    /* Clear the terminal and show a big "edoX" banner on startup */
    system("clear");
    printf("\n");
    printf("  _____    ____     ____    __   __ \n");
    printf(" |  ___|  |  _ \\   / __ \\   \\ \\ / / \n");
    printf(" | |__    | | | | | |  | |   \\ V /  \n");
    printf(" |  __|   | | | | | |  | |    > <   \n");
    printf(" | |___   | |_| | | |__| |   / . \\  \n");
    printf(" |_____|  |____/   \\____/   /_/ \\_\\ \n\n");

    /* install our SIGINT handler for the interactive prompt */
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    while (1)
    {
        /* If we previously received SIGINT, clear the flag and continue so prompt is fresh */
        if (sigint_received) sigint_received = 0;

        char* cwd = getcwd(NULL, 0);
        if (cwd) {
            printf("%s > ", cwd);
            free(cwd);
        } else {
            printf("[unknown]> ");
        }
        fflush(stdout);

        errno = 0;
        if (getline(&input, &input_size, stdin) == -1) // End of the file (EOF), ctrl + D
        {
            if (feof(stdin)) { /* user pressed Ctrl+D or stdin closed => exit shell loop cleanly */
                break;
            }
            if (errno == EINTR) {
                /* Interrupted by signal (likely Ctrl+C) — don't exit shell, reprint prompt */
                continue;
            }
            perror("getline");
            break;
        }

        args = parse_input(input);

        if (!args || !args[0]) {
            free_tokens(args);
            continue;
        } else if (my_strcmp(args[0], "setenv") == 0) {
            env = command_setenv(args, env);
        } else if (my_strcmp(args[0], "unsetenv") == 0) {
            env = command_unsetenv(args, env);
        } else {
            shell_builts(args, env, initial_directory);
        }

        free_tokens(args);
    }

    free(input);
    free(initial_directory);
    free(env);
} 

int main (int argc, char** argv, char** env)
{
    (void)argc;
    (void)argv;

    shell_loop(env);

    return 0;
}