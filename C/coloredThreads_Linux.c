/*
    C for *nix systems
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define CONSOLE_WIDTH 80
#define MAX_COLORS 5
#define BLUE "[0;34m"
#define GREEN "[0;32m"
#define YELLOW "[0;33m"
#define RED "[0;31m"
#define CYAN "[0;36m"

pthread_mutex_t consoleMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int number;
    char* color;
} ARGS, *PARGS;

char* colorCodeToName(char* color) {
    if (strcmp(color, BLUE) == 0)
        return "Blue";

    if (strcmp(color, GREEN) == 0)
        return "Green";

    if (strcmp(color, YELLOW) == 0)
        return "Yellow";

    if (strcmp(color, RED) == 0)
        return "Red";

    if (strcmp(color, CYAN) == 0)
        return "Cyan";

    return "Unknown";
}

void printToConsole(char message[], char color[]) {
    pthread_mutex_lock(&consoleMutex);
    char output[CONSOLE_WIDTH];
    sprintf(output, "\033%s%s\033[0m\n", color, message);
    printf("%s", output);
    pthread_mutex_unlock(&consoleMutex);
}

void recursiveThing(int number, char color[]) {
    char message[CONSOLE_WIDTH];
    sprintf(message, "%s thread, number is: %d", colorCodeToName(color), number);
    printToConsole(message, color);

    int random = rand() % 9 + 1;
    sleep(random);

    if (number == 0) {
        sprintf(message, "%s thread complete!", colorCodeToName(color));
        printToConsole(message, color);
    } else {
        recursiveThing(number - 1, color);
    }
}

void* recursiveThingAsync(void* args) {
    PARGS rgs;
    rgs = (PARGS)args;
    recursiveThing(rgs->number, rgs->color);
    return 0;
}

int main() {
    char* colors[MAX_COLORS] = {
        BLUE,
        GREEN,
        YELLOW,
        RED,
        CYAN
    };

    pthread_t threads[MAX_COLORS];
    ARGS args[MAX_COLORS];

    for (int i=0; i<MAX_COLORS; i++) {
        args[i].number = 10;
        args[i].color = colors[i];

        int ret = pthread_create(
            &threads[i],
            NULL,
            recursiveThingAsync,
            (void*) &args[i]
        );

        if (ret != 0)
            exit(1);
    }

    for (int i=0; i<MAX_COLORS; i++)
        pthread_join(threads[i], NULL);

    printf("All threads complete!\n");
    return 0;
}
