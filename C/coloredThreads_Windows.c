/*
    C for Windows
*/
#include <stdio.h>
#include <windows.h>

#define MAX_COLORS 5
#define GREEN 2
#define BLUE 1
#define YELLOW 14
#define RED 4
#define CYAN 3

HANDLE consoleMutex;

typedef struct recursiveData {
    int number;
    int color;
} ARGS, *PARGS;

char* colorCodeToName(int color) {
    switch(color) {
        case GREEN:
            return "Green";
            break;
        case BLUE:
            return "Blue";
            break;
        case YELLOW:
            return "Yellow";
            break;
        case RED:
            return "Red";
            break;
        case CYAN:
            return "Cyan";
            break;
        default:
            return "Unknown";
    }
}

void coloredConsoleMessage(int color, char message[]) {
    DWORD waitResult = WaitForSingleObject(consoleMutex, INFINITE);

    switch (waitResult) {
        case WAIT_OBJECT_0:
            HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(handle, color);
            printf("%s", message);
            SetConsoleTextAttribute(handle, 7);
            break;
        case WAIT_ABANDONED:
            break;
    }

    if (!ReleaseMutex(consoleMutex)) {
        // handle error?
    }
}

void recursiveThing(int number, int color) {
    char message[80];
    char* colorName = colorCodeToName(color);
    sprintf(message, "%s thread, number is: %d\n", colorName, number);
    coloredConsoleMessage(color, message);
    srand(GetCurrentThreadId());
    Sleep((rand() % 10) * 1000);

    if (number == 0) {
        sprintf(message, "%s thread complete!\n", colorName);
        coloredConsoleMessage(color, message);
        return;
    }

    recursiveThing(number-1, color);
}

DWORD recursiveThingAsync(LPVOID lpParam) {
    PARGS args;
    args = (PARGS)lpParam;
    recursiveThing(args->number, args->color);
    return 0;
}

int main() {
    int colors[MAX_COLORS] = {
        GREEN,  // Green
        BLUE,  // Blue
        YELLOW, // Yellow
        RED,  // Red
        CYAN   // Cyan
    };

    consoleMutex = CreateMutex(
        NULL,
        FALSE,
        NULL
    );

    DWORD threadIDs[MAX_COLORS];
    HANDLE tasks[MAX_COLORS];
    PARGS args[MAX_COLORS];

    for (int i=0; i < MAX_COLORS; i++) {
        args[i] = (PARGS)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ARGS));

        if (args[i] == NULL)
            ExitProcess(2);

        args[i]->number = 10;
        args[i]->color = colors[i];

        tasks[i] = CreateThread(
            NULL,
            0,
            recursiveThingAsync,
            args[i],
            0,
            &threadIDs[i]
        );

        if (tasks[i] == NULL)
            ExitProcess(3);
    }

    WaitForMultipleObjects(MAX_COLORS, tasks, TRUE, INFINITE);

    for (int i=0; i < MAX_COLORS; i++) {
        CloseHandle(tasks[i]);

        if (args[i] != NULL) {
            HeapFree(GetProcessHeap(), 0, args[i]);
            args[i] = NULL;
        }
    }

    printf("All threads complete!\n");
}
