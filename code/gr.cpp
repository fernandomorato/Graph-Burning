#include <bits/stdc++.h>
 
using namespace std;
 
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int INF = 1e9;

vector<vector<int>> adj;
vector<double> centrality_scores;
vector<pair<int, int>> edges;

bool check_solution(int n_vertices, vector<int> &burning_sequence) {
	return true;
	set<int> burned;
	for (auto x : burning_sequence) {
		if (burned.count(x)) {
			return false;
		}
		auto cp_burned = burned;
		for (int eita : cp_burned)
			for (int viz : adj[eita])
				burned.insert(viz);
		burned.insert(x);
	}
	if ((int) burned.size() != n_vertices) {
		return false;
	}
	return true;
}

void readInput(FILE* input_file, int *n_vertices, int *n_edges, double &density) {
	fscanf(input_file, "%d %d", n_vertices, n_edges);
	density = 2.0 * (double) (*n_edges) / (1.0 * (double) (*n_vertices) * (double) ((*n_vertices) - 1));
	adj.resize(*n_vertices);
	// adj_matrix.resize(*n_vertices);
	// for (int i = 0; i < *n_vertices; i++)
	// 	adj_matrix[i].resize(*n_vertices);
	for (int i = 0; i < *n_edges; i++) {
		int a, b;
		fscanf(input_file, "%d %d", &a, &b);
		adj[a].push_back(b);
		adj[b].push_back(a);
		edges.emplace_back(a, b);
		// adj_matrix[a][b] = adj_matrix[b][a] = 1;
	}
	centrality_scores.resize(*n_vertices);
	// for (int i = 0; i < *n_vertices; i++) {
	// 	adj_matrix[i][i] = 1;
	// 	fscanf(input_file, "%lf", &centrality_scores[i]);
	// }
}
 
void calc_centrality_scores(vector<double> &centrality_scores, vector<pair<int, int>> &edges, mt19937 &rng) {
	double EPS = 1e-3;
	int tamanho = (int) centrality_scores.size();
	vector<double> x0(tamanho), x1(tamanho);
	uniform_real_distribution<double> unif(0, 1);
	double norm = 0;
	for (int i = 0; i < tamanho; i++) {
		x1[i] = unif(rng);
		norm += x1[i] * x1[i];
	}
	norm = sqrt(norm);
	for (int i = 0; i < tamanho; i++) {
		x1[i] /= norm;
	}
	int iter = 0;
	double lambda = 0, lambdaOld = 0, m = 0;
	while (iter <= 100) {
		swap(x0, x1);
		for (int i = 0; i < tamanho; i++) 
			x1[i] = 0;
		for (auto p : edges) {
			x1[p.first] += x0[p.second];
			x1[p.second] += x0[p.first];
		}
		norm = 0;
		for (int i = 0; i < tamanho; i++) {
			norm += x1[i] * x1[i];
		}
		norm = sqrt(norm);
		lambda = 0;
		for (int i = 0; i < tamanho; i++) {
			x1[i] /= norm;
			lambda += x1[i] * x0[i];
		}
		lambda = sqrt(lambda);
		iter++;
		if (fabs((lambda - lambdaOld) / lambda) < EPS)
			break;
		lambdaOld = lambda;
	}
}
 
vector<int> select_vertices(vector<int> &candidates, vector<double> &centrality, double density) {
	vector<pair<double, int>> aux;
	for (int i = 0; i < (int) candidates.size(); i++) {
		aux.emplace_back(centrality[candidates[i]], candidates[i]);
	}
	sort(aux.rbegin(), aux.rend());
	vector<int> selected_vertices;
	for (int i = 0; i < (int) aux.size(); i++) {
		double current_vertex_centrality = aux[i].first;
		int current_vertex = aux[i].second;
		if (current_vertex_centrality >= (1 - density) * aux[0].first || (int) selected_vertices.size() < 5)
			selected_vertices.push_back(current_vertex);
	}
	if (selected_vertices.empty())
		selected_vertices.push_back(aux[0].second);
	return selected_vertices;
}

void dfs(vector<bool> &visited, int current_vertex, vector<pair<int, int>> &component_edges, vector<int> &component_vertices, vector<int> &vertex_status) {
	assert(vertex_status[current_vertex] == 0); // Checks if the vertice is safe
	component_vertices.push_back(current_vertex);
	visited[current_vertex] = true;
	for (int neighbour : adj[current_vertex]) {
		if (visited[neighbour] || vertex_status[neighbour] != vertex_status[current_vertex]) 
			continue;
		component_edges.emplace_back(current_vertex, neighbour);
		dfs(visited, neighbour, component_edges, component_vertices, vertex_status);
	}
}

void bfs(int source, int round, vector<int> &vertex_label) {
	vertex_label[source] = round;
	queue<int> q;
	q.push(source);
	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		for (auto x : adj[cur]) {
			if (vertex_label[cur] + 1 < vertex_label[x]) {
				vertex_label[x] = vertex_label[cur] + 1;
				q.push(x);
			}
		}
	}
}

pair<vector<int>, bool> construction(int iteration, vector<double> centrality, int n_vertices, int n_edges, double graph_density, mt19937 &rng, double alpha, int K_i) {
	// Round Variables
	int current_round = 1;
	vector<int> burning_sequence;
	set<int> safe, targeted, burned;
	vector<int> vertex_status(n_vertices); // 0 -> safe | 1 -> targeted | 2 -> burned
	vector<int> vertex_label(n_vertices, INF);

	// Round 1
	vector<int> vertices(n_vertices);
	iota(vertices.begin(), vertices.end(), 0);
	vector<int> selected_vertices = select_vertices(vertices, centrality, graph_density);
	assert(!selected_vertices.empty());

	int initial_vertex = selected_vertices[rng() % ((int) selected_vertices.size())];
	bfs(initial_vertex, current_round, vertex_label);
	burning_sequence.push_back(initial_vertex);
 
	for (int i = 0; i < n_vertices; i++) {
		if (vertex_label[i] == 1) {
			burned.insert(i);
			vertex_status[i] = 2;
		} else if (vertex_label[i] <= K_i) {
			targeted.insert(i);
			vertex_status[i] = 1;
		} else {
			safe.insert(i);
			vertex_status[i] = 0;
		}
	}
 
	// Rounds t >= 2
	do {
		current_round++;
		vector<bool> visited(n_vertices, false);
		for (int i = 0; i < n_vertices; i++) {
			for (int viz : adj[i]) {
				assert(abs(vertex_label[i] - vertex_label[viz]) <= 1);
			}
		}
		vector<int> cl; // Candidate List
		if (safe.empty()) {
			assert(!targeted.empty());
			cl = vector<int>(targeted.begin(), targeted.end());
		} else {
			for (int i : safe) {
				if (!visited[i]) {
					vector<pair<int, int>> component_edges;
					vector<int> component_vertices;
					dfs(visited, i, component_edges, component_vertices, vertex_status);
					double component_density = double(2 * (int) component_edges.size())
												/ ((double) component_vertices.size() 
												* ((double) component_vertices.size() - 1));
					calc_centrality_scores(centrality, component_edges, rng);
					vector<int> cl_local = select_vertices(component_vertices, centrality, component_density);
					for (int component_vertex : cl_local) {
						cl.push_back(component_vertex);
					}
				}
			}
		}
		assert(!cl.empty());

		// Agora que temos a nossa cl, precisamos definir a funcao de beneficio para compor uma RCL
		// definimos b(v) = b* - | b* - l(v) |, onde b* = L_M - K_i + i e L_M = maior label
		int L_M = 0;
		for (int i = 0; i < n_vertices; i++) { // Estou pegando o rotulo maximo considerando TODOS (talvez usar soh a cl)
			L_M = max(L_M, vertex_label[i]);
		}
		int b_star = L_M - K_i + current_round;
		int min_benefit = INF;
		int max_benefit = -INF;
		auto b = [&](int v) {
			return b_star - abs(b_star - vertex_label[v]);
		};
		for (int x : cl) {
			min_benefit = min(min_benefit, b(x));
			max_benefit = max(max_benefit, b(x));
		}
		vector<int> rcl;
		for (int x : cl) {
			if (b(x) >= max_benefit - alpha * (max_benefit - min_benefit)) {
				rcl.push_back(x);
			}
		}
		assert(!rcl.empty());
		int next_activator = rcl[rng() % (int) rcl.size()];
		vertex_label[next_activator] = current_round;
		bfs(next_activator, current_round, vertex_label);
		burning_sequence.push_back(next_activator);
		if ((int) burning_sequence.size() >= K_i) {
			return make_pair(burning_sequence, false);
		}
		vector<int> to_burn_now;
		to_burn_now.push_back(next_activator);
		targeted.erase(next_activator);
		safe.erase(next_activator);
		vertex_status[next_activator] = 2;

		for (int current_vertex : burned) {
			for (int neighbour : adj[current_vertex]) {
				if (vertex_status[neighbour] == 2)
					continue;
				if (vertex_status[neighbour] != 1) {
					cerr << vertex_status[current_vertex] << " viz " << vertex_status[neighbour] << '\n';
				}
				assert(vertex_status[neighbour] == 1);
				vertex_status[neighbour] = 2; // queimei
				targeted.erase(neighbour);
				to_burn_now.push_back(neighbour);
			}
		}
		for (int current_vertex : to_burn_now) {
			burned.insert(current_vertex);
			for (int neighbour : adj[current_vertex]) {
				if (vertex_status[neighbour] != 0)
					continue;
				assert(vertex_status[neighbour] == 0);
				vertex_status[neighbour] = 1;
				safe.erase(neighbour);
				targeted.insert(neighbour);
			}
		}
	} while ((int) burned.size() != n_vertices);
	return make_pair(burning_sequence, true);
}

void parse_args(int argc, char **argv, int &seed, double &alpha, int &time_limit, string &input_path, string &output_path, string &log_path, string &alpha_path) {
	bool f_seed  = false;
	for (int i = 0; i < argc; i++) {
		string str = argv[i];
		if (str[0] == '-') {
			string identifier = str.substr(1);
			if (identifier == "seed") {
				f_seed = true;
				seed = atoi(argv[i + 1]);
			} else if (identifier == "alpha")
				alpha = atof(argv[i + 1]);
			else if (identifier == "tl")
				time_limit = atoi(argv[i + 1]);
			else if (identifier == "ip")
				input_path = argv[i + 1];
			else if (identifier == "op")
				output_path = argv[i + 1];
			else if (identifier == "lp")
				log_path = argv[i + 1];
			else if (identifier == "ap")
				alpha_path = argv[i + 1];
			else
				cout << "Invalid option: -" << identifier << "\n";
		}
	}
	if (!f_seed) {
		seed = (int) chrono::steady_clock::now().time_since_epoch().count();
	}
}
 
int main(int argc, char **argv) {
	// Command Line Parameters
	int seed;
	double alpha;
	int time_limit;
	string input_path;
	string output_path;
	string log_path;
	string alpha_path;
	parse_args(argc, argv, seed, alpha, time_limit, input_path, output_path, log_path, alpha_path);
	// File Variables
	FILE *input_file = fopen(input_path.c_str(), "r");
	FILE *output_file = fopen(output_path.c_str(), "a");
	FILE *log_file = fopen(log_path.c_str(), "a");
	FILE *alpha_file = fopen(alpha_path.c_str(), "a");
	string input_name = input_path;
	input_name = input_name.substr(0, (int) input_name.size() - 3);
	// Graph Info Variables
	int n_vertices, n_edges;
	double graph_density;
	readInput(input_file, &n_vertices, &n_edges, graph_density);
	calc_centrality_scores(centrality_scores, edges, rng);
	// Iteration Variables
	vector<int> bs;
	vector<int> vertex_frequency(n_vertices, 1); // Initialy freq(v) = 1 for all v
	mt19937 rng(seed);
	int n_iterations = 0;
	double sol_value_mean = 0;
	int cnt_valid_solutions = 0;
	int freq_incumbent_solution = 0;
	int iteration_incumbent_solution = 0;
	clock_t inicio = clock();
	int incumbent_solution = (int) floor(2.0 * sqrt((double) n_vertices)); // Intial value = floor(2*sqrt(n)-1)	
	// Iterations
	fprintf(log_file, "\nALPHA = %.2lf\nSeed = %d\nInstance = %s\nN vertices = %d\nN edges = %d\nDensity = %.6lf\n\n", alpha, seed, input_name.c_str(), n_vertices, n_edges, graph_density);
	cout << "\nALPHA = " << alpha << "\nSeed  = " << seed << "\nInstance = " << input_name << "\nN vertices = " << n_vertices << "\nN edges = " << n_edges << "\nDensity = " << graph_density << "\n\n";
	double time_to_incumbent = 1e9;
	do {
		n_iterations++;
		pair<vector<int>, bool> ans = construction(n_iterations, centrality_scores, n_vertices, n_edges, graph_density, rng, alpha, incumbent_solution - 1);
		if (!ans.second) {
			continue;
		}
		vector<int> burning_sequence = ans.first;
		if (check_solution(n_vertices, burning_sequence)) {
			sol_value_mean *= (double) cnt_valid_solutions;
			cnt_valid_solutions++;
			sol_value_mean += (double) burning_sequence.size();
			sol_value_mean /= (double) cnt_valid_solutions;
			if ((int) burning_sequence.size() < incumbent_solution) {
				time_to_incumbent = 1.0 * (clock() - inicio) / CLOCKS_PER_SEC;
				bs = burning_sequence;
				incumbent_solution = (int) burning_sequence.size();
				freq_incumbent_solution = 1;
				iteration_incumbent_solution = n_iterations;
				fprintf(log_file, "[Iteration %d] New incumbent solution found! Solution value = %d\n", n_iterations, incumbent_solution);
				cout << "[Iteration " << n_iterations << "] New incumbent solution found! Solution value = " << incumbent_solution << "\n";
			} else if ((int) burning_sequence.size() == incumbent_solution) {
				freq_incumbent_solution++;
			}
		}
	} while (1.0 * (clock() - inicio) / CLOCKS_PER_SEC < time_limit); // limite de 5 minutos
	double time_consumed = 1.0 * (clock() - inicio) / CLOCKS_PER_SEC;
	fprintf(log_file, "\nNumber of iterations = %d\nMean of solution values = %.6lf\n", n_iterations, sol_value_mean);
	cout << "\nNumber of iterations = " << n_iterations << "\nMean of solution values = " << sol_value_mean << "\n";
	// fprintf(output_file, "%s,%.2lf,%d,%d,%.6lf,%d,%d,%d\n", input_name.c_str(), time_consumed, n_iterations, cnt_valid_solutions, 
		// sol_value_mean, incumbent_solution, freq_incumbent_solution, iteration_incumbent_solution);
	// fprintf(output_file, "%s,%d,%d,%.4lf,%d,%d,%.4lf,%d,%d", input_name.c_str(), n_vertices, n_edges, graph_density, n_iterations, incumbent_solution,
	// 	sol_value_mean, freq_incumbent_solution, iteration_incumbent_solution);
	fprintf(output_file, "%s,%.4lf,%.4lf,%d,%d,%d\n", input_name.c_str(), time_consumed, time_to_incumbent,
		iteration_incumbent_solution, freq_incumbent_solution, incumbent_solution);
	fprintf(log_file, "\nBest solution found:\nSolution value = %d\nNumber of rounds = %d\nIteration = %d\n",
		incumbent_solution, freq_incumbent_solution, iteration_incumbent_solution);
	cout << "\nBest solution found:\nSolution value = " << incumbent_solution << "\n";
	cout << "Number of rounds = " << freq_incumbent_solution << "\n";
	cout << "Iteration = " << iteration_incumbent_solution << "\n";

	fprintf(log_file, "\nBurning sequence = ");
	cout << "\nBurning sequence = ";
	for (auto x : bs) {
		fprintf(log_file, "%d ", x);
		cout << x << " ";
	}
	fprintf(log_file, "\n\n---------------------------------------------------------------------------------------------------------------------------------\n");
	cout << "\n\n---------------------------------------------------------------------------------------------------------------------------------\n";
	printf("%d %.2lf\n", incumbent_solution, 1.0 * (clock() - inicio) / CLOCKS_PER_SEC);
	return 0;
}
