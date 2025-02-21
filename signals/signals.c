#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <string.h>    /* memset                                         */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */

/* first, define the Ctrl-C counter, initialize it with zero. */
int ctrl_c_count = 0;
int got_response = 0;
#define CTRL_C_THRESHOLD  5 

/* the Ctrl-C signal handler */
void catch_int(int sig_num)
{
  /* increase count, and check if threshold was reached */
  ctrl_c_count++;
  if (ctrl_c_count >= CTRL_C_THRESHOLD) {
    char answer[30];

    /* prompt the user to tell us if to really
     * exit or not */
    printf("\nReally exit? [Y/n]: ");
    alarm(3);
    fflush(stdout);
    fgets(answer, sizeof(answer), stdin);
    if (answer[0] == 'n' || answer[0] == 'N') {
      printf("\nContinuing\n");
      fflush(stdout);
      /* 
       * Reset Ctrl-C counter
       */
      ctrl_c_count = 0;
    }
    else {
      printf("\nExiting...\n");
      fflush(stdout);
      exit(0);
    }
  }
}

/* the Ctrl-Z signal handler */
void catch_tstp(int sig_num)
{
  /* print the current Ctrl-C counter */
  
  printf("\n\nSo far, '%d' Ctrl-C presses were counted\n\n", ctrl_c_count);
  fflush(stdout);
}


void catch_alarm(int sig_num) 
{
printf("\nUser taking too long to respond. Exiting....");//Show that this function is called
fflush(stdout); //clear terminal
exit(0); //exits program with 0 code (program exit with no errors)
}
/* STEP - 1 (20 points) */
/* Implement alarm handler - following catch_int and catch_tstp signal handlers */
/* If the user DOES NOT RESPOND before the alarm time elapses, the program should exit */
/* If the user RESPONDEDS before the alarm time elapses, the alarm should be cancelled */
//YOUR CODE

int main(int argc, char* argv[])
{
  struct sigaction sa;
  struct sigaction *sa_ptr = &sa; //We need pointer to clear data

  /*
  memset starts at the pointer given. Fills everything with given value, 0, in this case,
  and goes until it reaches the end, size of sa in this case.
  */
  memset(sa_ptr, 0, sizeof(sa));
  /* STEP - 2 (10 points) */
  /* clear the memory at sa - by filling up the memory location at sa with the value 0 till the size of sa, using the function memset */
  /* type "man memset" on the terminal and take reference from it */
  /* if the sa memory location is reset this way, then no garbage value can create undefined behavior with the signal handlers */
  //YOUR CODE


  sigset_t mask_set;  /* mask_set stores signals that are to be blocked or not */

  /* STEP - 3 (10 points) */
  /* setup mask_set - fill up the mask_set with all the signals to block*/
  //YOUR CODE

  //sigfillset adds every single signal to a mask set.
  sigfillset(&mask_set);


  /* STEP - 4 (10 points) */
  /* ensure in the mask_set that the alarm signal does not get blocked while in another signal handler */
  //YOUR CODE

  //sigdelset removes a signal from the mask set
  sigdelset(&mask_set, SIGALRM);
  /* STEP - 5 (20 points) */
  /* set signal handlers for SIGINT, SIGTSTP and SIGALRM */
  //YOUR CODE
  

  //sa_handler is a pointer to the handler function that will be called when the signal is received.Use this when the
  //handler does not need additional information other than the signal number.You will use this in this lab to store a
  //pointer to each handler function.

  //assign the handler value of the sigaction struct to the handler function we defined
  sa.sa_handler = catch_int;
  
  //sigaction is a function that actually makes the system call/process that will lookout for signals
  //It needs the signal that its looking for, pointer to the sigaction struct or "instance"
  //We need three because we are "overiding" the sa_hander attribute for our struct and making three different calls

  sigaction(SIGINT, sa_ptr, NULL);


  sa.sa_handler = catch_tstp;
  sigaction(SIGTSTP, sa_ptr, NULL);

  sa.sa_handler = catch_alarm;
  sigaction(SIGALRM, sa_ptr, NULL);

  //alarm(30) simply sets the alarm to 30 seconds,
  
  /* STEP - 6 (10 points) */
  /* ensure that the program keeps running to receive the signals */
  //YOUR CODE
  while (1) {
      pause(); //a function that waits for signals
  }
  return 0;
}

