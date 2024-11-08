#line 2 "sub-process.c"
#ifndef _SUB_PROCESS_H_
#define _SUB_PROCESS_H_

/**
 * @file sub-process.c
 *
 * Utility to launch a sub-processes and interfact with them.
 *
 * (This version will probably only work on Unix like operating
 * systems but we should be able to make an MS-Windows version at some
 * point.)
 */

/**
 * @enum sub_process_exit_status_t
 */
typedef enum {
  EXIT_STATUS_UNKNOWN,
  EXIT_STATUS_NORMAL_EXIT,
  EXIT_STATUS_SIGNAL,
  EXIT_STATUS_ABNORMAL
} sub_process_exit_status_t;

typedef struct {
  // launch parameters
  value_array_t* argv;             // argv[0] is the program executable path

  // child process information
  pid_t pid;
  int stdin;
  int stdout;
  int stderr;
  
  // exit info
  sub_process_exit_status_t exit_status;
  int exit_code;
  int exit_signal;
} sub_process_t;

sub_process_t* make_sub_process(value_array_t* argv);

boolean_t sub_process_launch(sub_process_t* sub_process);

void sub_process_write(sub_process_t* sub_process, buffer_t* data);

void sub_process_read(sub_process_t* sub_process, buffer_t* stdout, buffer_t* stderr);

void sub_process_wait(sub_process_t* sub_process);

#endif /* _SUB_PROCESS_H_ */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/**
 * @function make_sub_process
 *
 * Make and return an *unlaunched* sub-process.
 */
sub_process_t* make_sub_process(value_array_t* argv) {
  sub_process_t* result = malloc_struct(sub_process_t);
  result->argv = argv;
  result->exit_code = -1;
  return result;
}

/**
 * @function sub_process_launch
 *
 * Do the actual sub process launch using the "arguments" in
 * sub_process_t.
 */
boolean_t sub_process_launch(sub_process_t* sub_process) {
  uint64_t length = sub_process->argv->length;
  if (length < 1) {
    log_fatal("Expected at least the program path in argv");
    fatal_error(ERROR_ILLEGAL_STATE);
  }

  // 1. Convert value_array_t to char** for execvp
  char** argv = cast(char**, malloc_bytes((length + 1) * sizeof(char*)));
  for (int i = 0; i < length; i++) {
    argv[i] = value_array_get_ptr(sub_process->argv, i, typeof(char*));
  }
  char** envp = NULL;

  // 2. Create pipes for stdin of the sub process as well as to
  // capture stdout and stderr.
  int stdin_pipe[2];
  int stdout_pipe[2];
  int stderr_pipe[2];
  if (pipe(stdin_pipe) == -1
      || pipe(stdout_pipe) == -1
      || pipe(stderr_pipe) == -1) {
    log_fatal("Failed to create pipe for stdin, stdout or stderr");
    fatal_error(ERROR_ILLEGAL_STATE);
    return false;
  }

  // 3. Fork the process
  pid_t pid = fork();
  if (pid == -1) {
    log_fatal("fork() failed.");
    fatal_error(ERROR_ILLEGAL_STATE);
    return false;
  }

  if (pid == 0) {
    /* ====================================================================== */
    /* Child Process */
    /* ====================================================================== */

    // 4. Redirect stdin, stdout and stderr to pipes
    dup2(stdin_pipe[0], STDIN_FILENO);
    dup2(stdout_pipe[1], STDOUT_FILENO);
    dup2(stderr_pipe[1], STDERR_FILENO);
    close(stdin_pipe[0]);
    close(stdin_pipe[1]);
    close(stdout_pipe[0]);
    close(stdout_pipe[1]);
    close(stderr_pipe[0]);
    close(stderr_pipe[1]);

    // 5. Final "exec" to the command
    execvp(argv[0], argv);

    // execvp should not return!
    fatal_error(ERROR_ILLEGAL_STATE);
    return false;
  } else {
    /* ====================================================================== */
    /* Parent Process */
    /* ====================================================================== */

    // 6. Close write ends of the pipes in the parent since we will
    // only be reading from the pipe.
    close(stdin_pipe[0]);   // Close read end of stdin pipe
    close(stdout_pipe[1]);  // Close write end of stdout pipe
    close(stderr_pipe[1]);  // Close write end of stderr pipe

    // 7. Record the pid, stdout, and stderr.
    sub_process->pid = pid;
    sub_process->stdin = stdin_pipe[1];
    sub_process->stdout = stdout_pipe[0];
    sub_process->stderr = stderr_pipe[0];

    free_bytes(argv);

    return true;
  }
}

void sub_process_write(sub_process_t* sub_process, buffer_t* data) {
  // FIXME
}

void sub_process_read(sub_process_t* sub_process, buffer_t* stdout, buffer_t* stderr) {
  if (stdout != NULL) {
    buffer_read_ready_bytes_file_number(stdout, sub_process->stdin, 0xffffffff);
  }
  if (stderr != NULL) {
    buffer_read_ready_bytes_file_number(stderr, sub_process->stderr, 0xffffffff);
  }
}

boolean_t is_sub_process_running(sub_process_t* sub_process) {
  int status;
  pid_t result = waitpid(sub_process->pid, &status, WNOHANG);

  if (result == sub_process->pid) { 
    return false;
  } else if (result == 0) {
    return true;
  } else { 
    return false;
  }
}

void sub_process_wait(sub_process_t* sub_process) {
  int status = 0;
  if (waitpid(sub_process->pid, &status, 0) == -1) {
    sub_process->exit_status = EXIT_STATUS_ABNORMAL;
    return;
  }
  
  // Check the exit status and return the exit code
  if (WIFEXITED(status)) {
    sub_process->exit_status = EXIT_STATUS_NORMAL_EXIT;
    sub_process->exit_code = WEXITSTATUS(status);
  } else if (WIFSIGNALED(status)) {
    sub_process->exit_status = EXIT_STATUS_SIGNAL;
    sub_process->exit_signal = WTERMSIG(status);
  } else {
    sub_process->exit_status = EXIT_STATUS_ABNORMAL;
  }
}
