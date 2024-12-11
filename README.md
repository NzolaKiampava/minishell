# minishell
Minishell 42 Project


minishell/
├── Makefile
├── includes/
│   └── minishell.h
├── libft/
│   ├── Makefile
│   └── [seus arquivos libft]
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
