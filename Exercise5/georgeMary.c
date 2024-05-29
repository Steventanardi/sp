#include <stdio.h>  // Include the standard input/output library
#include <unistd.h>
#include <stdlib.h>

void print_george() {
    while (1) {
        printf("George\n");
        sleep(1);
    }
}

void print_mary() {
    while (1) {
        printf("Mary\n");
        sleep(2);
    }
}

int main() {
    pid_t pid = fork();  // Create a new process

    if (pid == 0) {
        // Child process
        print_george();
    } else {
        // Parent process
        pid_t pid2 = fork();
        if (pid2 == 0) {
            print_mary();
        } else {
            // Main parent process
            while (1) {
                printf("----------------\n");
                sleep(1);
            }
        }
    }
    return 0;
}
