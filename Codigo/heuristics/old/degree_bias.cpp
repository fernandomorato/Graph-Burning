#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 1e6 + 5;

int n, m;
vector<int> adj[MAXN];
int grau[MAXN];
int frequencia[MAXN];

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

int escolhe_bias(set<int> &st, double ALPHA, mt19937 &rng) {
	vector<pair<int, int>> b; // bias
	for (auto &x : st) {
		b.emplace_back(frequencia[x], x);
	}
	sort(b.rbegin(), b.rend());
	vector<pair<int, int>> rank = b;
	rank[0].first = 1;
	for (int i = 1; i < (int) b.size(); i++) {
		rank[i].first = rank[i - 1].first + (b[i].first != b[i - 1].first ? 1 : 0);
	}
	vector<double> probabilidades((int) rank.size());
	for (int i = 0; i < (int) rank.size(); i++) {
		probabilidades[i] = 1.0 / rank[i].first;
	}
	discrete_distribution<size_t> d{probabilidades.begin(), probabilidades.end()};
	return rank[d(rng)].second;
}

int escolhe(set<int> &queimados, set<int> &st, double ALPHA, mt19937 &rng, bool bias) {
	if (bias)
		return escolhe_bias(st, ALPHA, rng);
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
	assert(!v.empty());
	int cara = v[rng() % ((int) v.size())];
	st.erase(cara);
	return cara;
}

vector<int> greedy_randomized(double ALPHA, mt19937 &rng, int numero_de_iteracoes) {
	set<int> vertices_seguros;
	set<int> vertices_quase_queimados;
	set<int> vertices_queimados;
	for (int i = 0; i < n; i++) {
		vertices_seguros.insert(i);
	}
	vector<int> solucao_viavel;
	// Se o vizinho de um vertice queimado nao esta queimado, entao o colocamos no quase queimado
	while ((int) vertices_queimados.size() < n) {
		for (auto &atual : vertices_queimados) {
			for (auto &vizinho : adj[atual]) {
				if (!vertices_queimados.count(vizinho))
					vertices_quase_queimados.insert(vizinho);
			}
		}
		int escolhido = escolhe(vertices_queimados, vertices_seguros.empty() ? vertices_quase_queimados : vertices_seguros, ALPHA, rng, numero_de_iteracoes >= 100);
		frequencia[escolhido]++;
		solucao_viavel.push_back(escolhido);
		vertices_quase_queimados.insert(escolhido);
		for (auto &x : vertices_quase_queimados) {
			for (auto &vizinho : adj[x]) {
				vertices_seguros.erase(vizinho);
				grau[vizinho]--;
			}
			vertices_queimados.insert(x);
			vertices_seguros.erase(x);
		}
		vertices_quase_queimados.clear();
	}
	return solucao_viavel;
}

void reset() {
	for (int i = 0; i < n; i++) {
		grau[i] = (int) adj[i].size();
	}
}

vector<vector<int>> solucoes;

int main(int argc, char **argv) {
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
	int burning_number = 1000000;
	int iteracao_solucao = 0;
	clock_t clock_solucao = clock();
	do {
		solucoes.push_back(greedy_randomized(ALPHA, rng, numero_de_iteracoes++));
		if (burning_number > (int) solucoes.back().size()) {
			burning_number = (int) solucoes.back().size();
			iteracao_solucao = numero_de_iteracoes;
			clock_solucao = clock();
		}
		burning_number = min(burning_number, (int) solucoes.back().size());
		reset();
	} while (1.0 * (clock() - timer) / CLOCKS_PER_SEC  < 10);
	int grau_maximo = 0;
	long long s = 0;
	for (int i = 0; i < n; i++) {
		grau_maximo = max(grau_maximo, grau[i]);
		s += grau[i];
	}
	double grau_medio = 1.0 * s / (double) n;
	double densidade = 2.0 * m / (1.0 * n * (n - 1));
	// OUTPUT
	printf("%d\n", burning_number);									// Burning Number encontrado
	printf("%.2lf\n", ALPHA);										// Valor de ALPHA utilizado
	printf("%.2lf\n", densidade);									// Densidade
	printf("%d\n", n);												// Numero de vertices
	printf("%d\n", m);												// Numero de arestas
	printf("%d\n", grau_maximo);									// Grau maximo
	printf("%.2lf\n", grau_medio);									// Grau medio
	printf("%d\n", numero_de_iteracoes); 							// Numero de iteracoes efetivamente realizadas
	printf("%.2lf\n", 1.0 * (clock() - timer) / CLOCKS_PER_SEC); 	// Tempo de execucao total
	printf("%d\n", iteracao_solucao);								// Iteracao em que a solucao foi encontrada
	printf("%.2lf\n", 1.0 * (clock_solucao - timer) / CLOCKS_PER_SEC); // Tempo de execucao ate a solucao

	// printf("Tempo discorrido: %.2lfs ~ 10s\n", 1.0 * (clock() - timer) / CLOCKS_PER_SEC);
	// printf("Iteracoes realizadas em 10 segundos: %d\n\n", numero_de_iteracoes);
	// for (int i = 0; i < numero_de_iteracoes; i++) {
	// 	printf("Solucao %d -> [", i);
	// 	for (int j = 0; j < (int) solucoes[i].size(); j++) {
	// 		if (j) printf(", ");
	// 		printf("%d", solucoes[i][j]);
	// 	}
	// 	printf("] (%d)\n", (int) solucoes[i].size());
	// }
	return 0;
}