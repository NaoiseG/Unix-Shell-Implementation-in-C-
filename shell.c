#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

size_t MAXSIZE = 256;
#define SIZE_ARGV 16

char *command;
char *argv[16];
char *fp = NULL;
struct tm*current_time;

int string_array() //Function to turn command string into array of arguments
{
    char *tok; //Token variable
    int i = 0;

    tok = strtok(command, " "); //Create tokens from command string separated by spaces
    while(tok != NULL) //Loop to set aguments to token value
    {
        if(strcmp(tok, ">") == 0) //Check for redirect
        {
            tok = strtok(NULL, " ");
            fp = tok; //Copy filename
            break;
        }
        else
        {
            argv[i] = tok; 
            i++;
            tok = strtok(NULL, " ");
        }
	}
    argv[i] = NULL; //Set last value of argv to null pointer

    return i; //Return number of arguments
}

void display_prompt() //Function to display prompt
{
    time_t rawtime;
    struct tm *current_time;
    char buf[80];

    time(&rawtime);

    current_time = localtime(&rawtime); //Convert to local time

    strftime(buf,80,"%d/%m %I:%M%p", current_time);
    printf("|%s|  #", buf);
}


int read_command() //Function to read command from standard input
{
    display_prompt();

    char *buf = malloc(sizeof(char)*MAXSIZE); //Create buffer for command
    if(getline(&buf, &MAXSIZE, stdin) == -1) //Read buffer from standard input using getline
    {
        printf("^D\n"); //If ctrl + D inputted shell exits
        exit(1);
    }

    command = malloc(sizeof(char) * (strlen(buf) + 1)); 
    strcpy(command, buf); //Copy buffer string to command
    free(buf);

    if ((strlen(command) > 0) && (command[strlen (command) - 1] == '\n')) //If command was received successfully
    {
       command[strlen (command) - 1] = '\0'; //Trim trailing newline 
    }

    return 0;
}

void handle(int signal) //Function to handle signal when ctrl+C entered
{
    printf("\n");
    display_prompt(); //Display prompt again

    fflush(stdout); //Flush standard output
}



int shell()
{
    while(1) //Infinite loop to run until told to stop
    {
        signal(SIGINT, handle);

        if(read_command() == -1) //If failed to read command 
        {
            return -1;
        }
        int argc = string_array(); //Call string_array function and set argc variable to number of arguments

        if(!strcmp("exit", argv[0]))
        {
            return 0;
        }

         else if(!strcmp("cd", argv[0])) //Check for cd function
        {
            if(argv[1] == NULL) //If no path specified change to environment home
            {
                if(chdir(getenv("HOME")) != 0)
            {
                perror("Failed to change directory"); 
            }
            }

            else if(chdir(argv[1]) != 0) //Change to directory specified
            {
                perror("Failed to change directory"); 
            }
        }

        else
        {
            pid_t pid;
            pid = fork();
            if(pid == -1) //Failed
            {
                printf("Error: Failed to ceate child process\n");
                return -1;
            }

            else if(pid == 0) //Child process
            {
                char *path = getenv("PATH"); //Get value of PATH environment variable
                char *dir = strtok(path, ":"); //Set dir to each path value separated by :

                while(dir) //While loop to search for command in directories specified in PATH
                {
                    char *command_path = malloc(strlen(dir) + strlen(argv[0]) + 2); //Create buffer for command path
                    sprintf(command_path, "%s/%s", dir, argv[0]); //Set command path to path variable and command line

                    if (fp != NULL)
                    {
                        freopen(fp, "w", stdout); //Redirect output from stdout to file
                    }

                    execve(command_path, argv, NULL); //execute command

                    free(command_path); //Free buffer
                    dir = strtok(NULL, ":");
                }

                // If we reach here, execve failed for all directories in PATH
                printf("Error: command not found\n");
                exit(1);
            }
            else if(argv[argc] == NULL) //Parent process
            {
                wait(NULL);
            }

        }
    }
}
