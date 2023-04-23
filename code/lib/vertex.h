class Vertex {
public:
	Vertex() {}

	Vertex(int id) { setId(id); }

	void setId(int _id) {
		id = _id;
	}

	int getId() {
		return id;
	}

private:
	int id;
	vector<Vertex> neighboors;
};