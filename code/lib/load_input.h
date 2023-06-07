#include <iostream>
#include <vector>

#include "vertex.h"

using namespace std;

void load_input(FILE *input_file, int &n_vertices, int &n_edges, vector<Vertex*> &vertices) {
    if (!fscanf(input_file, "%d %d", &n_vertices, &n_edges))
        cerr << "Error reading the input file\n";

    vertices.resize(n_vertices);
    for (int i = 0; i < n_vertices; i++) {
        vertices[i] = new Vertex(i);
    }
    for (int i = 0; i < n_edges; i++) {
        int a, b;
        if (!fscanf(input_file, "%d %d", &a, &b))
            cerr << "Error reading the input file\n";
        vertices[a]->addNeighbor(vertices[b]);
        vertices[b]->addNeighbor(vertices[a]);
    }
}