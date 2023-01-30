#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 1e6 + 5;

int n, m;
vector<int> adj[MAXN];
int grau[MAXN];
int melhor_solucao;
double phi[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
double media_solucoes[10];
int qtd_solucoes[10];

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

int escolhe(set<int> &queimados, set<int> &st, double ALPHA, mt19937 &rng) {
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

vector<int> greedy_randomized(double ALPHA, mt19937 &rng) {
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
		int escolhido = escolhe(vertices_queimados, vertices_seguros.empty() ? vertices_quase_queimados : vertices_seguros, ALPHA, rng);
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

int pega_alpha(int iteracao, mt19937 &rng) {
	for (int i = 0; i < 10; i++) {
		if (iteracao < (i + 1) * 10)
			return i;
	}
	vector<double> q(10);
	double soma = 0;
	for (int i = 0; i < 10; i++) {
		q[i] = melhor_solucao / media_solucoes[i];
		soma += q[i];
	}
	vector<double> p(10);
	for (int i = 0; i < 10; i++) {
		p[i] = q[i] / soma;
	}
	discrete_distribution<size_t> d{p.begin(), p.end()};
	return d(rng);
}

int main(int argc, char **argv) {
	freopen(argv[1], "r", stdin);	// argv[1] = Arquivo de entrada
	int seed = atoi(argv[2]);				// argv[2] = Seed
	double best_alpha = atof(argv[3]);			// argv[3] = Valor de ALPHA
	int maximo_iteracoes = atoi(argv[4]);	// argv[4] = Numero maximo de iteracoes permitida
	int condicao_parada = atoi(argv[5]);	// argv[5] = Condicao de parada utilizada (nao usado no momento)
	// int criterio_guloso = atoi(argv[6]);	// argv[6] = Funcao gulosa utilizada
	mt19937 rng(seed);
	readInput();
	int numero_de_iteracoes = 0;
	clock_t timer = clock(), timer2;
	int burning_number = 1000000;
	int iteracao_solucao = 0;
	clock_t clock_solucao = clock();
	do {
		int id_alpha = pega_alpha(numero_de_iteracoes++, rng);
		solucoes.push_back(greedy_randomized(phi[id_alpha], rng));
		media_solucoes[id_alpha] *= qtd_solucoes[id_alpha];
		media_solucoes[id_alpha] += (int) solucoes.back().size();
		qtd_solucoes[id_alpha]++;
		media_solucoes[id_alpha] /= qtd_solucoes[id_alpha];
		if (burning_number > (int) solucoes.back().size()) {
			best_alpha = phi[id_alpha];
			burning_number = (int) solucoes.back().size();
			iteracao_solucao = numero_de_iteracoes;
			clock_solucao = clock();
		}
		burning_number = min(burning_number, (int) solucoes.back().size());
		melhor_solucao = burning_number;
		reset();
		if (numero_de_iteracoes == 100)
			timer2 = clock();
	} while (numero_de_iteracoes <= 100 || 1.0 * (clock() - timer2) / CLOCKS_PER_SEC  < 10);
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
	printf("%.2lf\n", best_alpha);									// Valor de ALPHA utilizado na melhor solucao
	printf("%.2lf\n", densidade);									// Densidade
	printf("%d\n", n);												// Numero de vertices
	printf("%d\n", m);												// Numero de arestas
	printf("%d\n", grau_maximo);									// Grau maximo
	printf("%.2lf\n", grau_medio);									// Grau medio
	printf("%d\n", numero_de_iteracoes); 							// Numero de iteracoes efetivamente realizadas
	printf("%.2lf\n", 1.0 * (clock() - timer) / CLOCKS_PER_SEC); 	// Tempo de execucao total
	printf("%d\n", iteracao_solucao);								// Iteracao em que a solucao foi encontrada
	printf("%.2lf\n", 1.0 * (clock_solucao - timer) / CLOCKS_PER_SEC); // Tempo de execucao ate a solucao

	for (int i = 0; i < 10; i++) {
		printf("ALPHA = %.1lf\n", phi[i]);
		printf("qtd de solucoes = %d\n", qtd_solucoes[i]);
		printf("media do valor das solucoes = %.2lf\n", media_solucoes[i]);
	}
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