# Minishell

A simple shell implementation inspired by bash, developed as part of the 42 School curriculum.

<img src="https://img.shields.io/badge/Grade-100%2F100-success"></img>


## 📝 About

Minishell is a simplified shell that implements basic functionality similar to bash. This project challenges students to understand how shells work, process management, and file descriptors while building a functional command-line interpreter.

## ✨ Features

- Display a prompt while waiting for a new command
- Working history management
- Search and execute commands based on PATH variable or using relative/absolute path
- Handle quotes (`'` and `"`) and special characters
- Implement redirections (`<`, `>`, `<<`, `>>`)
- Implement pipes (`|`)
- Handle environment variables (`$` followed by characters)
- Handle `$?` which expands to the exit status
- Handle signals (`ctrl-C`, `ctrl-D`, `ctrl-\`)

### Built-in Commands

- `echo` with option `-n`
- `cd` with relative or absolute path
- `pwd` without any options
- `export` without any options
- `unset` without any options
- `env` without any options
- `exit` without any options

## 🛠️ Installation

```bash
# Clone the repository
git clone https://github.com/NzolaKiampava/minishell.git

# Navigate to the project directory
cd minishell

# Compile the project
make

# Run minishell
./minishell
```

## 💻 Usage

```bash
# Basic command execution
$> ls -la

# Using pipes
$> ls | grep filename

# Input/Output redirections
$> echo "hello" > output.txt
$> cat < input.txt

# Environment variables
$> echo $PATH

# Built-in commands
$> cd ..
$> pwd
$> echo "Hello World"
```

## ⚙️ Project Structure

```
minishell/
├── Makefile
├── includes/
│   └── minishell.h
├── libft/
│   ├── Makefile
│   └── [libft source files]
├── src/
│   ├── main.c
│   ├── builtins/
│   │   ├── cd.c
│   │   ├── echo.c
│   │   ├── env.c
│   │   ├── exit.c
│   │   ├── export.c
│   │   ├── pwd.c
│   │   └── unset.c
│   ├── executor/
│   │   ├── executor.c
│   │   ├── pipe_handler.c
│   │   └── redirections.c
│   ├── parser/
│   │   ├── parser.c
│   │   ├── tokenizer.c
│   │   └── expander.c
│   ├── signals/
│   │   └── signals.c
│   └── utils/
│       ├── environment.c
│       └── error_handler.c
```

### Component Description

1. **Builtins (`src/builtins/`)**
   - Implementation of all required shell built-in commands
   - Each command in a separate file for better organization

2. **Executor (`src/executor/`)**
   - `executor.c`: Main command execution logic
   - `pipe_handler.c`: Handles pipe operations between commands
   - `redirections.c`: Manages input/output redirections

3. **Parser (`src/parser/`)**
   - `parser.c`: Command line parsing and structure creation
   - `tokenizer.c`: Splits input into tokens
   - `expander.c`: Handles environment variable expansion

4. **Signals (`src/signals/`)**
   - `signals.c`: Handles all signal-related functionality

5. **Utils (`src/utils/`)**
   - `environment.c`: Environment variable management
   - `error_handler.c`: Error handling and messaging

## 🔍 Testing

The shell has been tested with:
- Basic command execution
- Complex pipe sequences
- Various types of redirections
- Edge cases with quotes and special characters
- Memory leak checks using Valgrind
- Error handling scenarios

## ⚠️ Known Limitations

- Does not handle advanced features like wildcards (`*`)
- No command history file persistence between sessions
- Limited signal handling compared to bash

## 📚 Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [Writing Your Own Shell](https://www.geeksforgeeks.org/making-linux-shell-c/)

## 👥 Contributors

- [Nzola Kiampava](https://github.com/NzolaKiampava)
- [Manuel de Almeida](https://github.com/ManuelAlmeida70)

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- 42 School for the challenging project
- Fellow 42 students for testing and feedback
- The open-source community for valuable resources
