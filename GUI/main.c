#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "simulatedannealing.h"
#include "startingcycle.h"
#include <gtk/gtk.h>


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

void mainProgram(FILE* input, FILE* output, FILE* error){
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

    simulatedAnnealing(n, E, pos, cycle, cycleLength, n * averageEdgeLength(n, E));

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

int main(int argc, char* argv[]){
    GtkWidget *window;
    GtkWidget *container;
    GtkWidget *startButton;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), container);

    startButton = gtk_button_new_with_label("Start");
    gtk_widget_set_size_request(startButton, 80, 60);
    gtk_fixed_put(GTK_FIXED(container), startButton, 600, 330);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}