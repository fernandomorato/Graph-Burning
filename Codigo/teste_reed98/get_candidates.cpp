#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 1e5 + 5;

vector<int> adj[MAXN];
int n, m;

bool queimado[MAXN];

bool queima(vector<int> &sequence)  {
	set<int> safe; // vertices que estao safe
	set<int> quase; // vertices que estao quase queimados
	set<int> burned; // vertices que estao queimados
	for (int x : sequence) {
		for (auto &xd : burned) {
			for (auto &viz : adj[xd])
				if (queimado[viz])
					quase.insert(viz);
		}
		int cara = x;
		if (burned.count(x)) {
			return false;
		}
		quase.insert(cara);
		for (auto &xd : quase) {
			for (auto &viz : adj[xd]) {
				safe.erase(viz);
			}
			queimado[xd] = true;
			burned.insert(xd);
			safe.erase(xd);
		}
		quase.clear();
	}
	return (int) burned.size() == n;
}

int try_permutations(vector<int> &p, vector<int> &bs) {
	do {
		// checa se a burning sequence definida por p da bom
		if (queima(p)) {
			swap(bs, p);
			return true;
		}
		for (int i = 0; i < n; i++) {
			queimado[i] = false;
		}
	} while (next_permutation(p.begin(), p.end()));
	return false;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	// Assuma que a entrada são os caras mais frequentes
	cin >> n >> m;
	for (int i = 0; i < m; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	int t; cin >> t;
	vector<int> v(t);
	for (int i = 0; i < t; i++) {
		cin >> v[i];
	}
	sort(v.begin(), v.end());
	bool flag = true;
	vector<int> bs;
	for (int i = 0; i < t && flag; i++) {
		for (int j = i + 1; j < t && flag; j++) {
			for (int k = j + 1; k < t && flag; k++) {
				vector<int> p = {v[i], v[j], v[k]};
				cerr << "Tentando " << v[i] << ' ' << v[j] << ' ' << v[k] << '\n';
				if (try_permutations(p, bs)) {
					flag = false;
				}
			}
		}
	}
	if (bs.empty()) {
		cout << "Deu muito ruim\n";
	}
	cout << (int) bs.size() << '\n';
	for (auto x : bs) {
		cout << x << ' ';
	}
	cout << '\n';
	return 0;
}