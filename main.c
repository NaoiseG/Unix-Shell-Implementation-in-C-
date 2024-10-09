/*
Name: Naoise Golden
Student number: 21376026
email: naoisegolden@hotmail.com

Below is an implementation of a simple shell in C. 
The shell behaves like bash on a basic level including user commands, change directory and a redirect output function too.
All aspects of the shell work as desired.
*/

#include "shell.h"

int main(void)
{
    if(!shell())
    {
        return -1;
    }

    return 0;
}
