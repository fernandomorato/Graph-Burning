#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 2e5 + 5;

vector<int> adj[MAXN];
bool vis[MAXN];

void dfs(int v) {
	vis[v] = true;
	for (int u : adj[v]) {
		if (!vis[u])
			dfs(u);
	}
}

int main(int argc, char **argv) {
	ios::sync_with_stdio(false);
	cin.tie(0);
	int n, m;
	cin >> n >> m;
	for (int i = 0; i < m; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	int cnt = 0;
	bool ok = true;
	for (int i = 0; i < n; i++) {
		if (!vis[i]) {
			dfs(i);
			cnt++;
		}
	}
	cout << cnt << '\n';
	return 0;
}