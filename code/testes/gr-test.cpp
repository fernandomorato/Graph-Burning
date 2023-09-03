#include <bits/stdc++.h>

#include "../lib/vertex.h"
#include "../lib/utility.h"
#include "../lib/centrality.h"
#include "../lib/load_input.h"

using namespace std;

/******************
* Argument parser *
******************/
void parse_args(int argc, char **argv, int &time_limit, string &input_path, string &output_path, string &solution_path, double &alpha, long long &seed) {
	bool has_seed = false;
	for (int i = 1; i < argc; i++) {
		string str = argv[i];
		if(str[0] == '-') {
			if(str.substr(1) == "tl")
				time_limit = atoi(argv[i + 1]);
			else if(str.substr(1) == "ip")
				input_path = argv[i + 1];
			else if(str.substr(1) == "op")
				output_path = argv[i + 1];
			else if(str.substr(1) == "sp")
				solution_path = argv[i + 1];
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

void dfs(Vertex *cur, vector<Vertex*> &vertices, vector<pair<int, int>> &edges) {
	cur->setVisited(true);
	vertices.push_back(cur);
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
	int cnt_safe_vertices = 0;
	for (Vertex *v : vertices) {
		v->reset();
		cnt_safe_vertices++;
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
				assert(cnt_safe_vertices > 0);
				vector<Vertex*> component_vertices;
				vector<pair<int, int>> component_edges;
				v->setVisited(++comps);
				dfs(v, component_vertices, component_edges);
				calculate_centrality(vertices, centrality, component_vertices, component_edges, rng);
				vector<Vertex*> local_candidate_list = select_vertices(component_vertices);
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
				sort(candidate_list.begin(), candidate_list.end(), [&](auto v, auto u) {
					return b(v) > b(u);
				});
				int max_benefit = b(candidate_list[0]);
				int min_benefit = b(candidate_list.back());
				for (Vertex *v : candidate_list) {
					if (1.0 * b(v) >= max_benefit - alpha * (max_benefit - min_benefit) or (int) restricted_candidate_list.size() < 5) {
						restricted_candidate_list.push_back(v);
					}
				}
			}
		}
		Vertex *current_activator = restricted_candidate_list[rng() % ((int) restricted_candidate_list.size())];
		assert(current_activator->getState() != 2);
		current_activator->setLabel(round); // burned
		burning_sequence.push_back(current_activator->getId());
		bfs(current_activator, best_so_far);
		// int cnt_safe_vertices = 0;
		int cnt_burned_vertices = 0;
		for (Vertex *v : vertices) {
			if (v->getLabel() == round) 
				v->setState(2);
			cnt_safe_vertices += (v->getState() == 0 ? 1 : 0);
			cnt_burned_vertices += (v->getState() == 2 ? 1 : 0);
		}
		if ((int) burning_sequence.size() > best_so_far or cnt_burned_vertices == (int) vertices.size())
			ok = 0;
	}
	return make_pair(burning_sequence, (int) burning_sequence.size() <= best_so_far);
}

int main(int argc, char **argv) {

	for (int i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
	}
	printf("\n\n");

	FILE *input_file, *output_file, *solution_file;
	double total_time, alpha;
	int n_vertices, n_edges, time_limit;
	string instance_name, input_path, output_path, solution_path;
	vector<Vertex*> vertices;
	vector<double> centrality;
	long long seed;

	parse_args(argc, argv, time_limit, input_path, output_path, solution_path, alpha, seed);
	instance_name = input_path.substr(input_path.find_last_of("/") + 1, input_path.find_last_of(".") - (input_path.find_last_of("/") + 1));
	input_file = fopen(input_path.c_str(), "r");
	load_input(input_file, n_vertices, n_edges, vertices);
	print_instance(n_vertices, n_edges, vertices);

	clock_t init = clock();
	mt19937 rng(seed);
	int incumbent_solution = (int) floor(2.0 * sqrt((double) n_vertices));
	int iteration_incumbent_solution = 0;
	double time_to_incumbent_solution = 0;
	vector<int> burning_sequence;
	centrality.resize(n_vertices);

	for (int iteration = 1; ; iteration++) {
		pair<vector<int>, bool> solution = construction(vertices, centrality, alpha, rng, incumbent_solution);
		// cerr << (int) solution.first.size() << ' ' << (solution.second ? "good" : "not good") << endl;
		if (solution.second && incumbent_solution > (int) solution.first.size()) {
			iteration_incumbent_solution = iteration;
			time_to_incumbent_solution = (clock() - init) / CLOCKS_PER_SEC;
			incumbent_solution = (int) solution.first.size();
			burning_sequence = solution.first;
		}
		if ((clock() - init) / CLOCKS_PER_SEC > time_limit) {
			total_time = (clock() - init) / CLOCKS_PER_SEC;
			break;
		}
	}

	printf("Elapsed time: %.8lf s\n\n", total_time);

	fclose(input_file);

	output_file = fopen(output_path.c_str(), "a");
	fprintf(output_file, "%s,", instance_name.c_str());
	fprintf(output_file, "%.4lf,", total_time);
	fprintf(output_file, "%.4lf,", time_to_incumbent_solution);
	fprintf(output_file, "%d,", iteration_incumbent_solution);
	fprintf(output_file, "%d\n", incumbent_solution);
	fclose(output_file);

	solution_file = fopen(solution_path.c_str(), "w");
	fprintf(solution_file, "%d\n", (int) burning_sequence.size());
	for (int x : burning_sequence) {
		fprintf(solution_file, "%d ", x);
	}
	fprintf(solution_file, "\n");
	fclose(solution_file);
	exit(0);
}