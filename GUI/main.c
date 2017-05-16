#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "simulatedannealing.h"
#include "startingcycle.h"
#include <windows.h>


double averageEdgeLength(int n, float** E){
    double sum = 0.0;
    double numOfElements = n*(n-1)/2;
    for(int i = n - 1; i > 0; --i){
        for(int j = i - 1; j >= 0; --j){
            sum += E(i,j)/numOfElements;
        }
    }
    return sum;
}

void mainProgram(FILE* input, FILE* output, FILE* error, HWND hwnd){
    int n;
    fscanf(input, "%d", &n);

    //save memory
    float** E;
    float** pos;
    E = malloc(n * sizeof(float*));
    for (int i = n - 1; i >= 0; --i) {
        E[i] = malloc((i + 1) * sizeof(float));
    }
    pos = malloc(n * sizeof(float*));
    for (int i = 0; i < n; ++i) {
        pos[i] = malloc(2 * sizeof(float));
    }
    initializeEdgesAndPositions(n, E, pos, input);

    int* cycle = malloc(n * sizeof(int));
    //firstCycleAscending(n, cycle);
    firstCycleRandom(n, cycle);

    double* cycleLength = malloc(sizeof(double));
    *cycleLength = cycleLen(n, cycle, E);

    simulatedAnnealing(n, E, pos, cycle, cycleLength, n * averageEdgeLength(n, E), hwnd);

    fprintf(output, "%f\n", *cycleLength);
    /*int start;
    for (start = 0; start < n && cycle[start] != 0; ++start);
    for (int i = 0; i < n; ++i) {
        fprintf(error, "%d\n", cycle[(start + i)%n] + 1);
    }
    fprintf(error, "%d\n", cycle[start] + 1);*/

    for (int i = 0; i < n; ++i) {
        free(E[i]);
    }
    free(E);
    for (int i = 0; i < n; ++i) {
        free(pos[i]);
    }
    free(pos);
    free(cycle);
    free(cycleLength);
}

#define BUTTON_START 501

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
    switch(msg){
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_COMMAND:
            switch(wparam){
                case BUTTON_START: {
                    FILE *input = fopen("data.txt", "r");
                    if (input != NULL) {
                        mainProgram(input, stdout, stderr, hwnd);
                    }
                    break;
                }
                default:
                    break;
            }
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    HWND hWindow;
    MSG message;
    WNDCLASS window;

    window.hInstance = hInstance;
    window.lpszClassName = "main class";
    window.lpfnWndProc = wndProc;
    window.lpszMenuName = NULL;
    window.style = 0;
    window.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    window.hCursor = LoadCursor(NULL, IDC_ARROW);
    window.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;

    if(!RegisterClass(&window)) return 0;

    hWindow = CreateWindow("main class", "TSP Simulated Annealing GUI", WS_OVERLAPPEDWINDOW, 300, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
    ShowWindow(hWindow, SW_SHOW);

    HWND hButtonStart = CreateWindow("BUTTON", "Start", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 50, 100, 50, hWindow, (HMENU) BUTTON_START, hInstance, NULL);

    while(GetMessage(&message, NULL, 0, 0)){
        DispatchMessage(&message);
    }

    return 0;
}