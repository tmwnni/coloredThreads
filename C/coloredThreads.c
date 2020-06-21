/*
    C for Windows
*/
#include <stdio.h>
#include <windows.h>

#define MAX_COLORS 5

HANDLE consoleMutex;

typedef struct recursiveData {
    int number;
    int color;
} RDATA, *PRDATA;

char* colorCodeToName(int color) {
    switch(color) {
        case 2:
            return "Green";
            break;
        case 1:
            return "Blue";
            break;
        case 14:
            return "Yellow";
            break;
        case 4:
            return "Red";
            break;
        case 3:
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
            printf(message);
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
    sprintf(message, "%s thread, number is: %d\n", colorCodeToName(color), number);
    coloredConsoleMessage(color, message);
    srand(GetCurrentThreadId());
    Sleep((rand() % 10) * 1000);

    if (number == 0) {
        coloredConsoleMessage(color, "Thread complete!\n");
        return;
    }

    recursiveThing(number-1, color);
}

DWORD recursiveThingAsync(LPVOID lpParam) {
    PRDATA args;
    args = (PRDATA)lpParam;
    recursiveThing(args->number, args->color);
    return 0;
}

int main() {
    int colors[MAX_COLORS] = {
        2,  // Green
        1,  // Blue
        14, // Yellow
        4,  // Red
        3   // Cyan
    };

    consoleMutex = CreateMutex(
        NULL,
        FALSE,
        NULL
    );

    DWORD threadIDs[MAX_COLORS];
    HANDLE tasks[MAX_COLORS];
    PRDATA args[MAX_COLORS];

    for (int i=0; i < MAX_COLORS; i++) {
        args[i] = (PRDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(RDATA));

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
