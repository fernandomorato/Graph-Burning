#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 1e6 + 5;
const double eps = 1e-3;

namespace EigenvectorCentrality {

	tuple<vector<long double>, long double, int> get_scores(vector<vector<int>> &adj_matrix, long double EPS, mt19937 &rng) {
		int n = (int) adj_matrix.size();
		vector<long double> x0(n), x1(n);
		uniform_real_distribution<long double> unif(0, 1);
		long double norm = 0;
		for (int i = 0; i < n; i++) {
			x1[i] = unif(rng);
			norm += x1[i] * x1[i];
		}
		norm = sqrt(norm);
		for (int i = 0; i < n; i++) {
			x1[i] /= norm;
		}
		int iter = 0;
		long double lambda = 0, lambdaOld = 0, m = 0;
		while (iter <= 100) {
			swap(x0, x1);
			for (int i = 0; i < n; i++) {
				x1[i] = 0;
				for (int j = 0; j < n; j++) {
					x1[i] += (long double) adj_matrix[i][j] * x0[j];
				}
			}
			norm = 0;
			for (int i = 0; i < n; i++) {
				norm += x1[i] * x1[i];
			}
			norm = sqrt(norm);
			lambda = 0;
			for (int i = 0; i < n; i++) {
				x1[i] /= norm;
				lambda += x1[i] * x0[i];
			}
			lambda = sqrt(lambda);
			iter++;
			if (fabs((lambda - lambdaOld) / lambda) < EPS)
				break;
			lambdaOld = lambda;
		}
		return make_tuple(x1, lambda, iter);
	}
}

int n, m;
vector<int> adj[MAXN];
int grau[MAXN];
vector<vector<int>> adj_matrix;
vector<long double> centrality_scores;
long double eigenvalue, eigenvector_centrality[MAXN];

void readInput() {
	set<int> st;
	cin >> n >> m;
	centrality_scores.resize(n);
	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		assert(0 <= a && a < n && 0 <= b && b < n);
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	// auto ec_ans = EigenvectorCentrality::get_scores(adj_matrix, 1e-9, rng);
	// centrality_scores = get<0>(ec_ans);
	// eigenvalue = get<1>(ec_ans);
	for (int i = 0; i < n; i++) {
		cin >> centrality_scores[i];
	}
}

int escolhe(set<int> &queimados, set<int> &st, double ALPHA, mt19937 &rng) {
	if (st.empty()) {
		assert(false);
		return -1;
	}
	// long double maximo = -1e9;
	// long double minimo = 1e9;
	vector<pair<long double, int>> valores;
	for (auto &x : st) {
		assert(!queimados.count(x));
		valores.emplace_back(eigenvector_centrality[x], x);
	}
	sort(valores.rbegin(), valores.rend());
	long double lowerBound = valores[0].first - (long double) ALPHA * (valores[0].first - valores.back().first);
	vector<int> v;
	for (int i = 0; i < (int) valores.size(); i++) {
		if (valores[i].first < lowerBound)
			break;
		v.push_back(valores[i].second);
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
	int cnt = 0;
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
				eigenvector_centrality[vizinho] -= eigenvector_centrality[x];
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
		eigenvector_centrality[i] = centrality_scores[i];
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
	// return 0;
	int numero_de_iteracoes = 0;
	clock_t timer = clock();
	int burning_number = 1000000;
	int iteracao_solucao = 0;
	clock_t clock_solucao = clock();
	do {
		numero_de_iteracoes++;
		solucoes.push_back(greedy_randomized(ALPHA, rng));
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