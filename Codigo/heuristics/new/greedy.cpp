#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 6e4 + 5;
const int INF = 1e9;


double ALPHA;
double GRAPH_DENSITY;
vector<double> centrality_scores;

int N, M;
int K_BOUND, K_i;
int label[MAXN];
int status[MAXN]; // 0 -> safe | 1 -> targeted | 2 -> burned
int adj_matrix[MAXN][MAXN];
vector<int> adj[MAXN];

bool visited[MAXN];

void readInput() {
	cin >> N >> M;
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

void bfs(int source, int round) {
	label[source] = round;
	queue<int> q;
	q.push(source);
	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		for (auto x : adj[cur]) {
			if (label[cur] + 1 < label[x]) {
				label[x] = label[cur] + 1;
				q.push(x);
			}
		}
	}
}

int choose_vertex(set<int> &possible_vertices, mt19937 &rng) {
	return 0;
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
	return selected_vertices;
}

pair<int, vector<int>> dfs(int current_vertex) {
	assert(status[current_vertex] == 0);
	visited[current_vertex] = true;
	int edges = 0;
	vector<int> vertices(1, current_vertex);
	for (int neighbour : adj[current_vertex]) {
		if (visited[neighbour] || status[neighbour] != status[current_vertex]) 
			continue;
		auto [edges_neighbour, vertices_neighbour] = dfs(neighbour);
		edges += edges_neighbour + 1;
		for (int x : vertices_neighbour) {
			vertices.push_back(x);
		}
	}
	return make_pair(edges, vertices);
}

vector<int> construction(vector<double> centrality, mt19937 &rng) {
	int current_round = 1;
	vector<int> vertices(N);
	iota(vertices.begin(), vertices.end(), 0);
	vector<int> burning_sequence;
	vector<int> selected_vertices = select_vertices(vertices, centrality, GRAPH_DENSITY);
	assert(!selected_vertices.empty());
	int initial_vertex = selected_vertices[rng() % ((int) selected_vertices.size())];
	bfs(initial_vertex, current_round++);
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
		vector<int> cl; // Candidate List
		if (safe.empty()) {
			cl = vector<int>(targeted.begin(), targeted.end());
		} else {
			vector<pair<double, vector<int>>> connected_components; // [i] = {densidade, vertices da componente}
			for (int i = 0; i < N; i++) {
				if (!label[i] && !visited[i]) {
					auto [qtd_arestas_componente, vertices_componente] = dfs(i);
					int qtd_vertices_componente = (int) vertices_componente.size();
					double densidade = 2.0 * double(qtd_arestas_componente) / (double(qtd_vertices_componente) * double(qtd_vertices_componente - 1));
					calc_centrality_scores(centrality, vertices_componente, rng);
					connected_components.emplace_back(densidade, vertices_componente);
				}
			}
			for (auto &cc : connected_components) {
				double density = cc.first;
				vector<int> vertices_componente = cc.second;
				vector<int> cl_local = select_vertices(vertices_componente, centrality, density);
				// [ TALVEZ SEJA MELHOR USAR UMA FUNCAO DE MERGE PRA NAO DAR TANTO PUSH_BACK ]
				for (int current_vertex : cl_local) {
					cl.push_back(current_vertex);
				}
			}
		}
		assert(!cl.empty());

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
		set<int> to_burn;
		to_burn.insert(next_activator);
		for (int current_vertex : burned) {
			for (int neighbour : adj[current_vertex]) {
				if (label[neighbour] != 1)
					continue;
				label[neighbour] = 2; // queimei
				targeted.erase(neighbour);
				to_burn.insert(neighbour);
			}
		}
		for (int current_vertex : to_burn) {
			burned.insert(current_vertex);
			for (int neighbour : adj[current_vertex]) {
				if (label[neighbour] != 0)
					continue;
				label[neighbour] = 1;
				safe.erase(neighbour);
				targeted.insert(neighbour);
			}
		}
	} while ((int) burned.size() != N);
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
	do {
		K_i = min(K_BOUND, incumbent_solution - 1);
		auto burning_sequence = construction(centrality_scores, rng);
		incumbent_solution = min(incumbent_solution, (int) burning_sequence.size());
		// armazenar coisas referentes aa sequencia encontrada
		iteracoes_realizadas++;
		final = clock();
	} while (1.0 * (clock() - inicio) / CLOCKS_PER_SEC < 600); // limite de 10 minutos
	return 0;
}