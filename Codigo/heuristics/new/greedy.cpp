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
bool blocked[MAXN];

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

vector<int> construction(vector<double> centrality, mt19937 &rng) { // vetor por referencia?
	vector<int> vertices(N), burning_sequence;
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

	return {initial_vertex};

	bfs(initial_vertex);
	set<int> safe, targeted, burned;
	for (int i = 0; i < N; i++) {
		safe.insert(i);
	}
	int current_round = 1; // para saber quem será queimado
	burning_sequence.push_back(initial_vertex);
	burned.insert(initial_vertex);
	safe.erase(initial_vertex);
	for (int i = 0; i < N; i++) {
		if (i != initial_vertex && label[i] < BURNING_NUMBER) {
			blocked[i] = true;
			targeted.insert(i);
			safe.erase(i);
		}
	}
	vector<pair<double, set<int>>> connected_components; // densidade, vertices da componente
	while (!safe.empty() || !targeted.empty()) {
		int vertices_count = 0;
		int edges_count = 0;
		set<int> st;
		for (int i = 0; i < N; i++) {
			// dfs();
		}
		set<int> candidate_list;
		for (auto &cc : connected_components) {

		}
		int to_burn = choose_vertex(candidate_list, rng);
	}
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