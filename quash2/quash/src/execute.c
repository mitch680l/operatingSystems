/**
 * @file execute.c
 *
 * @brief Implements interface functions between Quash and the environment and
 * functions that interpret an execute commands.
 *
 * @note As you add things to this file you may want to change the method signature
 */

 #include "execute.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h> //WNOHANG
#include <sys/types.h> //pid_t
#include <unistd.h>
 
 #include "quash.h"
 #include "limits.h" //defines maximum length for a path in linux
 #include "deque.h"
 
 IMPLEMENT_DEQUE_STRUCT(pqueue, pid_t);
 IMPLEMENT_DEQUE(pqueue, pid_t);
 pqueue pque;
 struct job {
   int job_id; //Simple identify
   char* cmd; //The command the job is doing
   pqueue pque; //The processes that the job contains
   pid_t pid;
 }job;
 
 //We need two queue structurse. One for processes and one for jobs. Jobs will contain a que for its processes, since it can contain multple.
 
 
 
 IMPLEMENT_DEQUE_STRUCT(jqueue, struct job);
 IMPLEMENT_DEQUE(jqueue, struct job);
 
 jqueue jque;
 bool jque_initialized = false;
 /***************************************************************************
  * Interface Functions
  ***************************************************************************/
 
 
 // Return a string containing the current working directory.
 char* get_current_directory(bool* should_free) {
   char *cwd = (char*) malloc(PATH_MAX * sizeof(char));
   if (cwd == NULL) {
     perror("Failed to allocate memory in get_current_directory");
     *should_free = false;
     return NULL;
   }
 
   if (getcwd(cwd, PATH_MAX) == NULL) {
     perror("Failed to get directory in get_current_directory");
     free(cwd);
     *should_free = false;
     return NULL;
   }
 
   *should_free = true;
   return cwd;
 }
 // Returns the value of an environment variable env_var
 const char* lookup_env(const char* env_var) {
   if (env_var == NULL) {
     perror("lookup_env invalid input (NULL)");
        }
   char *path_env = getenv(env_var); 
   if(path_env == NULL) {
     perror("Failed to get env_var enviroment in lookup_env");
        }
   return path_env;
 }


 // Check the status of background jobs
 void check_jobs_bg_status() {
   //Enter global job queue
   while(!is_empty_jqueue(&jque)) {
     struct job current_job = pop_front_jqueue(&jque);
 
     //Enter current job's process queue
     while(!is_empty_pqueue(&current_job.pque)) {
       pid_t current_pid = pop_front_pqueue(&current_job.pque);
       int return_status;
       //WNOHANG means that waitpid will not block running processes
       if (waitpid(current_pid, &return_status, WNOHANG) == 0) {
         push_back_pqueue (&current_job.pque, current_pid);
       }
     }
 
     print_job_bg_complete(current_job.job_id, current_job.pid, current_job.cmd);
   }
 }
 void print_job(int job_id, pid_t pid, const char* cmd) {
   printf("[%d]\t%8d\t%s\n", job_id, pid, cmd);
   fflush(stdout);
 }
 void print_job_bg_start(int job_id, pid_t pid, const char* cmd) {
   printf("Background job started: ");
   print_job(job_id, pid, cmd);
 }
 void print_job_bg_complete(int job_id, pid_t pid, const char* cmd) {
   printf("Completed: \t");
   print_job(job_id, pid, cmd);
 }
 void run_generic(GenericCommand cmd) {
   char* exec = cmd.args[0];
   char** args = cmd.args;
   //execvp(file, argv[]);
   execvp(exec, args);
   //execvp creates new process, so if it reaches this line, it has failed to do so.
   perror("ERROR: Failed to execute program");
 }
 
 
 void run_echo(EchoCommand cmd) {
   // Print an array of strings. The args array is a NULL terminated (last
   // string is always NULL) list of strings.
   char** str = cmd.args;
 
   while (*str != NULL) {
     printf("%s", *str);
     str++;
   }
   printf("\n");
   fflush(stdout);
 }
 
 // Sets an environment variable
 void run_export(ExportCommand cmd) {
   const char* env_var = cmd.env_var;
   const char* val = cmd.val;
 
   //setenv(name, value, overwrite)
   //if overwrite != then it overwrites.
   //Returns 0 on success
   if (setenv(env_var,val,1) != 0) {
     perror("run_export failed to setenv(3)");
   } 
 }
 
 // Changes the current working directory
 void run_cd(CDCommand cmd) {
   // Get the directory name
   const char* dir = cmd.dir;
   char absolute_path[PATH_MAX]; //store the actual path
   char old_path[PATH_MAX]; //store the old path(OPTIONAL)
 
 
   // Check if the directory is valid
   if (dir == NULL) {
     perror("Incorrect input (dir) in run_cd");
   }
 
   if (getcwd(old_path, sizeof(old_path)) == NULL) {
     perror("Failed to get current working directory  in run_cd");
   }
 
 
   //This function can turn relative paths to their absolute
   if (realpath(dir, absolute_path) == NULL) {
     perror("Failed find absolute path of dir in run_cd");
   }
 
   //chdir changes the directory
   if (chdir(absolute_path) != 0) {
     perror("Failed to change directory in run_cd");
   }
   //set enviroment variables
   setenv("OLD_PWD",old_path,1);
   setenv("PWD",absolute_path,1);
 }
 
 // Sends a signal to all processes contained in a job
 void run_kill(KillCommand cmd) {
   int signal = cmd.sig;
   int job_id = cmd.job;
 
   int qsize = length_jqueue(&jque);
   for (int i = 0; i < qsize; i++) {
     struct job curr_job = pop_front_jqueue(&jque);
     if (curr_job.job_id == job_id) {
       while(!is_empty_pqueue(&curr_job.pque)) {
           pid_t curr_pid = pop_front_pqueue(&curr_job.pque);
 
           if (kill(curr_pid, signal) != 0) {
             perror("Failed to kill process in run_kill");
           }
 
           push_back_pqueue(&curr_job.pque, curr_pid);
         }
       }
       push_back_jqueue(&jque, curr_job);
     }
     
   }
 
 
 // Prints the current working directory to stdout
 void run_pwd() {
   char dir[PATH_MAX];
 
   if (getcwd(dir, sizeof(dir)) == NULL) {
     perror("Failed to get current working directory  in run_cd");
   }
 
   printf("%s",dir);
   fflush(stdout);
 }
 
 // Prints all background jobs currently in the job list to stdout
 void run_jobs() {
   
   int size = length_jqueue(&jque);
   for (int i = 0; i < size; i++ ) {
     struct job curr_temp = pop_front_jqueue(&jque);
     print_job(curr_temp.job_id, curr_temp.pid, curr_temp.cmd);
     push_back_jqueue(&jque, curr_temp);
   }
   fflush(stdout);
 }
 
 /***************************************************************************
  * Functions for command resolution and process setup
  ***************************************************************************/
 
 /**
  * @brief A dispatch function to resolve the correct @a Command variant
  * function for child processes.
  *
  * This version of the function is tailored to commands that should be run in
  * the child process of a fork.
  *
  * @param cmd The Command to try to run
  *
  * @sa Command
  */
 void child_run_command(Command cmd) {
   CommandType type = get_command_type(cmd);
 
   switch (type) {
   case GENERIC:
     run_generic(cmd.generic);
     break;
 
   case ECHO:
     run_echo(cmd.echo);
     break;
 
   case PWD:
     run_pwd();
     break;
 
   case JOBS:
     run_jobs();
     break;
 
   case EXPORT:
   case CD:
   case KILL:
   case EXIT:
   case EOC:
     break;
 
   default:
     fprintf(stderr, "Unknown command type: %d\n", type);
   }
 }
 
 /**
  * @brief A dispatch function to resolve the correct @a Command variant
  * function for the quash process.
  *
  * This version of the function is tailored to commands that should be run in
  * the parent process (quash).
  *
  * @param cmd The Command to try to run
  *
  * @sa Command
  */
 void parent_run_command(Command cmd) {
   CommandType type = get_command_type(cmd);
 
   switch (type) {
   case EXPORT:
     run_export(cmd.export);
     break;
 
   case CD:
     run_cd(cmd.cd);
     break;
 
   case KILL:
     run_kill(cmd.kill);
     break;
 
   case GENERIC:
   case ECHO:
   case PWD:
   case JOBS:
   case EXIT:
   case EOC:
     break;
 
   default:
     fprintf(stderr, "Unknown command type: %d\n", type);
   }
 }
 
 /**
  * @brief Creates one new process centered around the @a Command in the @a
  * CommandHolder setting up redirects and pipes where needed
  *
  * @note Processes are not the same as jobs. A single job can have multiple
  * processes running under it. This function creates a process that is part of a
  * larger job.
  *
  * @note Not all commands should be run in the child process. A few need to
  * change the quash process in some way
  *
  * @param holder The CommandHolder to try to run
  *
  * @sa Command CommandHolder
  */
 void create_process(CommandHolder holder) {


   // Read the flags field from the parser
   bool p_in  = holder.flags & PIPE_IN;
   bool p_out = holder.flags & PIPE_OUT;
   bool r_in  = holder.flags & REDIRECT_IN;
   bool r_out = holder.flags & REDIRECT_OUT;
   bool r_app = holder.flags & REDIRECT_APPEND; // This can only be true if r_out
                                                // is true
 
   int p1[2];
   /*
   TODO
 
   * maybe not enough pipes. Need some way to detect # of processes
   * add the process to the deque.
 
   */
   if(p_in || p_out) {
     if(pipe(p1) == -1) {
       perror("Failed to make pipe in create_process");
     }
   }
 
   pid_t pid = fork();
 
   push_back_pqueue(&pque, pid);
   //child
   if (pid == 0) {
     if (p_in) {
       close(p1[1]);
       dup2(p1[0],STDIN_FILENO);
       close(p1[0]);
     }
 
     if(p_out) {
       close(p1[0]);
       dup2(p1[1],STDOUT_FILENO);
       close(p1[1]);
     }
     //fopen takes in filename and a mode
     //r = read, w = writing, a = append
     if (r_in) {
       
       FILE *file = fopen(holder.redirect_in, "r");
       dup2(fileno(file), STDIN_FILENO);
     }
 
     if (r_out) {
       //r_app only true when r_out true
       if(r_app) {
         FILE *file = fopen(holder.redirect_out, "a");
         dup2(fileno(file), STDIN_FILENO);
       }
       else {
         FILE *file = fopen(holder.redirect_out, "a");
         dup2(fileno(file), STDIN_FILENO);
       }
     }
     child_run_command(holder.cmd);
     exit(1);
   }
   else {
     parent_run_command(holder.cmd);
   }
 
 }
 
 // Run a list of commands
 void run_script(CommandHolder* holders) {
  if (holders == NULL)
     return;
  if (!jque_initialized) {
    jque = new_jqueue(10);
    jque_initialized = true;
  }
   check_jobs_bg_status();
 
   if (get_command_holder_type(holders[0]) == EXIT &&
       get_command_holder_type(holders[1]) == EOC) {
     end_main_loop();
     return;
   }
 
   CommandType type;
 
   // Run all commands in the `holder` array
   for (int i = 0; (type = get_command_holder_type(holders[i])) != EOC; ++i) {
   
     create_process(holders[i]);
   
   }
  
   pque = new_pqueue(1);
 
   if (!(holders[0].flags & BACKGROUND)) {
     // Not a background Job
     // TODO: Wait for all processes under the job to complete
     pid_t curr_pid;
     while(!is_empty_pqueue(&pque)) {
       curr_pid = pop_front_pqueue(&pque);
       int return_status;
       waitpid(curr_pid, &return_status, 0);
     }
     destroy_pqueue(&pque);
   }
   else {
     struct job curr_job;
     curr_job.job_id = length_jqueue(&jque) + 1;
     curr_job.cmd = get_command_string();
     curr_job.pid = peek_back_pqueue(&pque);
     curr_job.pque = pque;
 
     push_back_jqueue(&jque, curr_job);
     print_job_bg_start(curr_job.job_id, curr_job.pid, curr_job.cmd);
   }
 }
