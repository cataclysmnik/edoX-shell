#include "my_shell.h"
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

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
    printf("\trun <file>          - Compile and run the given file.\n");
    printf("\techo <text>         - Print the given text.\n");
    printf("\tenv                 - Display all environment variables.\n");
    printf("\tsetenv VAR=value    - Set an environment variable.\n");
    printf("\tunsetenv <variable> - Remove an environment variable.\n");
    printf("\twhich <command>     - Locate an executable in the system's PATH.\n");
    printf("\t.help               - Display this help message.\n");
    printf("\thelp <command>      - Display help messages with examples for certain commands.\n");
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
    } else if (my_strcmp(args[0], "run") == 0) {
        return command_run(args, env);
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

/* raw mode helpers for single-char input */
static struct termios orig_termios;
static int raw_enabled = 0;

static void disable_raw_mode(void) {
    if (raw_enabled) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        raw_enabled = 0;
    }
}

static int enable_raw_mode(void) {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) return -1;
    atexit(disable_raw_mode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // no echo, non-canonical
    raw.c_iflag &= ~(IXON); // disable Ctrl-S/Ctrl-Q flow control
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) return -1;
    raw_enabled = 1;
    return 0;
}

/* print prompt (cwd) */
static void print_prompt(void) {
    char* cwd = getcwd(NULL, 0);
    if (cwd) {
        printf("[edosh] %s > ", cwd);
        free(cwd);
    } else {
        printf("[unknown]> ");
    }
    fflush(stdout);
}

static void refresh_display(const char* input_buf, size_t input_len, size_t cursor)
{
    printf("\r");
    print_prompt();
    printf("\x1b[K"); /* clear to end of line */
    if (input_len > 0) {
        /* print the whole buffer */
        fwrite(input_buf, 1, input_len, stdout);
    }
    /* move cursor back to correct position */
    size_t to_move = input_len > cursor ? input_len - cursor : 0;
    for (size_t m = 0; m < to_move; ++m) printf("\x1b[D");
    fflush(stdout);
}

void shell_loop(char** env)
{
    char input_buf[MAX_INPUT];
    size_t input_len = 0;
    size_t cursor = 0;

    char** args;
    char* initial_directory = getcwd(NULL, 0);

    /* print a blank line before the next prompt when the previous input executed */
    bool need_leading_newline = false;

    /* Clear the terminal and show a big "edoX" banner on startup */
    system("clear");
    printf("\n");
    printf("  _____    ____     ____    __   __ \n");
    printf(" |  ___|  |  _ \\   / __ \\   \\ \\ / / \n");
    printf(" | |__    | | | | | |  | |   \\ V /  \n");
    printf(" |  __|   | | | | | |  | |    > <   \n");
    printf(" | |___   | |_| | | |__| |   / . \\  \n");
    printf(" |_____|  |____/   \\____/   /_/ \\_\\ \n\n");

    printf("\nEnter .help for help.\n");

    /* install our SIGINT handler for the interactive prompt */
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    /* simple history */
    #define HISTORY_SIZE 100
    char* history[HISTORY_SIZE];
    int history_count = 0;
    int history_index = 0; /* navigation index */
    for (int i = 0; i < HISTORY_SIZE; ++i) history[i] = NULL;

    while (1)
    {
        /* reset input buffer and navigation index */
        memset(input_buf, 0, sizeof(input_buf));
        input_len = 0;
        cursor = 0;
        history_index = history_count; /* start at "current" (no selection) */

        /* if the previous command ran, print an extra newline before showing the prompt */
        if (need_leading_newline) {
            putchar('\n');
            need_leading_newline = false;
        }
        print_prompt();

        if (enable_raw_mode() == -1) {
            perror("tcgetattr");
            break;
        }

        while (1) {
            char c;
            ssize_t r = read(STDIN_FILENO, &c, 1);
            if (r <= 0) {
                disable_raw_mode();
                break;
            }

            if (c == '\r' || c == '\n') { /* Enter */
                putchar('\n');
                disable_raw_mode();
                break;
            } else if (c == 127 || c == 8) { /* Backspace */
                 if (cursor > 0) {
                     /* remove char before cursor */
                     memmove(&input_buf[cursor - 1], &input_buf[cursor], input_len - cursor + 1); /* include null */
                     input_len--;
                     cursor--;
                    refresh_display(input_buf, input_len, cursor);
                 }
             } else if (c == '\x1b') { /* Escape sequence: arrows */
                char seq[2] = {0,0};
                if (read(STDIN_FILENO, &seq[0], 1) <= 0) continue;
                if (read(STDIN_FILENO, &seq[1], 1) <= 0) continue;
                if (seq[0] == '[') {
                    if (seq[1] == 'A') { /* Up */
                        if (history_count == 0) continue;
                        if (history_index > 0) history_index--;
                        const char* h = history[history_index];
                        if (h) {
                            strncpy(input_buf, h, sizeof(input_buf)-1);
                            input_len = strlen(input_buf);
                            cursor = input_len;
                        } else {
                            input_buf[0] = '\0';
                            input_len = cursor = 0;
                        }
                        refresh_display(input_buf, input_len, cursor);
                    } else if (seq[1] == 'B') { /* Down */
                        if (history_count == 0) continue;
                        if (history_index < history_count - 1) {
                            history_index++;
                            const char* h = history[history_index];
                            if (h) {
                                strncpy(input_buf, h, sizeof(input_buf)-1);
                                input_len = strlen(input_buf);
                                cursor = input_len;
                            }
                        } else {
                            history_index = history_count;
                            input_buf[0] = '\0';
                            input_len = cursor = 0;
                        }
                        refresh_display(input_buf, input_len, cursor);
                    } else if (seq[1] == 'C') { /* Right */
                         if (cursor < input_len) {
                             printf("\x1b[C");
                             cursor++;
                             fflush(stdout);
                         }
                     } else if (seq[1] == 'D') { /* Left */
                         if (cursor > 0) {
                             printf("\x1b[D");
                             cursor--;
                             fflush(stdout);
                         }
                     }
                 }
             } else if ((unsigned char)c >= 32 && (unsigned char)c <= 126) { /* printable */
                 if (input_len + 1 < sizeof(input_buf)) {
                     /* insert at cursor */
                     memmove(&input_buf[cursor + 1], &input_buf[cursor], input_len - cursor + 1);
                     input_buf[cursor] = c;
                     input_len++;
                     cursor++;
                    refresh_display(input_buf, input_len, cursor);
                     /* if user was navigating history and types, move to editing (empties selection) */
                     history_index = history_count;
                 }
             } else {
                 /* ignore other control characters */
                 continue;
             }
        } /* end char read loop */

        /* if raw mode was left due to EOF */
        if (!raw_enabled && feof(stdin)) break;

        /* trim leading/trailing whitespace */
        size_t start = 0;
        while (start < input_len && (input_buf[start] == ' ' || input_buf[start] == '\t' || input_buf[start] == '\n')) start++;
        size_t end = input_len;
        while (end > start && (input_buf[end-1] == ' ' || input_buf[end-1] == '\t' || input_buf[end-1] == '\n')) end--;
        size_t linelen = (end > start) ? (end - start) : 0;

        if (linelen == 0) {
            continue;
        }

        /* add to history (keep newest at end) */
        if (linelen > 0) {
            char* entry = malloc(linelen + 1);
            if (entry) {
                memcpy(entry, &input_buf[start], linelen);
                entry[linelen] = '\0';
                if (history_count == HISTORY_SIZE) {
                    /* drop oldest */
                    free(history[0]);
                    memmove(&history[0], &history[1], (HISTORY_SIZE - 1) * sizeof(char*));
                    history[HISTORY_SIZE - 1] = entry;
                } else {
                    history[history_count++] = entry;
                }
            }
        }

        /* parse & execute */
        args = parse_input(&input_buf[start]);
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
        /* mark that a command executed so next prompt is preceded by a newline */
        need_leading_newline = true;

    } /* main while */

    /* cleanup history */
    for (int i = 0; i < history_count; ++i) free(history[i]);
    disable_raw_mode();
    free(initial_directory);
    free(env);
}  /* end shell_loop */

/* Program entry point */
int main(int argc, char** argv, char** env)
{
    (void)argc;
    (void)argv;
    shell_loop(env);
    return 0;
}