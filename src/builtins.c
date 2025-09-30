#include "my_shell.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>   // <--- added

// cd, cd [path], cd - (previous dir), cd ~ (home dir), cd .., handle non exsiting dirs, permission issues
int command_cd(char** args, char* init_dir)
{
    (void)init_dir;
    if (args[1] == NULL) {
        printf("cd: expected argument \"cd [path]\"\n");
    } else if (chdir(args[1]) == 0) {
        // printf("CD worked!\n");
    } else {
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
    } else {
        perror("getcwd");
    }
    return 0;
}

// echo Hello World, echo -n Hello, echo $PATH
int command_echo(char** args, char** env)
{
    int new_line = 1;
    size_t i = 1;

    if (args[1] != NULL && my_strcmp(args[1], "-n") == 0) {
        new_line = 0;
        i++;
    }

    for (; args[i]; i++) {
        if (args[i][0] == '$') {
            char* value = my_getenv(args[i] + 1, env);
            if (value) {
                printf("%s", value);
            }
        } else {
            printf("%s", args[i]);
        }
        if (args[i + 1] != NULL) {
            printf(" ");
        }
    }
    if (new_line) {
        printf("\n");
    }
    return 0;
}

int command_env(char** env)
{
    size_t index = 0;
    while (env[index])
    {
        printf("%s\n", env[index]);
        index++;
    }
    return 0;    
}

char* find_command_in_path(const char* command, char** env);

int command_which(char** args, char** env)
{
    if (args[1] == NULL) {
        printf("which: expected argument\n");
        return 1;
    }

    // List of the built-ins
    const char* built_in_commands[] = {"cd", "pwd", "echo", "env", "setenv", "unsetenv", "which", "exit", NULL};
    for (size_t i = 0; built_in_commands[i]; i++) {
        if (my_strcmp(args[1], built_in_commands[i]) == 0) {
            printf("%s: shell built-in command\n", args[1]);
            return 0;
        }
    }

    // Check external commands
    char* full_path = find_command_in_path(args[1], env);
    if (full_path != NULL) {
        printf("%s\n", full_path);
        free(full_path);
        return 0;
    } else {
        printf("which: %s command not found\n", args[1]);
        return 1;
    }
}

// ---------------------- new help command ----------------------
int command_help(char** args, char** env)
{
    (void)env;
    if (!args[1]) {
        printf("Usage: help <command>\n");
        printf("Try: help cd | help pwd | help ls | help cp | help rm | help chmod | help uname | help df | help top\n");
        return 0;
    }

    const char* cmd = args[1];

    if (my_strcmp(cmd, "cd") == 0) {
        printf("cd <directory>\n");
        printf("  Change the current directory.\n");
        printf("  Example: cd /tmp\n");
    } else if (my_strcmp(cmd, "pwd") == 0) {
        printf("pwd\n");
        printf("  Print the current working directory.\n");
        printf("  Example: pwd\n");
    } else if (my_strcmp(cmd, "ls") == 0) {
        printf("ls [options] [file...]\n");
        printf("  List directory contents. The shell appends -F by default to mark directories with '/'.\n");
        printf("  Example: ls -la\n");
    } else if (my_strcmp(cmd, "mkdir") == 0) {
        printf("mkdir <directory>\n");
        printf("  Create a new directory.\n");
        printf("  Example: mkdir mydir\n");
    } else if (my_strcmp(cmd, "rmdir") == 0) {
        printf("rmdir <directory>\n");
        printf("  Remove an empty directory.\n");
        printf("  Example: rmdir mydir\n");
    } else if (my_strcmp(cmd, "touch") == 0) {
        printf("touch <file>\n");
        printf("  Create an empty file or update file timestamps.\n");
        printf("  Example: touch file.txt\n");
    } else if (my_strcmp(cmd, "cp") == 0) {
        printf("cp <source> <dest>\n");
        printf("  Copy files or directories (use -r for directories).\n");
        printf("  Example: cp file.txt /tmp/file.txt\n");
    } else if (my_strcmp(cmd, "mv") == 0) {
        printf("mv <source> <dest>\n");
        printf("  Move or rename files/directories.\n");
        printf("  Example: mv oldname newname\n");
    } else if (my_strcmp(cmd, "rm") == 0) {
        printf("rm [options] <file>\n");
        printf("  Remove files or directories (use -r to remove directories).\n");
        printf("  Example: rm -r mydir\n");
    } else if (my_strcmp(cmd, "cat") == 0) {
        printf("cat <file>\n");
        printf("  Concatenate and print file(s) to stdout.\n");
        printf("  Example: cat file.txt\n");
    } else if (my_strcmp(cmd, "less") == 0) {
        printf("less <file>\n");
        printf("  Pager to view files interactively (q to quit).\n");
        printf("  Example: less /var/log/syslog\n");
    } else if (my_strcmp(cmd, "head") == 0) {
        printf("head [options] <file>\n");
        printf("  Show the first lines of a file (default 10 lines).\n");
        printf("  Example: head -n 20 file.txt\n");
    } else if (my_strcmp(cmd, "tail") == 0) {
        printf("tail [options] <file>\n");
        printf("  Show the last lines of a file (use -f to follow).\n");
        printf("  Example: tail -f /var/log/syslog\n");
    } else if (my_strcmp(cmd, "nano") == 0) {
        printf("nano <file>\n");
        printf("  Simple terminal text editor.\n");
        printf("  Example: nano file.txt\n");
    } else if (my_strcmp(cmd, "vim") == 0) {
        printf("vim <file>\n");
        printf("  Modal terminal text editor (advanced).\n");
        printf("  Example: vim file.txt\n");
    } else if (my_strcmp(cmd, "chmod") == 0) {
        printf("chmod <mode> <file>\n");
        printf("  Change file permissions (e.g. 755).\n");
        printf("  Example: chmod 755 script.sh\n");
    } else if (my_strcmp(cmd, "chown") == 0) {
        printf("chown <owner>[:<group>] <file>\n");
        printf("  Change file owner and group.\n");
        printf("  Example: chown user:staff file.txt\n");
    } else if (my_strcmp(cmd, "uname") == 0 || (my_strcmp(cmd, "uname -a") == 0)) {
        printf("uname -a\n");
        printf("  Print system information (kernel, hostname, architecture).\n");
        printf("  Example: uname -a\n");
    } else if (my_strcmp(cmd, "df") == 0 || (my_strcmp(cmd, "df -h") == 0)) {
        printf("df -h\n");
        printf("  Show disk space usage in human-readable form.\n");
        printf("  Example: df -h\n");
    } else if (my_strcmp(cmd, "top") == 0) {
        printf("top\n");
        printf("  Interactive process viewer (press q to quit).\n");
        printf("  Example: top\n");
    } else if (my_strcmp(cmd, "exit") == 0 || my_strcmp(cmd, "quit") == 0) {
        printf("exit\n");
        printf("  Exit the shell.\n");
        printf("  Example: exit\n");
    } else if (my_strcmp(cmd, "help") == 0 || my_strcmp(cmd, ".help") == 0) {
        printf("help <command>\n");
        printf("  Show help for a builtin or frequently used external command.\n");
        printf("  Example: help cd\n");
    } else {
        printf("help: no help available for '%s'\n", cmd);
    }
    return 0;
}

// ---------------------- new run command ----------------------
int command_run(char** args, char** env)
{
    if (!args[1]) {
        printf("Usage: run <file> [args...]\n");
        return 1;
    }

    const char* file = args[1];
    const char* extp = strrchr(file, '.');
    if (!extp || extp[1] == '\0') {
        printf("run: unknown file type for '%s'\n", file);
        return 1;
    }
    const char* ext = extp + 1;

    /* helper to count extra args after the filename (args[2..]) */
    int extra = 0;
    while (args[2 + extra]) extra++;

    if (my_strcmp(ext, "c") == 0 || my_strcmp(ext, "cpp") == 0 ||
        my_strcmp(ext, "cc") == 0 || my_strcmp(ext, "cxx") == 0)
    {
        /* choose compiler */
        const char* compiler = my_strcmp(ext, "c") == 0 ? "gcc" : "g++";

        /* compile to a temp executable in /tmp */
        char outpath[PATH_MAX];
        snprintf(outpath, sizeof(outpath), "/tmp/edox_run_%d", (int)getpid());

        char* compile_cmd[] = { (char*)compiler, (char*)file, "-o", outpath, NULL };
        executor(compile_cmd, env);

        if (access(outpath, X_OK) != 0) {
            printf("run: compilation failed for '%s'\n", file);
            return 1;
        }

        /* build argv for the compiled program: outpath, then any extra args */
        int run_argc = 1 + extra;
        char** run_argv = malloc((run_argc + 1) * sizeof(char*));
        if (!run_argv) { perror("malloc"); unlink(outpath); return 1; }
        run_argv[0] = strdup(outpath);
        for (int i = 0; i < extra; ++i) {
            run_argv[1 + i] = strdup(args[2 + i]);
        }
        run_argv[run_argc] = NULL;

        /* fork and execve the absolute path to avoid executor path searching */
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            for (int i = 0; i < run_argc; ++i) free(run_argv[i]);
            free(run_argv);
            unlink(outpath);
            return 1;
        }
        if (pid == 0) {
            execve(outpath, run_argv, env);
            perror("execve");
            _exit(EXIT_FAILURE);
        } else {
            int status = 0;
            waitpid(pid, &status, 0);
            /* cleanup */
            for (int i = 0; i < run_argc; ++i) free(run_argv[i]);
            free(run_argv);
            unlink(outpath);
            return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
        }
    }
    else if (my_strcmp(ext, "py") == 0) {
        /* run with python3, pass through extra args */
        char** cmd = malloc((2 + extra + 1) * sizeof(char*));
        if (!cmd) { perror("malloc"); return 1; }
        cmd[0] = my_strdup("python3");
        cmd[1] = my_strdup(file);
        for (int i = 0; i < extra; ++i) {
            cmd[2 + i] = my_strdup(args[2 + i]);
        }
        cmd[2 + extra] = NULL;
        int ret = executor(cmd, env);
        for (int i = 0; i < 2 + extra; ++i) free(cmd[i]);
        free(cmd);
        return ret;
    }
    else if (my_strcmp(ext, "java") == 0) {
        /* javac file.java && java -cp dir ClassName */
        char* javac_args[] = { "javac", (char*)file, NULL };
        executor(javac_args, env);

        const char* slash = strrchr(file, '/');
        const char* fname = slash ? slash + 1 : file;
        size_t flen = strlen(fname);
        char class_name[NAME_MAX];
        if (flen > 5 && strcmp(fname + flen - 5, ".java") == 0) {
            memcpy(class_name, fname, flen - 5);
            class_name[flen - 5] = '\0';
        } else {
            printf("run: unexpected java filename '%s'\n", file);
            return 1;
        }

        char class_dir[PATH_MAX];
        if (slash) {
            snprintf(class_dir, sizeof(class_dir), "%.*s", (int)(slash - file), file);
        } else {
            snprintf(class_dir, sizeof(class_dir), ".");
        }

        /* build java argv (no extra arg forwarding for now) */
        char* java_args[] = { "java", "-cp", class_dir, class_name, NULL };
        return executor(java_args, env);
    } else {
        printf("run: unsupported extension '.%s'\n", ext);
        return 1;
    }
}

// Function to search for the command in PATH
char* find_command_in_path(const char* command, char** env)
{
    char* path_env = NULL; // Store the PATH value
    char* path = NULL; // Duplicate of PATH
    char* token = NULL; // Tokenize dirs from the PATH
    char full_path[1024]; // BUffer to construct full paths

    // Locate the PATH
    for (size_t i = 0; env[i]; i++) {
        if (my_strncmp(env[i], "PATH=", 5) == 0) {
            path_env = env[i] + 5; // Skip PATH= prefix
            break;
        }
    }    
    if (path_env == NULL) {
        return NULL; // No path
    } 

    // Dupliacte the PATH avoiding modifying the original
    path = my_strdup(path_env);
    if(path == NULL) {
        perror("my_strdup");
        return NULL;
    }

    // Split PATH into individual dirs using ':' delimiter
    token = my_strtok(path, ":");
    while (token != NULL) {
        // Constract full path, check for '/'
        size_t len = my_strlen(token);
        if (token[len - 1] != '/') {
            snprintf(full_path, sizeof(full_path), "%s%s%s", token, "/", command);
        } else {
            snprintf(full_path, sizeof(full_path), "%s%s", token, command);
        }

        // Check if the command exists as executable
        if (access(full_path, X_OK) == 0) {
            free(path);
            return my_strdup(full_path); // found commands path
        }

        token = my_strtok(NULL, ":"); // move to the next dir
    }

    free(path);
    return NULL;
}

// Helper function to count env vars
int count_env_vars(char** env) {
    int count = 0;
    while (env[count])
    {
        count++;
    }
    return count;
}

// Function to set an environement variable
char** command_setenv(char** args, char** env)
{
    if (args[1] == NULL) {
        printf("Usage:  setenv VAR=value\nor\tsetenv <variable> <value>\n");
        return env;
    }
    
    int env_count = count_env_vars(env);
    char** new_env = malloc((env_count + 2) * sizeof(char*));
    if(!new_env) {
        perror("malloc");
        return env;
    }

    // Copy existing environment variables
    for (int i = 0; i < env_count; i++)
    {
        new_env[i] = my_strdup(env[i]);
        if (!new_env[i]) {
            perror("strdup");
            for (int j = 0; j < i; j++) {
                free(new_env[j]);
            }
            free(new_env);
            return env;            
        }
    }

    // Determine the format of the uinput and create the new variable
    char* new_var = NULL;
    if(args[2] == NULL) {  // Format Var=value
        new_var = my_strdup(args[1]);
    } else {
        new_var = malloc(my_strlen(args[1]) + my_strlen(args[2]) + 2);
        if (new_var) {
            sprintf(new_var, "%s=%s", args[1], args[2]);
        }
    }

    if(!new_var) {
        perror("malloc");
        for (int i = 0; i < env_count; i++) {
            free(new_env[i]);
        }
        free(new_env);
        return env;
    }

    new_env[env_count] = new_var;
    new_env[env_count  + 1] = NULL;

    // Free the old env array
    // for (size_t i = 0; env[i]; i++) {
    //     free(env[i]);
    // }
    // free(env);  

    return new_env;
}

// Function to unset environment variables
char** command_unsetenv(char** args, char** env){
    if (!args[1]) {
        printf("Usage: unsetenv <variable>\n");
        return env;
    }

    int env_count = count_env_vars(env);
    char** new_env = malloc(env_count * sizeof(char*));
    if(!new_env) {
        perror("malloc");
        return env;
    }

    int j = 0, found = 0;
    for (int i = 0; i < env_count; i++) { // var=123
        if (my_strncmp(env[i], args[1], my_strlen(args[1])) == 0 && env[i][my_strlen(args[1])] == '=') {
            found = 1;
            free(env[i]); // Free the matching variable
        } else {
            new_env[j++] = env[i];
        }
    }

    if (!found) {
        printf("Variable %s not found in environment\n", args[1]);
        free(new_env);
        return env;
    }

    new_env[j] = NULL;
    // free(env);
    return new_env;
}