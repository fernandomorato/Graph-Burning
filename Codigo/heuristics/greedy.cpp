#include <bits/stdc++.h>
 
using namespace std;
 
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
 
const int MAXN = 54574;
const int INF = 1e9;
 
double ALPHA;
double GRAPH_DENSITY;
vector<double> centrality_scores;
 
int N, M;
int K_BOUND, K_i;
int label[MAXN];
int status[MAXN]; // 0 -> safe | 1 -> targeted | 2 -> burned
vector<vector<int>> adj_matrix;
vector<int> adj[MAXN];
 
bool visited[MAXN];

bool status_checker[MAXN];

// bool check_solution(vector<int> burning_sequence) {
// 	for (int i = 0; i < N; i++) {
// 		status_checker[i] = 0;
// 	}
// 	vector<int> v, nv;
// 	for (int x : burning_sequence) {
// 		v.push_back(x);
// 		for (int xx : v) {
// 			status_checker[xx] = 1;
// 			if (label[xx] != label[x])
// 				cerr << label[xx] << " => " << label[x] << '\n';
// 			assert(label[xx] == label[x]);
// 			for (int viz : adj[xx]) {
// 				// cerr << label[xx] << " -> " << label[viz] << '\n';
// 				// assert(label[viz] == label[x] || label[viz] == label[x] + 1);
// 				if (label[viz] == label[x] + 1)
// 					nv.push_back(viz);
// 			}
// 		}
// 		swap(v, nv);
// 		nv.clear();
// 	}
// 	for (int i = 0; i < N; i++) {
// 		if (status_checker[i] == 0)
// 			return false;
// 	}
// 	return true;
// }

void readInput() {
	cin >> N >> M;
	adj_matrix.resize(N);
	for (int i = 0; i < N; i++)
		adj_matrix[i].resize(N);
	for (int i = 0; i < M; i++) {
		int a, b;
		cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
		adj_matrix[a][b] = adj_matrix[b][a] = 1;
	}
	centrality_scores.resize(N);
	for (int i = 0; i < N; i++) {
		adj_matrix[i][i] = 1;
		label[i] = INF;
		cin >> centrality_scores[i];
	}
}
 
int bfs(int source, int round) {
	int cnt = 0;
	label[source] = round;
	queue<int> q;
	q.push(source);
	while (!q.empty()) {
		cnt++;
		int cur = q.front();
		q.pop();
		for (auto x : adj[cur]) {
			if (label[cur] + 1 < label[x]) {
				label[x] = label[cur] + 1;
				q.push(x);
			}
		}
	}
	return cnt;
}
 
void calc_centrality_scores(vector<double> &centrality_scores, vector<int> &vertices, mt19937 &rng) {
	double EPS = 1e-3;
	int tamanho = (int) vertices.size();
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
		int ptr1 = 0;
		for (int i : vertices) {
			x1[ptr1++] = 0;
			int ptr2 = 0;
			for (int j : vertices) {
				x1[i] += (double) adj_matrix[i][j] * x0[ptr2++];
			}
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
	// cerr << '\n';
	// for (int x : selected_vertices) {
	// 	cerr << x << "(" << centrality[x] << ") ";
	// }
	// cerr << '\n';
	return selected_vertices;
}
 
int qtd_arestas_componente;
vector<int> vertices_componente;
 
void dfs(int current_vertex) {
	cerr << "DFS --- " << current_vertex << "(" << status[current_vertex] << ")\n";
	assert(status[current_vertex] == 0);
	vertices_componente.push_back(current_vertex);
	visited[current_vertex] = true;
	for (int neighbour : adj[current_vertex]) {
		if (status[neighbour] == status[current_vertex])
			qtd_arestas_componente++;
		if (visited[neighbour] || status[neighbour] != status[current_vertex]) 
			continue;
		dfs(neighbour);
	}
	// cerr << current_vertex << " " << qtd_arestas_componente << ' ' << (int) vertices_componente.size() << '\n';
}
 
vector<int> construction(vector<double> centrality, mt19937 &rng) {
	for (int i = 0; i < N; i++) {
		label[i] = INF;
	}
	// cerr << "Densidade = " << GRAPH_DENSITY << '\n';
	int current_round = 1;
	vector<int> vertices(N);
	iota(vertices.begin(), vertices.end(), 0);
	vector<int> burning_sequence;
	vector<int> selected_vertices = select_vertices(vertices, centrality, GRAPH_DENSITY);
	assert(!selected_vertices.empty());
	int initial_vertex = selected_vertices[rng() % ((int) selected_vertices.size())];
	// cerr << "Vertice Inicial = " << initial_vertex << '\n';
	int xd = bfs(initial_vertex, current_round++);
	// cerr << "Visited on BFS(" << initial_vertex << ") = " << xd << '\n';	
	burning_sequence.push_back(initial_vertex);
 
	// A PARTIR DAQUI O FAREMOS AS RODADAS ONDE t >= 2
 
	set<int> safe, targeted, burned;
	for (int i = 0; i < N; i++) {
		visited[i] = false;
		if (label[i] > K_i) {
			status[i] = 0;
			safe.insert(i);
		} else if (label[i] != 1) {
			status[i] = 1;
			targeted.insert(i);
		} else {
			assert(label[i] == 1);
			status[i] = 2;
			burned.insert(i);
		}
	}
 
	do {
		for (int i = 0; i < N; i++) {
			for (int viz : adj[i]) {
				assert(abs(label[i] - label[viz]) <= 1);
			}
		}
		vector<int> cl; // Candidate List
		if (safe.empty()) {
			cl = vector<int>(targeted.begin(), targeted.end());
		} else {
			vector<pair<double, vector<int>>> connected_components; // [i] = {densidade, vertices da componente}
			for (int i : safe) {
				if (!visited[i]) {
					qtd_arestas_componente = 0;
					vertices_componente.clear();
					dfs(i);
					int qtd_vertices_componente = (int) vertices_componente.size();
					double density = 2.0 * double(qtd_arestas_componente * 0.5) / (double(qtd_vertices_componente) * double(qtd_vertices_componente - 1));
					calc_centrality_scores(centrality, vertices_componente, rng);
					// cerr << "info -> " << density << " [";
					// for (int i = 0; i < (int) vertices_componente.size(); i++) {
					// 	if (i) cerr << ", ";
					// 	cerr << vertices_componente[i];
					// }
					// cerr << "]\n";
					connected_components.emplace_back(density, vertices_componente);
				}
			}
			// cerr << "COMPONENTES -> " << (int) connected_components.size() << '\n';
			for (auto &cc : connected_components) {
				double density = cc.first;
				vector<int> vertices = cc.second;
				vector<int> cl_local = select_vertices(vertices, centrality, density);
				// [ TALVEZ SEJA MELHOR USAR UMA FUNCAO DE MERGE PRA NAO DAR TANTO PUSH_BACK ]
				for (int current_vertex : cl_local) {
					cl.push_back(current_vertex);
				}
			}
		}
		assert(!cl.empty());

		// cerr << "Iteracao " << current_round << "\n";
		// cerr << "Lista de candidatos: [";
		// for (int i = 0; i < (int) cl.size(); i++) {
		// 	if (i) cerr << ", ";
		// 	cerr << cl[i];
		// }
		// cerr << "]\n";

		// Agora que temos a nossa cl, precisamos definir a funcao de beneficio para compor uma RCL
		// definimos b(v) = b* - | b* - l(v) |, onde b* = L_M - K_i + i e L_M = maior label
 
		int L_M = 0;
		for (int i = 0; i < N; i++) { // Estou pegando o rotulo maximo considerando TODOS (talvez usar soh a cl)
			L_M = max(L_M, label[i]);
		}
		int b_star = L_M - K_i + current_round;
		int min_benefit = INF;
		int max_benefit = -INF;
		auto b = [&](int v) {
			return b_star - abs(b_star - label[v]);
		};
		for (int x : cl) {
			min_benefit = min(min_benefit, b(x));
			max_benefit = max(max_benefit, b(x));
		}
		vector<int> rcl;
		for (int x : cl) {
			if (b(x) >= max_benefit - ALPHA * (max_benefit - min_benefit)) {
				rcl.push_back(x);
			}
		}
		assert(!rcl.empty());
		int next_activator = rcl[rng() % (int) rcl.size()];
		// cerr << "CHOOSEN = " << next_activator << '\n';
		label[next_activator] = current_round;
		int nvis = bfs(next_activator, current_round++);
		// cerr << "[LABEL = " << label[next_activator] << "] Visited on BFS(" << next_activator << ") = " << nvis << '\n';
		burning_sequence.push_back(next_activator);
		set<int> to_burn;
		to_burn.insert(next_activator);
		for (int current_vertex : burned) {
			for (int neighbour : adj[current_vertex]) {
				assert(status[neighbour] != 0);
				if (status[neighbour] != 1)
					continue;
				status[neighbour] = 2; // queimei
				targeted.erase(neighbour);
				to_burn.insert(neighbour);
			}
		}
		for (int current_vertex : to_burn) {
			burned.insert(current_vertex);
			for (int neighbour : adj[current_vertex]) {
				if (status[neighbour] != 0)
					continue;
				status[neighbour] = 1;
				safe.erase(neighbour);
				targeted.insert(neighbour);
			}
		}
	} while ((int) burned.size() != N);
	// cerr << '\n';
	return burning_sequence;
}
 
int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	readInput();
	ALPHA = 0.5;
	mt19937 rng(42);
	int iteracoes_realizadas = 0;
	clock_t inicio = clock(), final = clock();
	K_BOUND = K_i = (int) floor(2.0 * sqrt((double) N) - 1.0); // bound for bn(G)
	GRAPH_DENSITY = 2.0 * (double) M / (1.0 * (double) N * (double)(N - 1)); // D = 2|E| / (|V| * (|V| - 1))
	int incumbent_solution = K_BOUND;
	int cnt_valid = 0;
	do {
		auto burning_sequence = construction(centrality_scores, rng);
		// if (check_solution(burning_sequence)) {
			incumbent_solution = min(incumbent_solution, (int) burning_sequence.size());
			cnt_valid++;
		// }
		// armazenar coisas referentes aa sequencia encontrada
		iteracoes_realizadas++;
		// cerr << "Burning Sequence at iteration " << iteracoes_realizadas << ": [";
		// for (int i = 0; i < (int) burning_sequence.size(); i++) {
		// 	if (i) cerr << ", ";
		// 	cerr << burning_sequence[i];
		// }
		// cerr << "] -> " << (int) burning_sequence.size() << '\n';
		final = clock();
	} while (/*iteracoes_realizadas < 1000 &&*/ 1.0 * (clock() - inicio) / CLOCKS_PER_SEC < 60); // limite de 5 minutos
	cerr << "\nIterations: " << iteracoes_realizadas << '\n';
	cerr << "Valid Solutions: " << cnt_valid << '\n';
	cerr << "Solution: " << incumbent_solution << '\n';
	cerr << "Time elapsed: " << 1.0 * (final - inicio) / CLOCKS_PER_SEC << "s\n\n";
	cout << "Solution: " << incumbent_solution << '\n';
	return 0;
}