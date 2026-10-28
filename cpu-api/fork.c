#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  // Close standard output redirect to file
  close(STDOUT_FILENO);
  // open("./hw.output", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
  
  int x = 100;

  pid_t pid = fork();
  if(pid < 0) {
    // Fork failed
    printf("Fork failed\n");
    exit(1);
  } else if(pid == 0) {
      // Child process
    printf("hello\n");
    // fflush(stdout);

    execlp("ls", "ls", NULL);

    printf("Child x: %d\n",x);
    x = 101;
    printf("Child x: %d\n",x);
    
    exit(0);
  } else {
    // Parent process
    int *status;
    printf("wait returns: %d -- pid: %d\n", waitpid(pid, status, NULL), pid);
    printf("Status: %d\n", status);
    printf("goodbye\n");

    printf("Parent x: %d\n",x);
    x = 202;
    printf("Parent x: %d\n",x);

    exit(0);
  }
  return 0;
}