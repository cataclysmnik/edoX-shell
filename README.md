# EDOSH - Educational Shell

<div align="center">

```
  _____   ____     ___    ____   _   _ 
 |  ___| |  _ \   / _ \  / ___| | | | |
 | |__   | | | | | | | | \___ \ | |_| |
 |  __|  | | | | | | | |  ___) ||  _  |
 | |___  | |_| | | |_| | |____/ | | | |
 |_____| |____/   \___/  |____/ |_| |_|
```

**A beginner-friendly Linux shell built for learning**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform: Linux](https://img.shields.io/badge/Platform-Linux-green.svg)](https://www.linux.org/)

[Features](#features) • [Installation](#installation) • [Usage](#usage) • [Documentation](#built-in-commands)

</div>

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Educational Features](#educational-features)
- [Git Integration](#git-integration)
- [Code Runner](#code-runner)
- [Project Structure](#project-structure)
- [Technical Details](#technical-details)
- [Troubleshooting](#troubleshooting)
- [Acknowledgments](#acknowledgments)

---

## Overview

**EDOSH** (Educational Shell) is a custom Linux shell designed specifically for students and beginners learning command-line interfaces. Unlike traditional shells that assume prior knowledge, EDOSH provides contextual help, interactive tutorials, and smart suggestions to make learning Linux approachable and less intimidating.

### The Problem

Many students struggle with Linux because:
- Traditional shells assume prior knowledge
- Error messages are cryptic and unhelpful
- No guidance or context for commands
- Steep learning curve discourages beginners
- No feedback on what went wrong

### Our Solution

EDOSH bridges this gap by:
- Providing contextual tips after command execution
- Offering detailed explanations with real-world analogies
- Suggesting corrections for common typos
- Including interactive tutorials built into the shell
- Displaying helpful git information for developers
- Supporting code compilation and execution for multiple languages

---

## Features

### Educational Features

- **Learning Mode**: Toggle educational tips on/off with `learning on/off`
- **Interactive Tutorials**: 4-lesson curriculum covering Linux fundamentals
- **Command Explanations**: Detailed breakdowns with real-world analogies (`explain <command>`)
- **Contextual Tips**: Helpful suggestions appear after running commands
- **Error Suggestions**: Smart typo detection (e.g., `gti` → `git`, `sl` → `ls`, `lol` → `ls`)

### Developer Features

- **Multi-Language Code Runner**: Compile and run C, C++, Python, and Java files directly
  ```bash
  run hello.c
  run script.py arg1 arg2
  run Main.java
  run app.cpp
  ```

- **Advanced Git Integration**: Real-time repository status in prompt
  - Branch name display
  - Staged/unstaged/untracked file counts
  - Commits ahead/behind origin tracking
  - Color-coded status (green = clean, red/yellow = changes)

- **Smart Autocomplete**: Tab completion for:
  - Built-in commands
  - External commands in PATH
  - Files and directories
  - Common prefix completion for multiple matches

- **Command History**: 
  - Up/Down arrow navigation through previous commands
  - `history` command to view all past commands
  - Persistent session history (up to 100 commands)

### Shell Features

- **Raw Mode Input**: Full line editing with cursor positioning
- **Signal Handling**: Proper Ctrl+C behavior (doesn't exit shell)
- **Environment Management**: `setenv`/`unsetenv` commands
- **Path Resolution**: Searches current directory and PATH
- **Quote Handling**: Proper parsing of single/double quotes
- **Directory Shortcuts**: `cd` without args goes to root, `cd -` for previous directory

---

## Installation

### Prerequisites

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install -y build-essential git
```

#### Fedora/RHEL/CentOS
```bash
sudo dnf groupinstall "Development Tools"
sudo dnf install git
```

#### Arch Linux
```bash
sudo pacman -S base-devel git
```

### Optional Dependencies (for `run` command)

```bash
# Ubuntu/Debian
sudo apt install g++ python3 default-jdk

# Fedora
sudo dnf install gcc-c++ python3 java-11-openjdk-devel

# Arch
sudo pacman -S gcc python jdk-openjdk
```

### Build from Source

```bash
# Clone the repository
git clone https://github.com/cataclysmnik/edoX-shell.git
cd edoX-shell

# Build the shell
make

# Test locally
./edosh

# Install system-wide (optional)
sudo cp edosh /usr/local/bin/
```

### Build Options

```bash
# Clean build
make clean && make

# Remove all build artifacts
make fclean

# Rebuild from scratch
make re
```

---

## Quick Start

### First Launch

```bash
# Start EDOSH
./edosh

# You'll see the EDOSH banner and a helpful tip about learning mode
```

### Enable Learning Mode (Recommended for Beginners)

```bash
# Turn on educational tips
learning on

# Start the interactive tutorial
tutorial

# Try command explanations
explain ls
explain cd
explain grep
```

### Try Key Features

```bash
# 1. Tab Autocomplete
ls <TAB>              # Lists matching completions
cd Do<TAB>            # Completes to Documents/
ec<TAB>               # Completes to echo

# 2. Command History
pwd                   # Run some commands
ls
cd /tmp
history               # View all previous commands
# Use ↑/↓ arrows to navigate history

# 3. Code Runner
cd /tmp
echo 'print("Hello!")' > test.py
run test.py           # Runs the Python script

echo '#include <stdio.h>
int main() { printf("Hello from C!\n"); return 0; }' > hello.c
run hello.c           # Compiles and runs C program

# 4. Git Integration (in a git repository)
cd ~/my-git-repo
# Prompt shows: /home/user/my-git-repo (main ↑2 +3 ~1) >
# Meaning: main branch, 2 commits ahead, 3 staged files, 1 unstaged change

# 5. Smart Error Handling
gti status            # Suggests: Did you mean: git?
sl                    # Suggests: Did you mean: ls?
lol                   # Suggests: Did you mean: ls?
```

---

## Usage

### Basic Commands

```bash
# Navigation
cd <directory>        # Change directory
cd                    # Go to root directory
cd ~                  # Go to home directory
cd -                  # Go to previous directory
cd ..                 # Go up one level
pwd                   # Print current directory

# File Operations
ls                    # List files
ls -la                # Detailed list with hidden files
mkdir <folder>        # Create directory
touch <file>          # Create empty file
rm <file>             # Delete file
rm -rf <folder>       # Delete folder and contents
cp <src> <dest>       # Copy file
mv <src> <dest>       # Move/rename file
cat <file>            # Display file contents

# System
echo <text>           # Print text
env                   # Show environment variables
which <command>       # Locate command in PATH
history               # Show command history
exit / quit           # Exit the shell
```

### Educational Commands

```bash
# Get Help
help                  # List available commands
help <command>        # Show help for specific command
explain <command>     # Detailed explanation with examples
tutorial              # Start interactive tutorial

# Learning Mode
learning on           # Enable educational tips
learning off          # Disable tips
learning              # Check current status
```

---

## Built-in Commands

| Command | Description | Example |
|---------|-------------|---------|
| `cd` | Change directory | `cd Documents` |
| `pwd` | Print working directory | `pwd` |
| `echo` | Print text/variables | `echo $PATH` |
| `env` | Display environment variables | `env` |
| `setenv` | Set environment variable | `setenv VAR=value` |
| `unsetenv` | Remove environment variable | `unsetenv VAR` |
| `which` | Locate command in PATH | `which gcc` |
| `run` | Compile/run code files | `run hello.c` |
| `explain` | Explain command in detail | `explain ls` |
| `tutorial` | Start interactive tutorial | `tutorial` |
| `learning` | Toggle educational mode | `learning on` |
| `history` | View command history | `history` |
| `help` | Show help for commands | `help cd` |
| `exit` / `quit` | Exit the shell | `exit` |

All other commands are executed as external programs (e.g., `ls`, `grep`, `git`).

---

## Educational Features

### Interactive Tutorial

A comprehensive 4-lesson tutorial system built into the shell:

```bash
tutorial
```

**Lesson 1: Understanding Directories**
- File system structure (tree hierarchy)
- Special directories (/, ~, .)
- Absolute vs relative paths

**Lesson 2: Navigation**
- Moving between directories
- Understanding pwd, cd, ls
- Special symbols (., .., ~, /)

**Lesson 3: Working with Files**
- Creating files and directories
- Viewing file contents
- Copying, moving, deleting
- Safety warnings for destructive operations

**Lesson 4: Getting Help**
- Using help, explain, and man commands
- Tab completion
- Keyboard shortcuts (Ctrl+C, Ctrl+L, arrows)

### Command Explanations

Detailed explanations with real-world analogies:

```bash
explain ls
```

Output:
```
╔════════════════════════════════════════════════════════════════════╗
║  Command Explanation: ls                                           ║
╚════════════════════════════════════════════════════════════════════╝

ls - List Directory Contents

   What it does:
   Lists all files and folders in the current directory.

   Common uses:
   • ls           - Show files in current directory
   • ls -l        - Detailed list with permissions, size, date
   • ls -a        - Show hidden files (starting with .)
   • ls -lh       - Human-readable file sizes (KB, MB, GB)
   • ls /path     - List contents of specific directory

   Real-world analogy:
   Like looking at the contents of a folder on your desktop!
```

### Contextual Tips

When learning mode is enabled, tips appear after commands:

```bash
learning on
ls
# Tip: Files ending with '/' are directories. Use 'ls -l' for detailed info.

cd Documents
# Tip: Use 'cd ..' to go up one directory, 'cd ~' for home, or just 'cd' for root.

rm file.txt
# Tip: Be careful! Deleted files can't be recovered. Use 'rm -i' for confirmation.
```

### Error Suggestions

Smart typo detection helps beginners:

```bash
gti status
# edosh: command not found: gti
# Did you mean: git?

sl
# edosh: command not found: sl
# Did you mean: ls?

lol
# edosh: command not found: lol
# Did you mean: ls?
```

---

## Git Integration

EDOSH displays rich git repository information directly in your prompt:

### Prompt Format

```
/path/to/repo (branch ↑2 ↓1 +3 ~2 ?1) >
```

**Legend:**
- `branch` - Current branch name (or commit hash if detached)
- `↑2` - 2 commits ahead of origin
- `↓1` - 1 commit behind origin
- `+3` - 3 staged files (green)
- `~2` - 2 unstaged changes (red)
- `?1` - 1 untracked file (gray)

### Status Colors

- **Green parentheses**: Clean working directory, no changes
- **Yellow/Red parentheses**: Uncommitted changes detected
- **Red path**: Current working directory is displayed in red

### Example

```bash
cd ~/my-project
git status
# On branch main
# Your branch is ahead of 'origin/main' by 2 commits.
# Changes to be committed:
#   modified: src/main.c
#   new file: src/helper.c
# Changes not staged:
#   modified: README.md

# Your prompt shows:
/home/user/my-project (main ↑2 +2 ~1) >
```

---

## Code Runner

The `run` command compiles and executes source files in multiple languages:

### Supported Languages

| Language | Extensions | Compiler/Interpreter | Example |
|----------|-----------|---------------------|---------|
| C | `.c` | gcc | `run hello.c` |
| C++ | `.cpp`, `.cc`, `.cxx` | g++ | `run main.cpp` |
| Python | `.py` | python3 | `run script.py` |
| Java | `.java` | javac + java | `run Main.java` |

### Usage Examples

#### C Program
```bash
cat > hello.c << 'EOF'
#include <stdio.h>
int main() {
    printf("Hello from C!\n");
    return 0;
}
EOF

run hello.c
# Compiles to /tmp/edox_run_<pid> and executes
# Output: Hello from C!
```

#### C++ Program
```bash
cat > hello.cpp << 'EOF'
#include <iostream>
using namespace std;
int main() {
    cout << "Hello from C++!" << endl;
    return 0;
}
EOF

run hello.cpp
# Output: Hello from C++!
```

#### Python Script
```bash
echo 'print("Hello from Python!")' > hello.py
run hello.py arg1 arg2
# Arguments are passed to the script
```

#### Java Program
```bash
cat > Hello.java << 'EOF'
public class Hello {
    public static void main(String[] args) {
        System.out.println("Hello from Java!");
    }
}
EOF

run Hello.java
# Compiles and runs the Java program
```

### Features

- **Automatic Compilation**: C/C++ files are compiled to temporary executables
- **Argument Passing**: Additional arguments are forwarded to the program
- **Error Handling**: Compilation errors are displayed clearly
- **Cleanup**: Temporary files are automatically removed after execution

---

## Project Structure

```
edoX-shell/
├── src/
│   ├── main.c              # Shell loop, prompt, input handling
│   ├── builtins.c          # Built-in commands (cd, pwd, echo, run, etc.)
│   ├── executor.c          # External command execution, PATH resolution
│   ├── input_parser.c      # Token parsing with quote/escape handling
│   ├── autocomplete.c      # Tab completion for commands and files
│   ├── educational.c       # Tutorial, tips, and explain system
│   ├── help.c              # Help documentation for 60+ commands
│   ├── helpers.c           # Utility functions (string ops, etc.)
│   └── my_shell.h          # Header with function prototypes
├── codes/                  # Example code files for testing
│   ├── ct.c
│   ├── cppt.cpp
│   └── pt.py
├── Makefile               # Build configuration
├── README.md              # This file
└── edosh                  # Compiled binary (after make)
```

### Source File Responsibilities

| File | Lines | Purpose |
|------|-------|---------|
| `main.c` | ~550 | Shell main loop, raw terminal mode, prompt rendering |
| `builtins.c` | ~280 | Implementation of built-in commands |
| `executor.c` | ~150 | Fork/exec external commands, PATH searching |
| `input_parser.c` | ~90 | Tokenize input respecting quotes and escapes |
| `autocomplete.c` | ~325 | Tab completion logic for commands and files |
| `educational.c` | ~250 | Learning mode, tutorials, command explanations |
| `help.c` | ~260 | Help text for common Linux commands |
| `helpers.c` | ~120 | String utilities (strcmp, strdup, etc.) |
| `my_shell.h` | ~60 | Function prototypes and shared definitions |

**Total:** ~2000+ lines of C code

---

## Technical Details

### Architecture

EDOSH follows a modular design with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────────┐
│                        main.c                               │
│  ┌───────────┐  ┌──────────────┐  ┌──────────────────┐    │
│  │ Raw Mode  │  │ Prompt+Git   │  │ History+Signals  │    │
│  │ Terminal  │  │ Status       │  │ (Ctrl+C, arrows) │    │
│  └───────────┘  └──────────────┘  └──────────────────┘    │
└────────────────────────┬────────────────────────────────────┘
                         │
                         ▼
          ┌──────────────────────────────┐
          │     input_parser.c           │
          │  Tokenize with quotes/escapes│
          └──────────────┬───────────────┘
                         │
                         ▼
          ┌──────────────────────────────┐
          │      builtins.c              │
          │  cd, pwd, run, setenv, etc.  │
          └──────┬───────────────────────┘
                 │
       ┌─────────┴─────────┐
       ▼                   ▼
┌─────────────┐    ┌──────────────┐
│ executor.c  │    │ educational.c│
│ Fork+exec   │    │ Tips+Tutorial│
│ PATH search │    │ Explanations │
└─────────────┘    └──────────────┘
       │
       ▼
┌─────────────┐
│  helpers.c  │
│ String utils│
└─────────────┘
```

### Key Technologies

- **Language**: C (C99 standard)
- **Terminal Control**: POSIX termios for raw mode input
- **Process Management**: fork(), execve(), waitpid()
- **Signal Handling**: sigaction() for SIGINT
- **File I/O**: POSIX file operations, popen() for git commands
- **Memory Management**: Manual malloc/free with careful leak prevention

### Terminal Features

**Raw Mode Input**
- Character-by-character reading (no line buffering)
- Cursor positioning (left/right arrows)
- Line editing (insert, delete characters at cursor)
- History navigation (up/down arrows)
- Tab completion

**ANSI Escape Sequences**
- Colors: `\x1b[31m` (red), `\x1b[32m` (green), `\x1b[33m` (yellow), `\x1b[36m` (cyan)
- Cursor movement: `\x1b[C` (right), `\x1b[D` (left)
- Line clearing: `\x1b[K` (clear to end of line)

### Signal Handling

- **SIGINT (Ctrl+C)**: Doesn't exit shell, just prints newline
- **Child processes**: Inherit default SIGINT behavior (can be interrupted)
- **EOF (Ctrl+D)**: Exits shell cleanly

---

## Troubleshooting

### Common Issues

#### 1. Compilation Errors

**Problem**: `gcc: command not found`
```bash
# Solution: Install build tools
sudo apt install build-essential  # Ubuntu/Debian
sudo dnf groupinstall "Development Tools"  # Fedora
```

**Problem**: `fatal error: termios.h: No such file or directory`
```bash
# Solution: Install kernel headers
sudo apt install linux-libc-dev  # Ubuntu/Debian
sudo dnf install kernel-headers   # Fedora
```

#### 2. Runtime Issues

**Problem**: Tab autocomplete not working
```bash
# Check terminal type
echo $TERM  # Should be xterm-256color or similar

# Try setting better terminal
export TERM=xterm-256color
```

**Problem**: Colors not displaying
```bash
# Your terminal may not support 256 colors
# Try a different terminal emulator (gnome-terminal, konsole, alacritty)
```

**Problem**: `run` command fails to compile
```bash
# Check if compilers are installed
which gcc g++ python3 javac

# Install missing compilers:
sudo apt install gcc g++ python3 default-jdk  # Ubuntu
```

#### 3. Git Status Not Showing

**Problem**: Prompt doesn't show git branch
```bash
# Ensure you're in a git repository
git status

# Check if git is installed
git --version

# If not:
sudo apt install git  # Ubuntu
```

#### 4. Permission Denied

**Problem**: `./edosh: Permission denied`
```bash
# Make binary executable
chmod +x edosh
```

### Debug Mode

To troubleshoot issues, rebuild with debug symbols:

```bash
make clean
make CFLAGS="-Wall -Wextra -g -DDEBUG"

# Run with gdb
gdb ./edosh
```

---

## Acknowledgments

### Inspiration

- **Bash/Zsh**: For shell design patterns
- **Fish Shell**: For user-friendly features and suggestions
- **Explainshell.com**: For the idea of command explanations
- **Oh My Zsh**: For git prompt integration inspiration

### Technologies

- **POSIX**: For terminal control APIs (termios)
- **GNU GCC**: For C compilation
- **Git**: For version control and status integration

---

## Project Statistics

- **Language**: C (C99)
- **Lines of Code**: ~2000+
- **Files**: 9 source files + 1 header
- **Built-in Commands**: 15+
- **Supported Help Topics**: 60+ common Linux commands
- **Compilation Time**: < 2 seconds on modern hardware
- **Binary Size**: ~100KB (optimized build)
- **Supported Languages (run command)**: 4 (C, C++, Python, Java)

---

<div align="center">

**Made with care for students learning Linux**

Star us on GitHub if EDOSH helped you learn!

[Report Bug](https://github.com/cataclysmnik/edoX-shell/issues) • [Request Feature](https://github.com/cataclysmnik/edoX-shell/issues) • [Documentation](https://github.com/cataclysmnik/edoX-shell/wiki)

</div>
