#include "my_shell.h"
#include <signal.h>

// Executes a command by forking and running it in a child process
int executor(char** args, char** env)
{
    pid_t pid;
    int status;

    /* ignore SIGINT in parent around fork so parent isn't terminated by Ctrl+C
       save old action to restore after child finishes */
    struct sigaction sa_ignore, sa_old;
    sa_ignore.sa_handler = SIG_IGN;
    sigemptyset(&sa_ignore.sa_mask);
    sa_ignore.sa_flags = 0;
    sigaction(SIGINT, &sa_ignore, &sa_old);

    pid = fork();
    if (pid == -1) {
        /* restore previous handler before returning */
        sigaction(SIGINT, &sa_old, NULL);
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* In child: restore default SIGINT behavior so child is interruptible */
        struct sigaction sa_default;
        sa_default.sa_handler = SIG_DFL;
        sigemptyset(&sa_default.sa_mask);
        sa_default.sa_flags = 0;
        sigaction(SIGINT, &sa_default, NULL);

        /* child_process will _exit if execve fails, so no return expected */
        child_process(args, env);
        /* If child_process returns, command was not found - exit cleanly */
        _exit(127);  /* Standard "command not found" exit code */
    } 
    else // Parent process
    {
        if (waitpid(pid, &status, 0) == -1) {
            /* restore previous handler before returning */
            sigaction(SIGINT, &sa_old, NULL);
            perror("waitpid");
            return 1;
        }
        /* restore parent's previous SIGINT handling (likely our interactive handler) */
        sigaction(SIGINT, &sa_old, NULL);

        if (WIFSIGNALED(status)) {
            /* only print if it's not SIGINT (Ctrl+C) which is expected */
            int sig = WTERMSIG(status);
            if (sig != SIGINT) {
                fprintf(stderr, "Process terminated by signal: %d\n", sig);
            }
        } else if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            /* 127 means command not found - message already printed by child */
            if (exit_code == 127) {
                return 127;
            }
        }
    }
    return 0;
}

// Attempts to execute the command by searching paths and the current directory
int child_process(char** args, char** env) 
{
    char* path_string = get_path(env);
    int num_paths;
    char** path_list = split_paths(path_string, &num_paths);

    // access() execve()
    for (int i = 0; i < num_paths; i++) {
        char full_path[MAX_INPUT];
        snprintf(full_path, sizeof(full_path), "%s/%s", path_list[i], args[0]);

        if (access(full_path, X_OK) == 0) {
            execve(full_path, args, env);
        }
    }

    // Free the allocated paths - use num_paths, not NULL check
    for (int i = 0; i < num_paths; i++) {
        free(path_list[i]);
    }
    free(path_string);
    free(path_list);

    // Try executing the command in the current working directory
    char* cwd = NULL;
    cwd = getcwd(NULL, 0);
    if(cwd == NULL) {
        perror("getcwd");
        return 1;
    }

    char full_cwd_path[MAX_INPUT];
    snprintf(full_cwd_path, sizeof(full_cwd_path), "%s/%s", cwd, args[0]);
    free(cwd);
    execve(full_cwd_path, args, env);

    // If we reach here, command was not found anywhere
    // Print a helpful error message with suggestions
    fprintf(stderr, "edosh: command not found: %s\n", args[0]);
    
    // Provide suggestions for common typos
    if (my_strcmp(args[0], "lol") == 0 || my_strcmp(args[0], "lmao") == 0) {
        fprintf(stderr, "Did you mean: ls (list directory contents)?\n");
    } else if (my_strcmp(args[0], "clera") == 0 || my_strcmp(args[0], "claer") == 0) {
        fprintf(stderr, "Did you mean: clear?\n");
    } else if (my_strcmp(args[0], "sl") == 0) {
        fprintf(stderr, "Did you mean: ls?\n");
    } else if (my_strcmp(args[0], "gti") == 0) {
        fprintf(stderr, "Did you mean: git?\n");
    } else {
        fprintf(stderr, "Type 'help' for available commands.\n");
    }
    
    return 1;
}

// Fetches the PATH environment variable
char* get_path(char** env) {
    for (int i = 0; env[i]; i++) {
        if(my_strncmp(env[i], "PATH=", 5) == 0) {
            return my_strdup(env[i] + 5);
        }
    }
    return NULL;
}

// Split the PATH string into individual paths
char** split_paths(char* paths, int* count) {
    char** result = NULL;
    char* token;
    size_t size_of_path = my_strlen(paths);
    char paths_copy[size_of_path + 1]; // +1 for null terminator

    my_strncpy(paths_copy, paths, size_of_path + 1);
    paths_copy[size_of_path] = '\0';

    token = my_strtok(paths_copy, ":");
    *count = 0;

    while (token) {
        result = realloc(result, ((*count + 1) * sizeof(char*)));
        if (!result) {
            perror("realloc");
            return NULL;
        }
        result[*count] = my_strdup(token);
        if (!result[*count]) {
            perror("my_strdup");
            return NULL;
        }

        (*count)++;
        token = my_strtok(NULL, ":");
    }

    return result;
}