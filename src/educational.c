#include "my_shell.h"
#include <string.h>
#include <stdio.h>

/* Global flag for educational mode */
int educational_mode = 0;  /* Disabled by default */

/* Display welcome tutorial for new users */
void display_welcome_tutorial() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    Welcome to edoX Shell!                          ║\n");
    printf("║                   Educational Linux Shell                          ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("\x1b[36mLearning Mode is ON\x1b[0m - You'll get helpful tips as you go!\n\n");
    
    printf("\x1b[33mQuick Start Guide:\x1b[0m\n");
    printf("   • Type commands and press Enter to execute them\n");
    printf("   • Press Tab to auto-complete commands and filenames\n");
    printf("   • Use ↑/↓ arrows to navigate through command history\n");
    printf("   • Press Ctrl+C to cancel a running command\n");
    printf("   • Type 'exit' or 'quit' to close the shell\n\n");
    
    printf("\x1b[33mUseful Commands:\x1b[0m\n");
    printf("   \x1b[32mls\x1b[0m          - List files in current directory\n");
    printf("   \x1b[32mcd <dir>\x1b[0m    - Change to a different directory\n");
    printf("   \x1b[32mpwd\x1b[0m         - Show current directory path\n");
    printf("   \x1b[32mhelp\x1b[0m        - Show available commands\n");
    printf("   \x1b[32mtutorial\x1b[0m    - Interactive Linux tutorial\n");
    printf("   \x1b[32mexplain <cmd>\x1b[0m - Explain what a command does\n\n");
    
    printf("\x1b[33mTip:\x1b[0m Type '\x1b[32mlearning off\x1b[0m' to disable tips\n");
    printf("    Type '\x1b[32mlearning on\x1b[0m' to enable them again\n\n");
    
    printf("═══════════════════════════════════════════════════════════════════\n\n");
}

/* Show helpful tips after commands are executed */
void show_command_tip(const char* command) {
    if (!educational_mode || !command) return;
    
    if (my_strcmp(command, "ls") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m Files ending with '/' are directories. Use 'ls -l' for detailed info.\n");
    } else if (my_strcmp(command, "cd") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m Use 'cd ..' to go up one directory, 'cd ~' for home, or just 'cd' for root.\n");
    } else if (my_strcmp(command, "pwd") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m This shows your current location in the filesystem.\n");
    } else if (my_strcmp(command, "mkdir") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m Use 'mkdir -p parent/child' to create nested directories at once.\n");
    } else if (my_strcmp(command, "rm") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m Be careful! Deleted files can't be recovered. Use 'rm -i' for confirmation.\n");
    } else if (my_strcmp(command, "cp") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m Use 'cp -r' to copy directories and their contents.\n");
    } else if (my_strcmp(command, "mv") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m 'mv' can both move files and rename them!\n");
    } else if (my_strcmp(command, "cat") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m For large files, use 'less' to scroll through content page by page.\n");
    } else if (my_strcmp(command, "chmod") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m Common permissions: 644 (files), 755 (executables), 700 (private).\n");
    } else if (my_strcmp(command, "grep") == 0) {
        printf("\n\x1b[36mTip:\x1b[0m Use 'grep -i' for case-insensitive search, 'grep -r' to search in directories.\n");
    }
}

/* Explain what a command does */
int command_explain(char** args) {
    if (!args[1]) {
        printf("Usage: explain <command>\n");
        printf("Example: explain ls\n");
        return 1;
    }
    
    const char* cmd = args[1];
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║  Command Explanation: \x1b[32m%-47s\x1b[0m║\n", cmd);
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    if (my_strcmp(cmd, "ls") == 0) {
        printf("\x1b[33mls\x1b[0m - List Directory Contents\n\n");
        printf("   What it does:\n");
        printf("   Lists all files and folders in the current directory.\n\n");
        printf("   Common uses:\n");
        printf("   • \x1b[32mls\x1b[0m           - Show files in current directory\n");
        printf("   • \x1b[32mls -l\x1b[0m        - Detailed list with permissions, size, date\n");
        printf("   • \x1b[32mls -a\x1b[0m        - Show hidden files (starting with .)\n");
        printf("   • \x1b[32mls -lh\x1b[0m       - Human-readable file sizes (KB, MB, GB)\n");
        printf("   • \x1b[32mls /path\x1b[0m     - List contents of specific directory\n\n");
        printf("   Real-world analogy:\n");
        printf("   Like looking at the contents of a folder on your desktop!\n");
    } else if (my_strcmp(cmd, "cd") == 0) {
        printf("\x1b[33mcd\x1b[0m - Change Directory\n\n");
        printf("   What it does:\n");
        printf("   Moves you to a different folder (directory).\n\n");
        printf("   Common uses:\n");
        printf("   • \x1b[32mcd Documents\x1b[0m  - Go into Documents folder\n");
        printf("   • \x1b[32mcd ..\x1b[0m         - Go up one level (to parent directory)\n");
        printf("   • \x1b[32mcd ~\x1b[0m          - Go to your home directory\n");
        printf("   • \x1b[32mcd /\x1b[0m          - Go to the root (top) of filesystem\n");
        printf("   • \x1b[32mcd -\x1b[0m          - Go back to previous directory\n\n");
        printf("   Real-world analogy:\n");
        printf("   Like opening a different folder on your computer!\n");
    } else if (my_strcmp(cmd, "pwd") == 0) {
        printf("\x1b[33mpwd\x1b[0m - Print Working Directory\n\n");
        printf("   What it does:\n");
        printf("   Shows your current location in the file system.\n\n");
        printf("   Why use it:\n");
        printf("   • Know where you are in the directory tree\n");
        printf("   • Helps when navigating complex folder structures\n");
        printf("   • Useful before running commands that affect files\n\n");
        printf("   Real-world analogy:\n");
        printf("   Like looking at the address bar showing your current folder path!\n");
    } else if (my_strcmp(cmd, "mkdir") == 0) {
        printf("\x1b[33mmkdir\x1b[0m - Make Directory\n\n");
        printf("   What it does:\n");
        printf("   Creates a new folder (directory).\n\n");
        printf("   Common uses:\n");
        printf("   • \x1b[32mmkdir newfolder\x1b[0m      - Create a folder named 'newfolder'\n");
        printf("   • \x1b[32mmkdir -p a/b/c\x1b[0m       - Create nested folders at once\n");
        printf("   • \x1b[32mmkdir folder1 folder2\x1b[0m - Create multiple folders\n\n");
        printf("   Real-world analogy:\n");
        printf("   Like creating a new folder on your desktop!\n");
    } else if (my_strcmp(cmd, "rm") == 0) {
        printf("\x1b[33mrm\x1b[0m - Remove (Delete) Files\n\n");
        printf("   \x1b[31mWARNING: Deleted files cannot be recovered!\x1b[0m\n\n");
        printf("   What it does:\n");
        printf("   Permanently deletes files or directories.\n\n");
        printf("   Common uses:\n");
        printf("   • \x1b[32mrm file.txt\x1b[0m      - Delete a file\n");
        printf("   • \x1b[32mrm -i file.txt\x1b[0m   - Ask for confirmation before deleting\n");
        printf("   • \x1b[32mrm -r folder\x1b[0m     - Delete a folder and everything inside\n");
        printf("   • \x1b[32mrm -rf folder\x1b[0m    - Force delete (dangerous! use carefully)\n\n");
        printf("   Best practice:\n");
        printf("   Always use 'rm -i' when learning to avoid accidental deletions!\n");
    } else if (my_strcmp(cmd, "cp") == 0) {
        printf("\x1b[33mcp\x1b[0m - Copy Files\n\n");
        printf("   What it does:\n");
        printf("   Creates a copy of a file or directory.\n\n");
        printf("   Common uses:\n");
        printf("   • \x1b[32mcp file1.txt file2.txt\x1b[0m  - Copy file1 to file2\n");
        printf("   • \x1b[32mcp file.txt /path/\x1b[0m      - Copy to another directory\n");
        printf("   • \x1b[32mcp -r folder1 folder2\x1b[0m   - Copy entire directory\n");
        printf("   • \x1b[32mcp -i file.txt dest\x1b[0m     - Ask before overwriting\n\n");
        printf("   Real-world analogy:\n");
        printf("   Like Ctrl+C and Ctrl+V (copy and paste) in Windows!\n");
    } else if (my_strcmp(cmd, "mv") == 0) {
        printf("\x1b[33mmv\x1b[0m - Move or Rename Files\n\n");
        printf("   What it does:\n");
        printf("   Moves files to a new location OR renames them.\n\n");
        printf("   Common uses:\n");
        printf("   • \x1b[32mmv old.txt new.txt\x1b[0m      - Rename a file\n");
        printf("   • \x1b[32mmv file.txt /path/\x1b[0m      - Move to another directory\n");
        printf("   • \x1b[32mmv folder1 folder2\x1b[0m      - Rename a directory\n");
        printf("   • \x1b[32mmv *.txt backup/\x1b[0m        - Move all .txt files to backup\n\n");
        printf("   Real-world analogy:\n");
        printf("   Like dragging a file to a new folder, or pressing F2 to rename!\n");
    } else if (my_strcmp(cmd, "cat") == 0) {
        printf("\x1b[33mcat\x1b[0m - Concatenate and Display Files\n\n");
        printf("   What it does:\n");
        printf("   Shows the contents of a file on the screen.\n\n");
        printf("   Common uses:\n");
        printf("   • \x1b[32mcat file.txt\x1b[0m         - Display file contents\n");
        printf("   • \x1b[32mcat file1 file2\x1b[0m      - Display multiple files\n");
        printf("   • \x1b[32mcat file1 file2 > new\x1b[0m - Combine files into one\n\n");
        printf("   Alternative:\n");
        printf("   For large files, use 'less file.txt' to scroll page by page!\n");
    } else {
        printf("No detailed explanation available for '\x1b[32m%s\x1b[0m'\n\n", cmd);
        printf("   Try: \x1b[32mhelp %s\x1b[0m for basic information\n", cmd);
        printf("   Or: \x1b[32mman %s\x1b[0m to read the full manual (if installed)\n", cmd);
    }
    
    printf("\n");
    return 0;
}

/* Interactive tutorial for learning Linux basics */
int command_tutorial() {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║              Interactive Linux Tutorial                           ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n\n");
    
    printf("Welcome to the Linux command line tutorial!\n\n");
    
    printf("═══ Lesson 1: Understanding Directories ═══\n\n");
    printf("In Linux, files are organized in directories (folders).\n");
    printf("Think of it like a tree structure:\n\n");
    printf("    /  (root - the top of the tree)\n");
    printf("    ├── home/\n");
    printf("    │   ├── user/\n");
    printf("    │   │   ├── Documents/\n");
    printf("    │   │   ├── Downloads/\n");
    printf("    │   │   └── Pictures/\n");
    printf("    ├── etc/  (system configuration)\n");
    printf("    ├── usr/  (programs and applications)\n");
    printf("    └── tmp/  (temporary files)\n\n");
    
    printf("Try these commands:\n");
    printf("  1. \x1b[32mpwd\x1b[0m       - See where you are now\n");
    printf("  2. \x1b[32mls\x1b[0m        - See what files/folders are here\n");
    printf("  3. \x1b[32mls -l\x1b[0m     - See detailed information\n\n");
    
    printf("═══ Lesson 2: Navigation ═══\n\n");
    printf("Moving around:\n");
    printf("  • \x1b[32mcd Documents\x1b[0m - Go into Documents folder\n");
    printf("  • \x1b[32mcd ..\x1b[0m        - Go up one level\n");
    printf("  • \x1b[32mcd ~\x1b[0m         - Go to your home directory\n");
    printf("  • \x1b[32mcd /\x1b[0m         - Go to root directory\n\n");
    
    printf("Special symbols:\n");
    printf("  • \x1b[33m.\x1b[0m   means 'current directory'\n");
    printf("  • \x1b[33m..\x1b[0m  means 'parent directory' (one level up)\n");
    printf("  • \x1b[33m~\x1b[0m   means 'your home directory'\n");
    printf("  • \x1b[33m/\x1b[0m   means 'root directory' (top of file system)\n\n");
    
    printf("═══ Lesson 3: Working with Files ═══\n\n");
    printf("Creating:\n");
    printf("  • \x1b[32mtouch file.txt\x1b[0m   - Create empty file\n");
    printf("  • \x1b[32mmkdir folder\x1b[0m     - Create new folder\n\n");
    
    printf("Viewing:\n");
    printf("  • \x1b[32mcat file.txt\x1b[0m     - Show file contents\n");
    printf("  • \x1b[32mless file.txt\x1b[0m    - View large files (q to quit)\n");
    printf("  • \x1b[32mhead file.txt\x1b[0m    - Show first 10 lines\n");
    printf("  • \x1b[32mtail file.txt\x1b[0m    - Show last 10 lines\n\n");
    
    printf("Copying/Moving:\n");
    printf("  • \x1b[32mcp source dest\x1b[0m   - Copy file\n");
    printf("  • \x1b[32mmv old new\x1b[0m       - Move or rename\n\n");
    
    printf("Deleting:\n");
    printf("  • \x1b[32mrm file.txt\x1b[0m      - Delete file (⚠️ permanent!)\n");
    printf("  • \x1b[32mrm -r folder\x1b[0m     - Delete folder and contents\n\n");
    
    printf("═══ Lesson 4: Getting Help ═══\n\n");
    printf("  • \x1b[32mhelp\x1b[0m           - List available commands\n");
    printf("  • \x1b[32mhelp <command>\x1b[0m  - Help for specific command\n");
    printf("  • \x1b[32mexplain <command>\x1b[0m - Detailed explanation with examples\n");
    printf("  • \x1b[32mman <command>\x1b[0m   - Full manual (if available)\n\n");
    
    printf("═══ Pro Tips ═══\n\n");
    printf("  • Press \x1b[33mTab\x1b[0m to auto-complete commands and filenames\n");
    printf("  • Press \x1b[33m↑\x1b[0m and \x1b[33m↓\x1b[0m to browse command history\n");
    printf("  • Press \x1b[33mCtrl+C\x1b[0m to stop a running command\n");
    printf("  • Press \x1b[33mCtrl+L\x1b[0m or type \x1b[32mclear\x1b[0m to clear screen\n\n");
    
    printf("═══════════════════════════════════════════════════════════════════\n\n");
    printf("Practice these commands to get comfortable!\n");
    printf("   Use '\x1b[32mexplain <command>\x1b[0m' anytime you need more details.\n\n");
    
    return 0;
}
