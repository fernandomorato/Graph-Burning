#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

vector<pair<int, vector<int>>> solutions; // {valor, lista}

const int MAXN = 2e5 + 5;
clock_t timer;

// OUTPUT
int densidade, numero_de_vertices, numero_de_arestas, grau_maximo, numero_de_iteracoes, iteracoes_solucao;
double grau_medio, tempo_total, tempo_solucao;

struct Node {
	vector<Node*> vizinhos;
	int id;
	int grau;
	bool queimado;

	Node() {}

	Node(int _id) {
		id = _id;
		grau = 0;
		queimado = false;
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

inline int g(int v) { return vertice[v]->grau; }

int escolhe(set<int> &st) {
	if (st.empty())
		return -1;
	int mx = 0;
	int mn = (int) 1e9;
	for (auto &x : st) {
		assert(!vertice[x]->queimado);
		mx = max(mx, g(x));
		mn = min(mn, g(x));
	}
	int lowerBound = mx - (int) floor(ALFA * (mx - mn));
	vector<int> v;
	for (auto &x : st) {
		if (g(x) >= lowerBound)
			v.push_back(x);
	}
	int cara = v[rng() % ((int) v.size())];
	st.erase(cara);
	return cara;
}

pair<vector<int>, bool> greedy(double ALPHA = 0.5) {
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
		int cara = escolhe(safe.empty() ? quase : safe);
		if (cara == -1)
			return make_pair(sol, false);
		sol.push_back(cara);
		quase.insert(cara);
		for (auto &x : quase) {
			for (auto &viz : vertice[x]->vizinhos) {
				safe.erase(viz->id);
				viz->grau--;
			}
			vertice[x]->queimado = true;
			burned.insert(x);
			safe.erase(x);
		}
		// cerr << "quase = " << (int) quase.size() << '\n';
		// cerr << "burned = " << (int) burned.size() << '\n';
		// cerr << *burned.begin() << '\n';
		quase.clear();
		// printa(burned);
	}
	return make_pair(sol, true);
}

tuple<int, double, vector<int>> GRASP(int maximo_iteracoes, double ALPHA) {
	int best_f = (int) 1e9;
	int iteracao = -1;
	double tempo = 0;
	vector<int> best_sol;
	for (int i = 0; i < maximo_iteracoes; i++) {
		auto ret = greedy(ALPHA);
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
	}
	return make_tuple(iteracao, tempo, best_sol);
	// Talvez retornar a sequência obtida
}

void readInput() {
	set<int> st;
	cin >> n >> m;
	densidade = 2.0 * m / (n * (n - 1));
	numero_de_vertices = n;
	numero_de_arestas = m;
	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		a--, b--;
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
		grau[a]++;
		grau[b]++;
		grau_maximo = max({grau_maximo, grau[a], grau[b]});
	}
	assert(*st.begin() >= 0 && *st.rbegin() <= n - 1);
}

int main(int argc, char **argv) {
	freopen(argv[1].c_str(), "r", stdin);	// argv[1] = Arquivo de entrada
	int seed = atoi(argv[2]);				// argv[2] = Seed
	double ALPHA = atof(argv[3]);			// argv[3] = Valor de ALPHA
	int maximo_iteracoes = atoi(argv[4]);	// argv[4] = Numero maximo de iteracoes permitida
	int condicao_parada = atoi(argv[5]);	// argv[5] = Condicao de parada utilizada (nao usado no momento)
	int criterio_guloso = atoi(argv[6]);	// argv[6] = Funcao gulosa utilizada

	readInput();

	for (int i = 0; i < n; i++) {
		grau_medio += vertice[i]->grau;
	}
	grau_medio /= n;

	timer = clock();
	auto v = GRASP(maximo_iteracoes, ALPHA);
	int iteracao = get<0>(v);
	double tempoParaSolucao = get<1>(v);
	int bn = (int) get<2>(v).size();
	printf("%s,%s,%s,%d,%d,%.5lf,%.5lf", argv[1], argv[2], argv[3], bn, iteracao, tempoParaSolucao, 1.0 * (clock() - timer) / CLOCKS_PER_SEC);
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