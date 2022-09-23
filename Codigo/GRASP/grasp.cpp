#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

vector<pair<int, vector<int>>> solutions; // {valor, lista}

const int MAXN = 2e5 + 5;
const double ALFA = 0.5;
clock_t timer;

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

inline int f(vector<int> &sol) { return (int) sol.size(); }

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

void printa(set<int> &st) {
	for (auto x : st)
		printf("%d ", x);
	putchar('\n');
}

pair<vector<int>, bool> greedy() {
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

tuple<int, double, vector<int>> GRASP() {
	int best_f = (int) 1e9;
	int iteracao = -1;
	double tempo = 0;
	vector<int> best_sol;
	for (int i = 0; i < 1000; i++) {
		auto ret = greedy();
		if (!ret.second) continue;
		vector<int> sol = ret.first;
		// assert(valid(sol));
		// if (!valid(sol))
		// 	ajeita(sol);
		// sol = busca_local(sol);
		int f_sol = f(sol);
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
		grau[a]++;
		grau[b]++;
	}
	assert(*st.begin() >= 0 && *st.rbegin() <= n - 1);
}

// string converte(char linha[]) {
// 	printf("%d\n", (int) strlen(linha));
// 	int tamanho = strlen(linha);
// 	string s = "";
// 	for (int i = 0; i < tamanho; i++) {
// 		s += linha[i];
// 	}
// 	return s;
// }

int main(int argc, char **argv) {
	// ios::sync_with_stdio(false);
	// cin.tie();
	readInput();
	timer = clock();
	auto v = GRASP();
	// string nomeInstancia(argv[1]);
	auto split = [&](string s) {
		string a = "";
		int i = 0;
		for (; i < (int) s.size() && s[i] != ' '; i++) {
			a += s[i];
		}
		string b = "";
		i++;
		for (; i < (int) s.size(); i++) {
			b += s[i];
		}
		return pair<string, string>(a, b);
	};
	int iteracao = get<0>(v);
	double tempoParaSolucao = get<1>(v);
	int bn = get<2>(v);
	printf("%s,%s,%s,%d,%d,%.5lf,%.5lf", argv[1], argv[2], argv[3], bn, iteracao, tempoParaSolucao, 1.0 * (clock() - timer) / CLOCKS_PER_SEC);
	return 0;
}