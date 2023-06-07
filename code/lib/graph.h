#include <vector>
#include "vertex.h"
#include "edge.h"

using namespace std;

template <class T>
class Graph {
public:
	Graph() {}

	Graph(vector<int> &v, vector<pair<int, int>> &e) {
		n = (int) v.size();
		m = (int) e.size();
		vertices.resize(n);
		edges.resize(m);
		for (int i = 0; i < n; i++) {
			vertices[i] = Vertex(v[i]);
		}
		sort(vertices.begin(), vertices.end());
		for (int i = 0; i < m; i++) {
			edges[i] = Edge(e[i].first, e[i].second);
		}
	}

	int getSize() { 
		return this->n; 
	}

	double getDensity() { 
		return double(2 * m) / double(v) / double(v - 1); 
	}

	Vertex getVertex(int id) {
		return getVertices()[lower_bound(getVertices.begin(), getVertices.end(), Vertex(id)) - getVertices().begin()];
	}

	vector<Vertex> getVertices() {
		return this->vertices; 
	}

	vector<Edge> getEdges() { 
		return this->edges; 
	}

	vector<double> getCentralityScores() {
		return this->centrality;
	}

private:
	int n, m;
	vector<Vertex> vertices;
	vector<Edge> edges;
	vector<double> centrality;
	// vector<vector<T>> adj;
};