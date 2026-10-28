#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  int fd[2];

  if(pipe(fd) == -1) {
    perror("pipe");
    exit(1);
  }
  
  pid_t pid1 = fork();
  if(pid1 < 0) {
    perror("fork failed!");
    exit(1);
  } else if(pid1 == 0) {
    // Child process 1
    close(fd[0]); // Close read end
    dup2(fd[1], STDOUT_FILENO); // Redirect stdout to pipe
    close(fd[1]); // Close write end

    execlp("ls", "ls", NULL);
    perror("execlp");
    exit(1);
  }

  pid_t pid2 = fork();
  if(pid2 < 0) {
    perror("fork failed!");
    exit(1);
  } else if(pid2 == 0) {
    // Child process 2
    close(fd[1]); // Close write end
    dup2(fd[0], STDIN_FILENO); // Redirect stdin to pipe
    close(fd[0]); // Close read end

    close(STDOUT_FILENO);
    open("./hw_pipe.output", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

    execlp("wc", "wc", NULL);
    perror("execlp");
    exit(1);
  }
  
  // Parent process
  close(fd[0]);
  close(fd[1]);
  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);

  return 0;
}