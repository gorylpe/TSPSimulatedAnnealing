#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "simulatedannealing.h"
#include "startingcycle.h"
#include <gtk/gtk.h>
#include <locale.h>
#include <pthread.h>

//cycle vars
bool freed = true;
float** E = NULL;
float** pos = NULL;
int* cycle = NULL;
double* cycleLength = NULL;
int n;

//drawing vars
double minx;
double xsize;
double miny;
double ysize;

//simualtion thread vars
pthread_t* sim_thread = NULL;
gboolean sim_running = FALSE;

double averageEdgeLength(int n, float** E){
    double sum = 0.0;
    double numOfElements = n*(n-1)/2;
    for(int i = n - 1; i > 0; --i){
        for(int j = i - 1; j >= 0; --j){
            sum += Ed(i,j)/numOfElements;
        }
    }
    return sum;
}

typedef struct _InitStruct{
    GtkWidget* drawingArea;
} InitStruct;

void initialize(GtkWidget* widget, gpointer gpinitStruct){
    if(freed == false){
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

        freed = true;
    }

    InitStruct* initStruct = (InitStruct*)gpinitStruct;

    FILE* input = fopen("data.txt", "r");
    if(input != NULL) {
        fscanf(input, "%d", &n);
        printf("%d\n", n);

        E = malloc(n * sizeof(float *));
        for (int i = n - 1; i >= 0; --i) {
            E[i] = malloc((i + 1) * sizeof(float));
        }
        pos = malloc(n * sizeof(float *));
        for (int i = 0; i < n; ++i) {
            pos[i] = malloc(2 * sizeof(float));
        }
        initializeEdgesAndPositions(n, E, pos, input);

        fclose(input);

        minx = pos[0][0];
        double maxx = pos[0][0];
        miny = pos[0][1];
        double maxy = pos[0][1];
        for (int i = 1; i < n; ++i) {
            if (pos[i][0] < minx)
                minx = pos[i][0];
            if (pos[i][0] > maxx)
                maxx = pos[i][0];
            if (pos[i][1] < miny)
                miny = pos[i][1];
            if (pos[i][1] > maxy)
                maxy = pos[i][1];
        }
        xsize = maxx - minx;
        ysize = maxy - miny;

        printf("%f\n", minx);

        cycle = malloc(n * sizeof(int));
        firstCycleRandom(n, cycle);

        cycleLength = malloc(sizeof(double));
        *cycleLength = cycleLen(n, cycle, E);

        freed = false;

        gtk_widget_queue_draw(initStruct->drawingArea);
    }
}

void *simulation_thread_func(void *ptr){
    sim_running = TRUE;

    simulatedAnnealing(n, E, pos, cycle, cycleLength, n * averageEdgeLength(n, E));

    sim_running = FALSE;

    return NULL;
}

gboolean redraw_simulation_callback(gpointer data){
    GtkWidget* drawingArea = GTK_WIDGET(data);
    gtk_widget_queue_draw (drawingArea);

    printf("redrawing callback\n");

    return sim_running;
}

void start(GtkWidget* widget, gpointer gpinitStruct){
    InitStruct* initStruct = (InitStruct*)gpinitStruct;

    pthread_create(sim_thread, NULL, simulation_thread_func, NULL);

    g_timeout_add(100, redraw_simulation_callback, initStruct->drawingArea);
}

gboolean draw_callback(GtkWidget* widget, cairo_t* cr, gpointer data){
    if(pos != NULL && !freed){
        double width, height;

        width = gtk_widget_get_allocated_width(widget);
        height = gtk_widget_get_allocated_height(widget);

        cairo_set_source_rgb(cr, 255, 255, 255);
        cairo_rectangle(cr, 0.0, 0.0, width, height);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 1.0);

        for(int i = 1; i < n; ++i){
            float* first = pos[cycle[i-1]];
            float* second = pos[cycle[i]];
            double x1 = (first[0] - minx) * width / xsize;
            double y1 = (first[1] - miny) * height / ysize;
            cairo_move_to(cr, x1, y1);
            cairo_arc(cr, x1, y1, 3.0, 0, 2*M_PI);
            cairo_fill(cr);
            cairo_move_to(cr, x1, y1);
            cairo_line_to(cr, (second[0] - minx) * width / xsize, (second[1] - miny) * height / ysize);
            cairo_stroke(cr);
        }
        double x = (pos[cycle[n-1]][0] - minx) * width / xsize;
        double y = (pos[cycle[n-1]][1] - miny) * height / ysize;
        cairo_move_to(cr, x, y);
        cairo_arc(cr, x, y, 3.0, 0, 2*M_PI);
        cairo_fill(cr);
        cairo_move_to(cr, x, y);
        cairo_line_to(cr, (pos[cycle[0]][0] - minx) * width / xsize, (pos[cycle[0]][1] - miny) * height / ysize);

        cairo_stroke(cr);
    }

    return FALSE;
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    GtkWidget *window;
    GtkWidget *container;
    GtkWidget *loadButton;
    GtkWidget *startButton;
    GtkWidget *drawingArea;

    gtk_init (&argc, &argv);
    setlocale(LC_NUMERIC,"C");

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), container);

    drawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawingArea, 1180, 700);
    gtk_fixed_put(GTK_FIXED(container), drawingArea, 10, 10);
    g_signal_connect(G_OBJECT(drawingArea), "draw", G_CALLBACK(draw_callback), NULL);

    loadButton = gtk_button_new_with_label("Load");
    gtk_widget_set_size_request(loadButton, 60, 60);
    gtk_fixed_put(GTK_FIXED(container), loadButton, 1210, 10);
    InitStruct initStruct;
    initStruct.drawingArea = drawingArea;
    g_signal_connect(G_OBJECT(loadButton), "clicked", G_CALLBACK(initialize), &initStruct);

    startButton = gtk_button_new_with_label("Start");
    gtk_widget_set_size_request(startButton, 60, 60);
    gtk_fixed_put(GTK_FIXED(container), startButton, 1210, 80);
    g_signal_connect(G_OBJECT(startButton), "clicked", G_CALLBACK(start), &initStruct);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}