#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 6e4 + 5;
const int INF = 1e9;

int N, M;
vector<int> adj[MAXN];
double GRAPH_DENSITY, ALPHA;
int BURNING_NUMBER;
vector<double> centrality_scores;
int m_adj[MAXN][MAXN], label[MAXN];
bool blocked[MAXN], visited[MAXN];

void readInput() {
	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		int a, b;
		cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
		m_adj[a][b] = m_adj[b][a] = 1;
	}
	GRAPH_DENSITY = 2.0 * (double) M / (1.0 * (double) N * (double)(N - 1)); // D = 2|E| / (|V| * (|V| - 1))
	BURNING_NUMBER = (int) floor(2 * sqrt((double) N) - 1.0); // bound for bn(G)
	centrality_scores.resize(N);
	for (int i = 0; i < N; i++) {
		m_adj[i][i] = 1;
		label[i] = INF;
		cin >> centrality_scores[i];
	}
}

void bfs(int source) {
	label[source] = 1;
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

set<int> dfs(int current_vertex) {
	for (int neighbour : adj[current_vertex]) {

	}
}

void calc_centrality_scores(vector<double> &centrality_scores, set<int> &vertices, mt19937 &rng) {
	int EPS = 1e-3;
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

vector<int> construction(vector<double> centrality, mt19937 &rng) { // vetor por referencia?
	vector<int> vertices(N);
	iota(vertices.begin(), vertices.end(), 0);
	double best_centrality_score = *max_element(centrality.begin(), centrality.end());
	vector<int> selected_vertices;
	for (int i = 0; i < N; i++) {
		if (centrality[i] >= (1 - GRAPH_DENSITY) * best_centrality_score)
			selected_vertices.push_back(i);
	}
	if ((int) selected_vertices.size() < 5) {
		selected_vertices.clear();
		vector<pair<double, int>> aux;
		aux.resize((int) centrality.size());
		for (int i = 0; i < (int) aux.size(); i++) {
			aux[i] = make_pair(centrality[i], i);
		}
		sort(aux.rbegin(), aux.rend());
		for (int i = 0; i < 5; i++) {
			selected_vertices.push_back(aux[i].second);
		}
	}
	assert(!selected_vertices.empty());
	int initial_vertex = selected_vertices[rng() % ((int) selected_vertices.size())];

	// return {initial_vertex};

	bfs(initial_vertex);
	set<int> safe, targeted, burned;
	for (int i = 0; i < N; i++) {
		if (label[i] >= BURNING_NUMBER) {
			safe.insert(i);
		} else {
			blocked[i] = true;
			if (label[i] == 1)
				burned.insert(i);
			else
				targeted.insert(i);
		}
	}
	int current_round = 1; // para saber quem será queimado
	vector<int> burning_sequence;
	burning_sequence.push_back(initial_vertex);
	current_round++; // Comecamos no round 2
	do {
		// Aqui, vamos produzir as componentes conexas com uma dfs
		vector<pair<double, set<int>>> connected_components; // [i] = {densidade, vertices da componente}
		for (int i = 0; i < N; i++) {
			if (!blocked[i] && !visited[i]) {
				int qtd_arestas_componente = 0;
				set<int> vertices_componente = dfs(i, arestas);
				int qtd_vertices_componente = (int) vertices_componente.size();
				double densidade = 2.0 * double(qtd_arestas_componente) / (double(qtd_vertices_componente) * double(qtd_vertices_componente - 1));
				calc_centrality_scores(centrality, vertices_componente, rng);
				connected_components.emplace_back(densidade, vertices_componente);
			}
		}
		vector<int> cl; // Candidate List
		for (auto &cc : connected_components) {
			double densidade = cc.first;
			set<int> vertices_componente = cc.second;
			vector<int> cl_local;
			for (auto current_vertex : vertices_componente) {
				if (safe.count(current_vertex)) {
					cl_local.push_back(current_vertex);
				}
			}
			sort(cl_local.rbegin(), cl_local.rend(), [&](int &a, int &b) {
				return centrality[a] > centrality[b];
			});
			for (auto current_vertex : cl_local) {
				if (centrality[current_vertex] >= (1.0 - densidade) * best_centrality_score_local)
					cl.push_back(current_vertex);
			}
		}
		if (cl.empty()) assert(safe.empty());
		// for (int i = 0; i < N; i++) {
		// 	visited[i] = false;
		// }
		for (int i = 0; i < N; i++) {
			if (label[i] == current_round) {
				safe.erase(i);
				targeted.erase(i);
				burned.insert(i);
				blocked[i] = true;
			} else if (label[i] == current_round + 1) {
				safe.erase(i);
				targeted.insert(i);
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
	do {
		auto burning_sequence = construction(centrality_scores, rng);
		BURNING_NUMBER = min(BURNING_NUMBER, (int) burning_sequence.size());
		// armazenar coisas referentes aa sequencia encontrada
		iteracoes_realizadas++;
		final = clock();
	} while (1.0 * (clock() - inicio) / CLOCKS_PER_SEC < 600); // limite de 10 minutos
	return 0;
}