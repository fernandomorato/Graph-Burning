#include <bits/stdc++.h>

using namespace std;

// mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

vector<pair<int, vector<int>>> solutions; // {valor, lista}

const int MAXN = 2e5 + 5;
clock_t timer;

struct Node {
	vector<Node*> vizinhos;
	int id;
	int grau;
	bool queimado;
	long double eigenvector_centrality;

	Node() {}

	Node(int _id) {
		id = _id;
		grau = 0;
		queimado = false;
		eigenvector_centrality = 0;
	}

	void addEdge(Node *u) {
		vizinhos.push_back(u);
		grau++;
	}
};

int n, m;
Node *vertice[MAXN];
vector<int> adj[MAXN];
int grau[MAXN];
long double eigenvalue;
vector<vector<int>> adj_matrix;

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
		cout << '\n';
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

inline int g(int v, int criterio_guloso) { 
	if (criterio_guloso == 0) return vertice[v]->grau; 
	return vertice[v]->eigenvector_centrality;
}

int escolhe(set<int> &st, double ALPHA, mt19937 &rng, int criterio_guloso) {
	if (st.empty())
		return -1;
	int mx = 0;
	int mn = (int) 1e9;
	for (auto &x : st) {
		assert(!vertice[x]->queimado);
		mx = max(mx, g(x, criterio_guloso));
		mn = min(mn, g(x, criterio_guloso));
	}
	int lowerBound = mx - (int) floor(ALPHA * (mx - mn));
	vector<int> v;
	for (auto &x : st) {
		if (g(x, criterio_guloso) >= lowerBound)
			v.push_back(x);
	}
	int cara = v[rng() % ((int) v.size())];
	st.erase(cara);
	return cara;
}

void update_greedy_function(Node *node, int criterio_guloso) {
	for (auto &viz : node->vizinhos) {
		if (criterio_guloso == 0)
			viz->grau--;
		else if (criterio_guloso == 1)
			viz->eigenvector_centrality -= node->eigenvector_centrality;
	}
}

pair<vector<int>, bool> greedy(double ALPHA, mt19937 &rng, int criterio_guloso) {
	set<int> safe; // vertices que estao safe
	set<int> quase; // vertices que estao quase queimados
	set<int> burned; // vertices que estao queimados
	for (int i = 0; i < n; i++) {
		safe.insert(i);
		vertice[i]->queimado = false;
		vertice[i]->grau = grau[i];
	}
	vector<int> sol;
	while ((int) burned.size() != n) {
		for (auto &x : burned) {
			for (auto &viz : vertice[x]->vizinhos)
				if (!viz->queimado)
					quase.insert(viz->id);
		}
		int cara = escolhe(safe.empty() ? quase : safe, ALPHA, rng, criterio_guloso);
		if (cara == -1)
			return make_pair(sol, false);
		sol.push_back(cara);
		quase.insert(cara);
		for (auto &x : quase) {
			for (auto &viz : vertice[x]->vizinhos) {
				safe.erase(viz->id);
			}
			update_greedy_function(vertice[x], criterio_guloso);
			vertice[x]->queimado = true;
			burned.insert(x);
			safe.erase(x);
		}
		quase.clear();
	}
	return make_pair(sol, true);
}

void printa_bs(int f_sol, vector<int> &sol) {
	printf("Burning Number: %d\n", f_sol);
	printf("Burning Sequence: [");
	for (int i = 0; i < f_sol; i++) {
		if (i) printf(", ");
		printf("%d", sol[i]);
	}
	printf("]\n");
}

tuple<int, int, double, vector<int>> GRASP(int maximo_iteracoes, double ALPHA, mt19937 &rng, int parada, int criterio_guloso) {
	int best_f = (int) 1e9;
	int iteracao = -1;
	int qtd_iteracoes = 0;
	double tempo = 0;
	vector<int> best_sol;
	// fazer algo com o criterio de parada
	for (int i = 0; i < maximo_iteracoes; i++, qtd_iteracoes++) {
		auto ret = greedy(ALPHA, rng, criterio_guloso);
		if (!ret.second) continue;
		vector<int> sol = ret.first;
		// assert(valid(sol));
		// if (!valid(sol))
		// 	ajeita(sol);
		// sol = busca_local(sol);
		int f_sol = (int) sol.size();
		if (f_sol < best_f) {
			tempo = 1.0 * (clock() - timer) / CLOCKS_PER_SEC;
			iteracao = i + 1;
			best_f = f_sol;
			best_sol = sol;
		}
		printf("======================= Iteracao %d ==========================\n", i + 1);
		printa_bs(f_sol, sol);
	}
	return make_tuple(qtd_iteracoes, iteracao, tempo, best_sol);
	// Talvez retornar a sequência obtida
}

void readInput(mt19937 &rng) {
	set<int> st;
	cin >> n >> m;
	adj_matrix.resize(n, vector<int>(n));
	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		if (!st.count(a)) {
			vertice[a] = new Node(a);
			st.insert(a);
		}
		if (!st.count(b)) {
			vertice[b] = new Node(b);
			st.insert(b);
		}
		vertice[a]->addEdge(vertice[b]);
		vertice[b]->addEdge(vertice[a]);
		adj_matrix[a][b] = adj_matrix[b][a] = 1;
		grau[a]++;
		grau[b]++;
	}
	for (int i = 0; i < n; i++)
		adj_matrix[i][i] = 1;
	assert(*st.begin() >= 0 && *st.rbegin() <= n - 1);
	auto ec_ans = EigenvectorCentrality::get_scores(adj_matrix, 1e-9, rng);
	auto centrality_scores = get<0>(ec_ans);
	eigenvalue = get<1>(ec_ans);
	for (int i = 0; i < n; i++) {
		vertice[i]->eigenvector_centrality = centrality_scores[i];
	}
}

int main(int argc, char **argv) {
	freopen(argv[1], "r", stdin);	// argv[1] = Arquivo de entrada
	int seed = atoi(argv[2]);				// argv[2] = Seed
	double ALPHA = atof(argv[3]);			// argv[3] = Valor de ALPHA
	int maximo_iteracoes = atoi(argv[4]);	// argv[4] = Numero maximo de iteracoes permitida
	int condicao_parada = atoi(argv[5]);	// argv[5] = Condicao de parada utilizada (nao usado no momento)
	int criterio_guloso = atoi(argv[6]);	// argv[6] = Funcao gulosa utilizada
	mt19937 rng(seed);
	readInput(rng);
	double densidade = 2.0 * m / (n * (n - 1));
	int numero_de_vertices = n;
	int numero_de_arestas = m;
	int grau_maximo = 0;
	double grau_medio = 0;
	for (int i = 0; i < n; i++) {
		grau_maximo = max(grau_maximo, vertice[i]->grau);
		grau_medio += (double) vertice[i]->grau;
	}
	grau_medio /= n;
	timer = clock();
	auto v = GRASP(maximo_iteracoes, ALPHA, rng, condicao_parada, criterio_guloso);
	double tempo_total = 1.0 * (clock() - timer) / CLOCKS_PER_SEC;
	int numero_de_iteracoes = get<0>(v);
	int iteracoes_solucao = get<1>(v);
	double tempo_solucao = get<2>(v);
	vector<int> burning_sequence = get<3>(v);
	int burning_number = (int) burning_sequence.size();
	printf("=====================================================================\n");
	printf("Informacoes da instancia %s\n", argv[1]);
	printf("Densidade: %.2lf\n", densidade);
	printf("Numero de vertices: %d\n", numero_de_vertices);
	printf("Numero de arestas: %d\n", numero_de_arestas);
	printf("Grau maximo: %d\n", grau_maximo);
	printf("Grau medio: %.2lf\n", grau_medio);
	printf("Numero de iteracoes realizadas: %d\n", numero_de_iteracoes);
	printf("Tempo gasto considerando todas as iteracoes: %.2lfs\n", tempo_total);
	printf("Iteracao em que a melhor solucao foi obtida: %d\n", iteracoes_solucao);
	printf("Tempo gasto ate a melhor solucao: %.2lfs\n", tempo_solucao);
	printa_bs(burning_number, burning_sequence);
	printf("=====================================================================\n");
	printf("PARAMETROS UTILIZADOS\n");
	printf("Seed: %d\nALPHA: %.2lf\nMax Iteracoes %d\nCondicao de Parada: %d\nFuncao Gulosa %d\n", seed, ALPHA, maximo_iteracoes, condicao_parada, criterio_guloso);
	/*
	Printar: (Seja G o grafo utilizado)
		Densidade de G
		Numero de vertices de G
		Numero de arestas de G
		Grau maximo dentre os vertices de G
		Grau medio dentre os vertices de G
		Numero de iteracoes realizadas
		Tempo total de execucao considerando todas as iteracoes
		Iteracao em que a melhor solucao foi obtida
		Tempo de execucao ate a iteracao em que a melhor solucao foi encontrada
		-> Para cada iteracao:
			- Burning Number
			- Burning Sequence
	*/
	return 0;
}