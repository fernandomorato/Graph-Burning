#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

const int MAXN = 2e5 + 5;

int p[MAXN], sz[MAXN];

int range(int l, int r) {
	return l + rng() % (r - l + 1);
}

int find(int v) { return v == p[v] ? v : p[v] = find(p[v]); }

bool join(int a, int b) {
	a = find(a);
	b = find(b);
	if (a == b)
		return false;
	if (sz[a] > sz[b])
		swap(a, b);
	p[a] = b;
	sz[b] += sz[a];
	return true;
}

int myrandom(int i) { return rng() % i; }

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	int n = range(2, 20);
	int m = n - 1;
	for (int i = 1; i <= n; i++) {
		p[i] = i;
		sz[i] = 1;
	}
	cout << n << ' ' << m << '\n';
	vector<pair<int, int>> edges;
	for (int i = 1; i <= n; i++) {
		for (int j = i + 1; j <= n; j++) {
			edges.emplace_back(i, j);
		}
	}
	random_shuffle(edges.begin(), edges.end(), myrandom);
	int cnt = 0;
	for (auto x : edges) {
		if (join(x.first, x.second)) {
			cout << x.first - 1 << ' ' << x.second - 1 << '\n';
			cnt++;
		}
	}
	assert(cnt == n - 1);
	return 0;
}