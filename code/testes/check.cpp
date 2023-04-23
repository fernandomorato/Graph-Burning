#include <bits/stdc++.h>

using namespace std;

const int INF = 1e9 + 5;

void bfs(vector<vector<int>> &adj, vector<int> &solution, vector<int> &rotulos, int qtd) {
	int n = (int) adj.size();
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
	}
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
	// vector<int> solution = {42, 86, 39, 6, 90, 0};
	// vector<int> solution = {42, 94, 38, 5, 79, 1, 88};
	// vector<int> solution = {83, 6, 69, 10, 89, 77, 98};
	// vector<int> solution = {67, 21, 81, 6, 19, 94};
	vector<int> solution = {76, 21, 28, 80, 5, 92};
	bfs(adj, solution, rotulos, (int) solution.size());
}