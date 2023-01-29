#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 1e6 + 5;

int n, m;
vector<int> adj[MAXN];
int grau[MAXN];

void readInput() {
	set<int> st;
	scanf("%d %d", &n, &m);
	for (int i = 0; i < m; i++) {
		int a, b;
		scanf("%d %d", &a, &b);
		assert(0 <= a && a < n && 0 <= b && b < n);
		adj[a].push_back(b);
		adj[b].push_back(a);
		grau[a]++;
		grau[b]++;
	}
}

int escolhe(set<int> queimados, set<int> &st, double ALPHA, mt19937 &rng, int criterio_guloso) {
	if (st.empty()) {
		assert(false);
		return -1;
	}
	int maximo = 0;
	int minimo = (int) 1e9;
	for (auto &x : st) {
		assert(!queimados.count(x));
		maximo = max(maximo, grau[x]);
		minimo = min(minimo, grau[x]);
	}
	int lowerBound = maximo - (int) floor(ALPHA * (maximo - minimo));
	vector<int> v;
	for (auto &x : st) {
		if (grau[x] >= lowerBound)
			v.push_back(x);
	}
	int cara = v[rng() % ((int) v.size())];
	st.erase(cara);
	return cara;
}

bool greedy_randomized(double ALPHA, mt19937 &rng) {
	set<int> vertices_seguros;
	set<int> vertices_quase_queimados;
	set<int> vertices_queimados;
	for (int i = 0; i < n; i++) {
		safe.insert(i);
	}
	vector<int> solucao_viavel;
	// Se o vizinho de um vertice queimado nao esta queimado, entao o colocamos no quase queimado
	while ((int) vertices_queimados.size() < n) {
		for (auto &atual : vertices_queimados) {
			for (auto &vizinho : adj[atual]) {
				if (!vertices_queimados.count(vizinho))
					vertices_seguros.erase(vizinho);
					vertices_quase_queimados.insert(vizinho);
			}
		}
		int escolhido = escolhe(vertices_seguros.empty() ? vertices_quase_queimados : vertices_seguros, ALPHA, rng);
		if (escolhido == -1)
			assert(false);
	}
	return true;
}

int main() {
	freopen(argv[1], "r", stdin);	// argv[1] = Arquivo de entrada
	int seed = atoi(argv[2]);				// argv[2] = Seed
	double ALPHA = atof(argv[3]);			// argv[3] = Valor de ALPHA
	int maximo_iteracoes = atoi(argv[4]);	// argv[4] = Numero maximo de iteracoes permitida
	int condicao_parada = atoi(argv[5]);	// argv[5] = Condicao de parada utilizada (nao usado no momento)
	// int criterio_guloso = atoi(argv[6]);	// argv[6] = Funcao gulosa utilizada
	mt19937 rng(seed);
	readInput();
	int numero_de_iteracoes = 0;
	clock_t timer = clock();
	do {
		numero_de_iteracoes++;
		greedy_randomized(ALPHA, rng);
	} while (1.0 * (clock() - timer) / CLOCKS_PER_SEC  < 10);
	return 0;
}