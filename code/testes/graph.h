#include <vector>

using namespace std;

class Graph {
	private:

		int n, m;
		vector<Vertex> vertices;
		vector<Edge> edges;

	public:

		Graph() {}

		Graph(vector<int> &v, vector<pair<int, int>> &e) {
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
};