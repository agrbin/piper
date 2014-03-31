/*
 * piper:
 *
 *    A.stdout  =>  B.stdin
 * +---+             +---+
 * | A |             | B |
 * +---+             +---+
 *    A.stdin   <=  B.stdout
 *
 * (ascii art is like a hundred words)
 */
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define max_args 1000

pid_t child_pid[2] = {0};
char* child_path[2], *self;
int pipes[2][2], it;

// if something goes wrong, kill them all.
void shutdown(const char *what) {
  fprintf(stderr, "%s: %s\n", self, what);
  if (child_pid[0]) kill(child_pid[0], 9);
  if (child_pid[1]) kill(child_pid[1], 9);
  exit(1);
}

// if child declaration has multiple words,
// convert them to argument list.
void exec_child(int child) {
  char *part, *args[max_args];
  if ((part = strtok(child_path[child], " ")) == NULL) {
    shutdown("child program path must not be empty");
  }
  for (it = 0 ;; ++it) {
    args[it] = part;
    if (part == NULL) break;
    part = strtok(NULL, " ");
  }
  execvp(args[0], args);
  shutdown("child failed to execlp.");
}

// do the connecting
// f-yeah.
void prepare_child(int child) {
  close(pipes[child][1]);
  dup2(pipes[child][0], 0);
  close(pipes[!child][0]);
  dup2(pipes[!child][1], 1);
}

// create pipes and fork-off childs.
void spawn_childs() {
  for (it = 0; it < 2; ++it)
    assert(!pipe(pipes[it]));
  for (it = 0; it < 2; ++it) {
    child_pid[it] = fork();
    switch (child_pid[it]) {
      case (pid_t) -1:
        shutdown("fork failed.");
      case 0:
        prepare_child(it);
        exec_child(it);
        // won't return
    }
  }
}

// wait for childs to finish
// TODO: non-expected way can be tested.
void wait_childs() {
  int status, sol = 0;
  for (it = 0; it < 2; ++it) {
    pid_t id = wait(&status);
    if (id == (pid_t) -1) {
      shutdown("wait failed.");
    }
    if (!WIFEXITED(status)) {
      shutdown("child exited (or stopped) in a non-expected way.");
    }
    if (WEXITSTATUS(status)) {
      shutdown("child has non zero exit status");
    }
  }
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "usage: %s first_program second_program\n", *argv);
    fprintf(stderr, "\n"
                    "this program will execute first and second program with\n"
                    "theirs stdouts and stdins cross-connected.\n"
                    "stderr stream is not redirected. exits with zero if both\n"
                    "programs exit with zero, not zero otherwise.\n");
    exit(1);
  }
  self = argv[0];
  child_path[0] = argv[1];
  child_path[1] = argv[2];
  spawn_childs();
  wait_childs();
  return 0;
}

