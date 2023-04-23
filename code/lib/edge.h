template <class T = int>
class Edge {
public:
	Edge() {}

	Edge(Vertex from, Vertex to, T weight = T(1), bool directed = false) {
		setFrom(from);
		setTo(to);
		setWeight(weight);
		setDirected(directed);
	}

	void setFrom(Vertex _from) {
		from = _from;
	}

	Vertex getFrom() {
		return from;
	}

	void setTo(Vertex _to) {
		to = _to;
	}

	Vertex getTo() {
		return to;
	}

	void setWeight(T _weight) {
		weight = _weight;
	}

	T getWeight() {
		return weight;
	}

	void setDirected(bool _directed) {
		directed = _directed;
	}

	bool isDirected() {
		return directed;
	}
private:
	Vertex from, to;
	T weight;
	bool directed;
};