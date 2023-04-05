#include <bits/stdc++.h>
 
using namespace std;

const int INF = 1e9;
	
vector<vector<int>> adj;
vector<double> centrality_scores;
vector<int> vertices;
vector<pair<int, int>> edges;

enum Status { 
	BURNED, 
	TARGETED, 
	SAFE 
};

bool check_solution(int n_vertices, vector<int> &burning_sequence) {
	vector<int> burned(n_vertices); // maybe use a bitset for efficiency
	vector<int> last_burned;
	int cnt = 0;
	for (auto x : burning_sequence) {
		cnt++;
		if (burned[x])
			return false;
		vector<int> to_burn_now;
		to_burn_now.push_back(x);
		for (int cur_vertex : last_burned) {
			for (int neighbour : adj[cur_vertex]) {
				if (!burned[neighbour]) {
					to_burn_now.push_back(neighbour);
				}
			}
		}
		for (auto v : to_burn_now) {
			burned[v] = 1;
		}
		swap(to_burn_now, last_burned);
	}
	return *min_element(burned.begin(), burned.end()) == 1; // if min is 0, than it didn't burn the whole graph
}

void readInput(FILE* input_file, int *n_vertices, int *n_edges, double &density) {
	fscanf(input_file, "%d %d", n_vertices, n_edges);
	density = 2.0 * (double) (*n_edges) / ((double) (*n_vertices) * (double) ((*n_vertices) - 1));
	adj.resize(*n_vertices);
	edges.resize(*n_edges);
	vertices.resize(*n_vertices);
	iota(vertices.begin(), vertices.end(), 0);
	for (int i = 0; i < *n_edges; i++) {
		int a, b;
		fscanf(input_file, "%d %d", &a, &b);
		adj[a].push_back(b);
		adj[b].push_back(a);
		edges[i] = make_pair(a, b);
	}
	centrality_scores.resize(*n_vertices);
}
 
void calc_centrality_scores(vector<double> &centrality, vector<int> &v, vector<pair<int, int>> &e, mt19937 &rng) {
	double EPS = 1e-9;
	int tamanho = (int) centrality.size();
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
	while (iter <= 1000) {
		swap(x0, x1);
		x1.clear();
		for (auto &[a, b] : e) {
			x1[a] += x0[b];
			x1[b] += x0[a];
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
	for (int i = 0; i < (int) v.size(); i++) {
		centrality[v[i]] = x1[v[i]];
	}
}
 
vector<int> select_vertices(vector<int> candidates, vector<double> &centrality, double density) {
	sort(candidates.begin(), candidates.end(), [&](int a, int b) {
		return centrality[a] > centrality[b]; 
	});
	vector<int> selected_vertices;
	for (int cur : candidates) {
		if (centrality[cur] >= (1 - density) * centrality[candidates[0]] || (int) selected_vertices.size() < 5)
			selected_vertices.push_back(cur);
		else
			break;
	}
	assert(!selected_vertices.empty());
	return selected_vertices;
}

void dfs(int cur, vector<bool> &vis, vector<int> &v, vector<pair<int, int>> &e, vector<int> &status) {
	if (status[cur] != 0) {
		cerr << cur << " NAO EH ZERO\n";
	}
	assert(status[cur] == 0); // Checks if the vertice is safe
	v.push_back(cur);
	vis[cur] = true;
	for (int nxt : adj[cur]) if (!vis[nxt] && status[nxt] == status[cur]) {
		e.emplace_back(cur, nxt);
		dfs(nxt, vis, v, e, status);
	}
}

vector<int> bfs(int source, int round, vector<int> &vertex_labels) {
	vector<int> visited_vertices;
	vertex_labels[source] = round;
	queue<int> q;
	q.push(source);
	while (!q.empty()) {
		int cur = q.front();
		visited_vertices.push_back(cur);
		q.pop();
		for (auto nxt : adj[cur]) {
			if (vertex_labels[cur] + 1 < vertex_labels[nxt]) {
				vertex_labels[nxt] = vertex_labels[cur] + 1;
				q.push(nxt);
			}
		}
	}
	return visited_vertices;
}

pair<vector<int>, bool> construction(int iteration, vector<double> centrality, int n_vertices, int n_edges, double graph_density, mt19937 &rng, double alpha, int K_i) {
	// Round Variables
	int current_round = 1;
	vector<int> burning_sequence;
	set<int> safe(vertices.begin(), vertices.end()), targeted, burned;
	vector<int> vertex_status(n_vertices, 0); // 0 -> safe | 1 -> targeted | 2 -> burned (change this to ENUM)
	vector<int> vertex_labels(n_vertices, INF);

	// Round 1
	vector<int> selected_vertices = select_vertices(vertices, centrality, graph_density);
	int current_activator = selected_vertices[rng() % ((int) selected_vertices.size())];
	burning_sequence.push_back(current_activator);
	vertex_status[current_activator] = 2; // BURNED

 	auto bfs_vertices = bfs(current_activator, current_round, vertex_labels);
	for (auto cur : bfs_vertices) if (vertex_labels[cur] > current_round) {
		// Iterate over all unburned vertices
		if (vertex_labels[cur] <= K_i) {
			safe.erase(cur);
			targeted.insert(cur);
			vertex_status[cur] = 1;
		}
	}
	safe.erase(current_activator);
	burned.insert(current_activator);

	// Rounds t >= 2
	do {
		current_round++;
		vector<bool> visited(n_vertices, false);
		vector<int> cl; // Candidate List
		if (safe.empty()) {
			assert(!targeted.empty());
			cl = vector<int>(targeted.begin(), targeted.end());
		} else {
			for (int cur : safe) {
				assert(vertex_status[cur] == 0);
				if (!visited[cur]) {
					// dfs through cur connected component
					vector<int> component_vertices;
					vector<pair<int, int>> component_edges;
					dfs(cur, visited, component_vertices, component_edges, vertex_status);
					int N = (int) component_vertices.size();
					int M = (int) component_edges.size(); // Here, (a, b) != (b, a), so the real number of edges is M / 2
					double component_density = double(M) / double(1.0 * N * (N - 1));
					calc_centrality_scores(centrality, component_vertices, component_edges, rng);
					vector<int> cl_local = select_vertices(component_vertices, centrality, component_density);
					for (int component_vertex : cl_local) { // maybe use STL merge()
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
			L_M = max(L_M, vertex_labels[i]);
		}
		int b_star = L_M - K_i + current_round;
		int min_benefit = INF;
		int max_benefit = -INF;
		auto b = [&](int v) {
			return b_star - abs(b_star - vertex_labels[v]);
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
		current_activator = rcl[rng() % (int) rcl.size()];
		safe.erase(current_activator);
		targeted.erase(current_activator);
		vertex_status[current_activator] = 2; // BURNED
		bfs_vertices = bfs(current_activator, current_round, vertex_labels);
		burning_sequence.push_back(current_activator);
		if ((int) burning_sequence.size() > K_i) {
			return make_pair(burning_sequence, false);
		}
		for (auto cur : bfs_vertices) if (vertex_labels[cur] > current_round) {
			if (vertex_labels[cur] <= K_i) {
				safe.erase(cur);
				targeted.insert(cur);
				vertex_status[cur] = 1;
			}
		}
		vector<int> to_burn_now;
		to_burn_now.push_back(current_activator);
		for (auto cur : burned) {
			for (auto neighbour : adj[cur]) {
				if (vertex_status[neighbour] == 1) {
					vertex_status[neighbour] = 2; // BURNED
					targeted.erase(neighbour);
					to_burn_now.push_back(neighbour);
					assert(vertex_labels[neighbour] == current_round);
				}
				assert(vertex_status[neighbour] != 0);
			}
		}
		for (auto cur : to_burn_now) {
			for (auto neighbour : adj[cur]) {
				if (vertex_status[neighbour] == 0) {
					safe.erase(neighbour);
					vertex_status[neighbour] = 1;
					targeted.insert(neighbour);
					assert(vertex_labels[neighbour] == current_round + 1);
				}
			}
			burned.insert(cur);
		}
	} while ((int) burned.size() != n_vertices);
	return make_pair(burning_sequence, true);
}

void parse_args(int argc, char **argv, long long &seed, double &alpha, int &time_limit, string &input_path, string &output_path, string &log_path, string &alpha_path) {
	bool f_seed  = false;
	for (int i = 0; i < argc; i++) {
		string str = argv[i];
		if (str[0] == '-') {
			string identifier = str.substr(1);
			if (identifier == "seed") {
				f_seed = true;
				seed = atoll(argv[i + 1]);
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
		seed = (long long) chrono::steady_clock::now().time_since_epoch().count();
	}
}
 
int main(int argc, char **argv) {
	// Command Line Parameters
	long long seed;
	double alpha;
	int time_limit;
	string input_path;
	string output_path;
	string log_path;
	string alpha_path;
	parse_args(argc, argv, seed, alpha, time_limit, input_path, output_path, log_path, alpha_path);
	mt19937 rng(seed);
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
	calc_centrality_scores(centrality_scores, vertices, edges, rng);
	// Iteration Variables
	vector<int> bs;
	vector<int> vertex_frequency(n_vertices, 1); // Initialy freq(v) = 1 for all v
	int n_iterations = 0;
	double sol_value_mean = 0;
	int cnt_valid_solutions = 0;
	int freq_incumbent_solution = 0;
	int iteration_incumbent_solution = 0;
	clock_t inicio = clock();
	int incumbent_solution = (int) floor(2.0 * sqrt((double) n_vertices)) + 1; // Intial value = floor(2*sqrt(n)-1)	
	// Iterations
	fprintf(log_file, "\nALPHA = %.2lf\nSeed = %lld\nInstance = %s\nN vertices = %d\nN edges = %d\nDensity = %.6lf\n\n", alpha, seed, input_name.c_str(), n_vertices, n_edges, graph_density);
	cout << "\nALPHA = " << alpha << "\nSeed  = " << seed << "\nInstance = " << input_name << "\nN vertices = " << n_vertices << "\nN edges = " << n_edges << "\nDensity = " << graph_density << "\n\n";
	double time_to_incumbent = 1e9;
	do {
		n_iterations++;
		pair<vector<int>, bool> ans = construction(n_iterations, centrality_scores, n_vertices, n_edges, graph_density, rng, alpha, incumbent_solution - 1);
		if (!ans.second) {
			// cout << "Parou a execucao:\nSolucao parcial = ";
			// for (auto x : ans.first) {
			// 	cout << x << " ";
			// }
			// cout << "(" << (int) ans.first.size() << ")\n";
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
	} while (1.0 * (clock() - inicio) / CLOCKS_PER_SEC < time_limit);
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