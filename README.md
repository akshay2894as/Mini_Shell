# Mini_Shell
A Linux-based Mini Shell developed in C with process management, signal handling, built-in commands, background/foreground jobs, and pipe support.

# Mini Shell

A Linux-based **Mini Shell** developed in C that provides a command-line interface for executing built-in and external Linux commands. The project demonstrates process creation, process management, signal handling, job control, command execution, and inter-process communication.

## 📌 Project Overview

Mini Shell is a simplified implementation of a Linux command-line shell. It accepts commands from the user, identifies the command type, and executes the required operation.

The shell supports built-in commands, external Linux commands, foreground and background processes, signal handling, job management, and basic pipe functionality.

## ✨ Features

* Execute external Linux commands
* Support for built-in commands
* Process creation using `fork()`
* Command execution using `execvp()`
* Process synchronization using `waitpid()`
* Foreground and background process execution
* Job management
* Signal handling
* Pipe support between commands
* Custom shell prompt
* Environment variable handling
* Command parsing and validation
* Exit command to terminate the shell

## 🛠️ Technologies Used

* C Programming
* Linux
* GCC Compiler
* Linux System Calls
* Process Management
* Signals
* Pipes
* File Descriptors
* Dynamic Memory Allocation
* String Manipulation

## 🔧 System Calls Used

The project demonstrates the use of important Linux system calls and functions such as:

* `fork()`
* `execvp()`
* `waitpid()`
* `pipe()`
* `dup2()`
* `kill()`
* `signal()`
* `getpid()`
* `getppid()`

## 📂 Project Structure

```text
Mini-Shell/
│
├── main.c
├── minishell.c
├── minishell.h
├── execute.c
├── signal.c
├── pipe.c
├── builtins.c
├── jobs.c
├── Makefile
└── README.md
```

> The file names above should match your actual source files in the repository. Remove or modify any file names that are not present in your project.

## 🚀 How to Compile

Clone the repository:

```bash
git clone <YOUR-GITHUB-REPOSITORY-URL>
```

Navigate to the project directory:

```bash
cd Mini-Shell
```

Compile the project:

```bash
gcc *.c
```

Run the shell:

```bash
./a.out
```

If your project contains a Makefile, use:

```bash
make
```

and then run:

```bash
./minishell
```

## 💻 Example Commands

The shell can be used to execute commands such as:

```bash
pwd
ls
ls -l
date
whoami
echo Hello
```

### Built-in Commands

Examples include:

```bash
cd
pwd
echo
jobs
fg
bg
exit
```

### Background Process

Commands can be executed in the background using:

```bash
sleep 10 &
```

### Pipe

Commands can be connected using a pipe:

```bash
ls | wc
```

## 🔄 Working Principle

```text
             User Input
                 |
                 v
          Command Parsing
                 |
                 v
        Command Classification
          /              \
         /                \
   Built-in Command    External Command
         |                  |
         v                  v
     Execute Directly     fork()
                            |
                            v
                         execvp()
                            |
                            v
                      Process Execution
                            |
                            v
                    wait / Background Job
```

## 📚 Concepts Learned

Through this project, I gained practical knowledge of:

* Linux process creation and management
* Parent and child processes
* Process synchronization
* Foreground and background execution
* Signal handling
* Inter-process communication
* Pipes and file descriptors
* Linux system calls
* Command parsing
* Job control
* Shell architecture

## 🎯 Key Challenges

* Handling foreground and background processes correctly
* Managing signals such as `SIGINT`, `SIGTSTP`, and `SIGCHLD`
* Maintaining a list of active jobs
* Implementing built-in commands
* Managing parent-child process relationships
* Implementing pipe-based communication between processes
* Correctly handling command parsing and execution

## 👨‍💻 Author

**Akshay Suryavanshi**

Electronics and Communication Engineering

### GitHub

https://github.com/akshay2894as

### LinkedIn

https://www.linkedin.com/in/akshay-suryavanshi-a5015b359

## 📄 License

This project is developed for educational and learning purposes.
