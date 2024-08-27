//
//  main.c
//  temrinal
//
//  Created by Syed Abrar on 23/04/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

#define HOST_NAME_MAX 1000

int main() {
    char hostname[HOST_NAME_MAX];
    char *user;
    char *user_input;
    char *cmd;

    // Get the hostname and username
    gethostname(hostname, HOST_NAME_MAX);
    user = getenv("USER");

    // Loop to get and execute commands
    while (1) {
        // Display prompt and get input
        char prompt[1024];
        snprintf(prompt, 1024, "%s@%s$", user, hostname);
        user_input = readline(prompt);

        // Check for exit command
        if (strcmp(user_input, "exit") == 0) {
            free(user_input);
            break;
        }

        // Execute the command
        cmd = strdup(user_input);
        add_history(user_input);
        free(user_input);
        system(cmd);
        free(cmd);
    }

    return 0;
}
