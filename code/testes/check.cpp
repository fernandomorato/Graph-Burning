#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9 + 5;

bool check_solution(int n_vertices, vector<vector<int>> &adj, vector<int> &burning_sequence) {
	vector<int> burned(n_vertices); // maybe use a bitset for efficiency
	vector<int> last_burned;
	int cnt = 0;
	for (auto x : burning_sequence) {
		cnt++;
		if (burned[x])
			return false;
		vector<int> to_burn_now;
		to_burn_now.push_back(x);
		for (int cur_vertex : last_burned) {
			for (int neighbour : adj[cur_vertex]) {
				if (!burned[neighbour]) {
					to_burn_now.push_back(neighbour);
				}
			}
		}
		for (auto v : to_burn_now) {
			burned[v] = 1;
		}
		swap(to_burn_now, last_burned);
	}
	for (int i = 0; i < n_vertices; i++) {
		if (burned[i] == 0)
			cout << i << " was not burned!\n";
	}
	return *min_element(burned.begin(), burned.end()) == 1; // if min is 0, than it didn't burn the whole graph
}

void bfs(vector<vector<int>> &adj, vector<int> &solution, vector<int> &rotulos, int qtd) {
	int n = (int) adj.size();
	vector<int> aux = rotulos;
	for (int i = 0; i < qtd; i++) {
		queue<int> q;
		auto rotulo = rotulos;
		vector<int> pai(n, INF);
		for (int j = 0; j <= i; j++) {
			pai[solution[j]] = j + 1;
			rotulo[solution[j]] = j + 1;
			q.push(solution[j]);
		}
		cout << "RODADA " << i + 1 << '\n'; 
		while (!q.empty()) {
			int cur = q.front();
			q.pop();
			if (rotulo[cur] >= (int) solution.size())
				continue;
			for (int x : adj[cur]) {
				if (rotulo[x] > rotulo[cur] + 1) {
					rotulo[x] = rotulo[cur] + 1;
					pai[x] = pai[cur];
					q.push(x);
				}
			}
		}
		for (int j = 0; j < (int) pai.size(); j++) {
			if (pai[j] == i + 1)
				cout << pai[j] << " -> " << j << '\n';
		}
		swap(aux, rotulo);
	}
	swap(rotulos, aux);
}

int main() {
	int n, m;
	cin >> n >> m;
	vector<vector<int>> adj(n, vector<int>());
	vector<int> rotulos(n, INF);
	for (int i = 0; i < m; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	// vector<int> solution = {4, 27, 48, 106, 129, 156, 164};
	// vector<int> solution = {4, 0, 3};
	vector<int> solution = {48, 106, 164, 27, 129, 4, 156};
	if (!check_solution(n, adj, solution)) {
		cout << "INVALID SOLUTION\n";
		exit(0);
	}
	bfs(adj, solution, rotulos, (int) solution.size());
	cout << *min_element(rotulos.begin(), rotulos.end()) << ' ' << *max_element(rotulos.begin(), rotulos.end()) << '\n';
}