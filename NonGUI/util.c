#include "util.h"

float calculateEdgeLength(int a, int b, float** pos) {
	float dx = pos[b][0] - pos[a][0];
	float dy = pos[b][1] - pos[a][1];
	return sqrtf(dx*dx + dy*dy);
}

void printCycle(FILE* stream, int n, int* cycle) {
	for (int i = 0; i < n; ++i) {
		fprintf(stream, "%d,", cycle[i] + 1);
		if (cycle[i] == -1)
			break;
	}
	fprintf(stream, "END");
}

void printEdges(FILE* stream, int n, float** E) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			fprintf(stream, "%5.1f ", E[i][j]);
		}
		fprintf(stream, "\n");
	}
}

double cycleLen(int n, int* cycle, float** E) {
	double len = 0.0;
	for (int i = 1; i < n; ++i) {
		int a = cycle[i - 1];
		int b = cycle[i];
		len += (double)E(a,b);
	}
	len += (double)E(cycle[0],cycle[n - 1]);
	return len;
}

int calculateBirthdayParadoxValue(int n, float percent) {
	float p;
	float k = (float)n;
	float i;
	p = 1.0;
	for (i = 1.0; i < k; ++i) {
		p *= (1.0 - i / k);
		if (p < 1.0 - percent)
			break;
	}
	return (int)i + 1;
}

void calculateEdges(int n, float** E, float** pos) {
	for (int i = n - 1; i >= 0; --i) {
		for (int j = i; j >= 0; --j) {
			float len = 0.0;
			if (i != j) {
				len = calculateEdgeLength(i, j, pos);
			}
			E[i][j] = len;
		}
	}
}

void loadEdges(int n, float** E) {
	FILE* edges;
	edges = fopen("edges.txt", "r");

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			int e;
			fscanf(edges, "%d", &e);
			E[i][j] = (float)e;
		}
	}
	fclose(edges);
}

void loadPositions(int n, float** pos, FILE* input){
    for (int i = 0; i < n; ++i) {
        int a;
        float x, y;
        fscanf(input, "%d %f %f", &a, &x, &y);
        pos[a-1][0] = x;
        pos[a-1][1] = y;
    }
}

void initializeEdges(int n, float** E, FILE* input) {
	float** pos;
    pos = malloc(n * sizeof(float*));
    for (int i = 0; i < n; ++i) {
        pos[i] = malloc(2 * sizeof(float));
    }
    loadPositions(n, pos, input);

	calculateEdges(n, E, pos);
	//loadEdges(n, E);

	for (int i = 0; i < n; ++i) {
		free(pos[i]);
	}
	free(pos);
}
