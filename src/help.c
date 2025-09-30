#include "my_shell.h"
#include <stdio.h>
#include <string.h>

/* help <command> moved out of builtins.c to keep that file smaller.
   Uses my_strcmp from helpers. */
int command_help(char** args, char** env)
{
    (void)env;
    if (!args || !args[1]) {
        printf("Usage: help <command>\n");
        printf("Try: help cd | help pwd | help ls | help cp | help rm | help chmod | help uname | help df | help top | help run\n");
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
    } else if (my_strcmp(cmd, "uname") == 0 || my_strcmp(cmd, "uname -a") == 0) {
        printf("uname -a\n");
        printf("  Print system information (kernel, hostname, architecture).\n");
        printf("  Example: uname -a\n");
    } else if (my_strcmp(cmd, "df") == 0 || my_strcmp(cmd, "df -h") == 0) {
        printf("df -h\n");
        printf("  Show disk space usage in human-readable form.\n");
        printf("  Example: df -h\n");
    } else if (my_strcmp(cmd, "top") == 0) {
        printf("top\n");
        printf("  Interactive process viewer (press q to quit).\n");
        printf("  Example: top\n");
    } else if (my_strcmp(cmd, "run") == 0) {
        printf("run <file> [args...]\n");
        printf("  Compile and/or run source files. Supported types: .c, .cpp, .cc, .cxx, .py, .java\n");
        printf("  - C:    compiles with gcc and runs the produced binary.\n");
        printf("  - C++:  compiles with g++ and runs the produced binary.\n");
        printf("  - Python: runs with python3.\n");
        printf("  - Java: javac then java (class name derived from filename).\n");
        printf("  Examples:\n");
        printf("    run hello.c\n");
        printf("    run codes/cppt.cpp arg1 arg2\n");
        printf("    run script.py --flag\n");
        printf("    run MyClass.java\n");
    } else if (my_strcmp(cmd, "ping") == 0) {
        printf("ping [options] <host>\n");
        printf("  Send ICMP ECHO_REQUEST packets to network hosts and display replies.\n");
        printf("  Useful options:\n");
        printf("    -c <count>    Send <count> packets then stop (e.g. -c 4).\n");
        printf("    -i <seconds>  Wait <seconds> between packets (default 1s).\n");
        printf("    -w <deadline> Specify a timeout in seconds for the whole run.\n");
        printf("  Example: ping -c 4 8.8.8.8\n");
    } else {
        printf("help: no help available for '%s'\n", cmd);
    }
    return 0;
}