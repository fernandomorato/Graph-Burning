#include <bits/stdc++.h>

#include "lib/vertex.h"
#include "lib/utility.h"
#include "lib/centrality.h"
#include "lib/load_input.h"

using namespace std;

/******************
* Argument parser *
******************/
void parse_args(int argc, char **argv, int &time_limit, string &input_path, double &alpha, long long &seed) {
	bool has_seed = false;
	for (int i = 1; i < argc; i++) {
		string str = argv[i];
		if(str[0] == '-') {
			if(str.substr(1) == "tl")
				time_limit = atoi(argv[i + 1]);
			else if(str.substr(1) == "ip")
				input_path = argv[i + 1];
			else if (str.substr(1) == "alpha")
				alpha = atof(argv[i + 1]);
			else if (str.substr(1) == "seed") {
				seed = atoll(argv[i + 1]);
				has_seed = true;
			}
			else
				cout << "Invalid option: -" << str.substr(1) << "\n";
			i++;
		}
	}
	if (!has_seed) {
		seed = (long long) chrono::steady_clock::now().time_since_epoch().count();
	}
}

/**********************
* Print instance info *
**********************/
void print_instance(int n_vertices, int n_edges, vector<Vertex*> &vertices) {
	cout << "n_vertices: " << n_vertices << endl;
	cout << "n_edges: " << n_edges << endl << endl;

	for(int i = 0; i < n_vertices; i++) {
		cout << "vertex " << vertices[i]-> getId() << endl;
		cout << "degree: " << vertices[i]-> getDegree() << endl;
		cout << "neighs: ";
		vector<Vertex*> neighbors = vertices[i]->getNeighbors();
		cout << "[";
		for(int j = 0; j < vertices[i]-> getDegree(); j++) {
			if (j) cout << ' ';
			cout << neighbors[j]->getId();
		}
		cout << "]" << endl << endl;
	}
}

void dfs(Vertex *cur, vector<int> &vertices, vector<pair<int, int>> &edges) {
	vertices.push_back(cur->getId());
	for (Vertex *neig : cur->getNeighbors()) {
		if (!neig->getVisited() && neig->getState() == 0) {
			edges.emplace_back(min(cur->getId(), neig->getId()), max(cur->getId(), neig->getId()));
			dfs(neig, vertices, edges);
		}
	}
}

vector<Vertex*> select_vertices(vector<Vertex*> candidates) {
	double max_centrality = -1e9;
	for (Vertex *x : candidates) {
		max_centrality = max(max_centrality, x->getCentrality());
	}
	assert(max_centrality > -1e9);
	double bound = max_centrality - 2 * standart_deviation(candidates);
	vector<Vertex*> selected_vertices;
	sort(candidates.begin(), candidates.end(), [&](Vertex *a, Vertex *b) {
		return a->getCentrality() > b->getCentrality();
	});
	for (Vertex *cur : candidates) {
		if (cur->getCentrality() >= bound || (int) selected_vertices.size() < 5)
			selected_vertices.push_back(cur);
	}
	assert(!selected_vertices.empty());
	return selected_vertices;
}

void bfs(Vertex *source, int best_so_far) {
	queue<Vertex*> q;
	q.push(source);
	while (!q.empty()) {
		Vertex *cur = q.front();
		q.pop();
		if (cur->getLabel() < best_so_far) {
			// block the targeted vertices
			cur->setState(1);
		}
		for (Vertex *neig : cur->getNeighbors()) {
			if (neig->getLabel() > cur->getLabel() + 1) {
				neig->setLabel(cur->getLabel() + 1);
				q.push(neig);
			}
		}
	}
}

/**********************
* Construction phase  *
**********************/
pair<vector<int>, bool> construction(vector<Vertex*> vertices, vector<double> &centrality, double alpha, mt19937 &rng, int best_so_far) {
	for (Vertex *v : vertices) {
		v->reset();
	}
	vector<int> burning_sequence;
	for (int round = 1, ok = 1; ok; round++) {
		vector<Vertex*> candidate_list;
		for (Vertex *v : vertices) {
			v->setVisited(0);
		}
		int comps = 0;
		for (Vertex *v : vertices) {
			if (v->getVisited() == 0 && v->getState() == 0) {
				vector<int> component_vertices;
				vector<pair<int, int>> component_edges;
				v->setVisited(++comps);
				dfs(v, component_vertices, component_edges);
				calculate_centrality(vertices, centrality, component_vertices, component_edges, rng);
				vector<Vertex*> local_candidate_list = select_vertices(vertices);
				for (Vertex *vv : local_candidate_list) {
					candidate_list.push_back(vv);
				}
			}
		}
		vector<Vertex*> restricted_candidate_list;
		if (round == 1) {
			restricted_candidate_list = candidate_list;
		} else {
			auto b = [&](Vertex *v) {
				// Benefit function
				return v->getLabel();
			};
			if (candidate_list.empty()) {
				for (Vertex *v : vertices) {
					if (v->getState() == 1)
						restricted_candidate_list.push_back(v);
				}
			} else {
				int max_benefit = 0;
				int min_benefit = (int) 1e9;
				for (Vertex *v : candidate_list) {
					max_benefit = max(max_benefit, b(v));
					min_benefit = min(min_benefit, b(v));
				}
				for (Vertex *v : candidate_list) {
					if (1.0 * b(v) >= max_benefit - alpha * (max_benefit - min_benefit)) {
						restricted_candidate_list.push_back(v);
					}
				}
			}
		}
		Vertex *current_activator = restricted_candidate_list[rng() % ((int) restricted_candidate_list.size())];
		current_activator->setLabel(round); // burned
		burning_sequence.push_back(current_activator->getId());
		bfs(current_activator, best_so_far);
		int cnt_safe_vertices = 0;
		for (Vertex *v : vertices) {
			if (v->getLabel() == round) 
				v->setState(2);
			cnt_safe_vertices += (v->getState() == 0 ? 1 : 0);
		}
		if (cnt_safe_vertices == 0 or (int) burning_sequence.size() > best_so_far)
			ok = 0;
	}
	return make_pair(burning_sequence, (int) burning_sequence.size() <= best_so_far);
}


int main(int argc, char **argv) {

	for (int i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n\n");

	FILE *input_file;
	double total_time, alpha;
	int n_vertices, n_edges, time_limit;
	string instance_name, input_path;
	vector<Vertex*> vertices;
	vector<double> centrality;
	long long seed;

	parse_args(argc, argv, time_limit, input_path, alpha, seed);
	instance_name = input_path.substr(input_path.find_last_of("/") + 1, input_path.find_last_of(".") - (input_path.find_last_of("/") + 1));
	input_file = fopen(input_path.c_str(), "r");
	load_input(input_file, n_vertices, n_edges, vertices);
	print_instance(n_vertices, n_edges, vertices);

	clock_t init = clock();
	mt19937 rng(seed);
	int incumbent_solution = (int) floor(2.0 * sqrt((double) n_vertices));
	centrality.resize(n_vertices);

	for (int iteration = 1; ; iteration++) {
		pair<vector<int>, bool> solution = construction(vertices, centrality, alpha, rng, incumbent_solution);
		if (init + 1e9 - clock() < time_limit) {
			break;
		}
	}

	printf("Elapsed time: %.8lf s\n\n", total_time);

	fclose(input_file);

	exit(0);
}