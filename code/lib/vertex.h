#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

using namespace std;

class Vertex {

public:
	Vertex(int id = 0) {
		setId(id);
		setVisited(0);
		setState(0);
	}

	void setId(int _id) { id = _id; }
	int getId() { return id; }

	void setState(int _state) { state = _state; }
	int getState() { return state; }

	void setLabel(int _label) { label = _label; }
	int getLabel() { return label; }

	int getDegree() { return degree; }

	void setVisited(int _visited) { visited = _visited; }
	int getVisited() { return visited; }

	void setCentrality(double _centrality) { centrality = _centrality; }
	double getCentrality() { return centrality; }

	void setNeighbors(vector<Vertex*> &_neighbors) { neighboors = _neighbors; }
	void addNeighbor(Vertex *_neighbor) {
		neighboors.push_back(_neighbor);
		swap(neighboors[degree], neighboors.back());
		degree++;
	}
	vector<Vertex*> &getNeighbors() { return neighboors; }

	void reset() {
		setState(0);
		setLabel((int) 1e9);
		setVisited(0);
		setCentrality(0.0);
	}


private:
	int id;
	int state;
	int label;
	int degree;
	int visited;
	double centrality;
	vector<Vertex*> neighboors;

};

#endif