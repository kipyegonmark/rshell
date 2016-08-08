#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Read a line from stdin
#define RSH_RL_BUFSIZE 1024
char *rsh_read_line(void)
{
  char *line = NULL;
  ssize_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

//Parse the line read from stdin
#define rsh_tok_bufsize 64
#define rsh_tok_delim "\t\r\n\a"
char **rsh_split_line(char *line)
{
  char *tokens = NULL;
  ssize_t bufsize = 0;
  getline(&tokens, &bufsize, strtok);
  return tokens;
}

//Loop to get input and execute
    void rsh_loop(void)
    {
        char *line;
        char **args;
        int status;

        do {
            printf("> ");
            line = rsh_read_line;
            args = rsh_split_line;
            status = rsh_execute(args);

            free(line);
            free(args);
        } while(status);
    }

//Function to launch a program
int rsh_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        //Child process
        if (execvp(args[0], args) == -1) {
            perror("rsh");
        }
        exit EXIT_FAILURE;
    }
    else if (pid < 0) {
        perror("rsh");
    } else {
        //Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    return 1;
}

//Declaration of functions for builtin shell commands
int rsh_cd(char **args);
int rsh_help(char **args);
int rsh_exit(char **args);

//List of builtin commands and their corresponding functions
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
  &rsh_cd,
  &rsh_help,
  &rsh_exit
};

int rsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

//Builtin function implementations.
int rsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "rsh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("rsh");
    }
  }
  return 1;
}

int rsh_help(char **args)
{
  int i;
  printf("RShell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following programs are built in:\n");

  for (i = 0; i < rsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int rsh_exit(char **args)
{
  return 0;
}

//Function to launch either programs or processes
int rsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < rsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return rsh_launch(args);
}

//The main function
int main(int argc, char **argv)
{
    //Load config files.

    //Run command loop.
    rsh_loop();

    //Perform cleanup and shutdown.
    return EXIT_SUCCESS
}