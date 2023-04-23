#include <vector>

using namespace std;

class Graph {
public:

	Graph() {}

	Graph(vector<Vertex> &v, vector<Edge> &e) {
		n = (int) v.size();
		m = (int) e.size();
		vertices.resize(n);
		edges.resize(m);
		for (int i = 0; i < n; i++) {
			vertices[i] = Vertex(i);
		}
		for (int i = 0; i < m; i++) {
			edges[i] = Edge(vertices[i], vertices[i]);

		}
	}

	int size() { return this->n; }

private:
	int n, m;
	vector<Vertex> vertices;
	vector<Edge> edges;
};