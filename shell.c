#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#define BUFFERSIZE 2048
#define COMMAND_SIZE 1024
#define NUM_OF_COMMANDS 2048

int split_pipes(char **commands[], char *currLine);
void pipeline(char ***cmd, int num_of_commands);
void create_command(char *command_with_args[], char *commandStr);
void free_commands(char **commands[], int num_of_commands);

/**
 * free up the heap memory that allocated for commands
*/
void free_commands(char **commands[], int num_of_commands) {
    for (int i = 0; i < num_of_commands; i++) {
        free(commands[i]);
    }
}

/**
 * Parse the inputs and split it by the pipe.
*/
int split_pipes(char **commands[], char *inputs) {
    char *single_command;
    int i = 0;
    while ((single_command = strtok_r(inputs, "|", &inputs)) != NULL) {
        int size = 10; 
        char **command_with_args = (char **) malloc(size * sizeof(char*));

        create_command(command_with_args, single_command);
        commands[i] = command_with_args;
        i++;
    }
    commands[i] = NULL;
    return i;
}

/**
 * Storing the commands with their arguments in an array where a command and
 * its arguments are stored in seperate slots.
*/
void create_command(char *command_with_args[], char *commandStr) {
    char *token;
    int index = 0;

    while ((token = strtok_r(commandStr, " ", &commandStr)) != NULL) {
        command_with_args[index] = token; 
        (index)++;
    }
    command_with_args[index] = NULL;

}


void execute_commands(char *currLine) {
    int num_of_commands;
    char **commands[NUM_OF_COMMANDS];
    
    // split the commands
    num_of_commands = split_pipes(commands, currLine);
    if (num_of_commands == 1) { // single command
        int rc = fork();
        int status;
        if (rc < 0){
            fprintf(stderr, "fork failed\n");
            exit(EXIT_FAILURE);
        } else if (rc == 0){ // child (new process)
            execvp(commands[0][0], commands[0]);
            printf("Job error: Command not found: %s\n", commands[0][0]);
            free_commands(commands, num_of_commands);
            exit(127);
            printf("this shouldn't print out.");
        } else { // parent process
            waitpid(rc, &status, 0);
            if (WIFEXITED(status)) {
            free_commands(commands, num_of_commands);
                printf("Job status: %d\n", WEXITSTATUS(status));
            }
        }
    } else if (num_of_commands > 1) { // pipe case
        pipeline(commands, num_of_commands);
        free_commands(commands, num_of_commands);
    }
}

void pipeline(char ***commands, int num_of_commands) {
    int pc = num_of_commands - 1; //number of pipes
    int i = 0; //command index
    int pipes[pc * 2];
    int status, rc;

    for (i = 0; i < pc; i++){
        if (pipe(pipes + i * 2) < 0){
            fprintf(stderr, "creating pipes failed\n");
            exit(1);
        }
    }
    i = 0;
    while (i < pc + 1) {
        rc = fork();
        if (rc < 0){
            fprintf(stderr, "fork failed\n");
            free_commands(commands, num_of_commands);
            exit(1);
        } else if (rc == 0){ // child process
            if (i != 0) { //if there is previous
            //connect previous's reading
                if (dup2(pipes[i * 2], 0) < 0){
                    fprintf(stderr, "dup2 failed\n");
                    free_commands(commands, num_of_commands);
                    exit(1);
                }
            //if there is next
            } if (i != pc) {
                //connect next's writing
                if (dup2(pipes[i * 2], 1) < 0){
                    fprintf(stderr, "dup2 failed\n");
                    free_commands(commands, num_of_commands);
                    exit(1);
                }
            }
            //reset all fds
            for (int j = 0; j < 2 * pc; j++){
                close(pipes[j]); 
            }
            //execute commands
            if (execvp(commands[i][0], commands[i]) < 0){
                printf("Job error: Command not found: %s\n", (*commands)[0]);
                free_commands(commands, num_of_commands);
                exit(127);
            }
        }
        i++;
    }

    //close fds in parent process
    for (i = 0; i < 2 * pc; i++){
        close(pipes[i]);
    }
    //wait for children
    for (i = 0; i < pc +1; i++){
        waitpid(rc, &status, 0);
    }
    //print status of last children
    printf("Job status: %d\n", WEXITSTATUS(status));
}



int main(int argc, char *argv[]) {
    (void) argv;
    if (argc > 1) {
        printf("Usage: ./shell\n");
    }
    char commands[BUFFERSIZE];
    while(1) {
        // char currLine;
        printf("Jobsh$ ");

        //read in command
        if (!fgets(commands, BUFFERSIZE, stdin)) {
            printf("\n");
            break; //terminate if reach EOF or raise an error
        }
        commands[strcspn(commands, "\n")] = 0; //remove the newline char
        if (strcmp(commands, "exit") == 0) {
            // printf("exit pressed\n");
            exit(EXIT_SUCCESS);
        }
        else {
            execute_commands(commands);
        }
        
    }
    return 0;
}

