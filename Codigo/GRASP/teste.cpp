#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

// teste para a queima de um P9

vector<int> adj[105];
bool mark[105];

pair<vector<int>, int> burn(vector<int> bs, const int MAXN) {
	cout << "Trying: [";
	for (int i = 0; i < (int) bs.size(); i++) {
		if (i) cout << ", ";
		cout << bs[i];
	}
	cout << "] = ";
	vector<int> burned;
	for (int i = 0; i < (int) bs.size(); i++) {
		vector<int> add;
		for (auto x : burned) {
			for (auto v : adj[x]) {
				if (!mark[v]) {
					add.push_back(v);
				}
			}
		}
		if (mark[bs[i]]) {
			cout << "infinity\n";
			return {{}, (int) 1e9};
		}
		add.push_back(bs[i]);
		mark[bs[i]] = i + 1;
		for (auto x : add) {
			mark[x] = i + 1;
			burned.push_back(x);
		}
	}
	int cnt_burned = 0;
	for (int i = 1; i <= MAXN; i++) {
		cnt_burned += (mark[i] ? 1 : 0);
	}
	if (cnt_burned == MAXN) {
		cout << (int) bs.size() << "\n";
		return {bs, (int) bs.size()};
	}
	cout << -1 << '\n';
	return {{}, (int) 1e9};
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie();
	int MAXN;
	cin >> MAXN;
	clock_t z = clock();
	for (int i = 1; i <= MAXN - 1; i++) {
		adj[i].push_back(i + 1);
		adj[i + 1].push_back(i);
	}
	adj[1].push_back(MAXN);
	adj[MAXN].push_back(1);
	int ans = (int) 1e9;
	vector<int> bs;
	vector<int> caras;
	for (int i = 0; i <= (1 << MAXN) - 1; i++) {
		caras.push_back(i);
	}
	sort(caras.begin(), caras.end(), [&](int a, int b) {
		return __builtin_popcount(a) < __builtin_popcount(b);
	});
	for (int i = 0; i <= (1 << MAXN) - 1; i++) {
		// if (__builtin_popcount(i)) {
			vector<int> curs;
			for (int j = 0; j < MAXN; j++) {
				mark[j + 1] = 0;
				if (caras[i] & (1 << j))
					curs.push_back(j + 1);
			}
			do {
				auto eita = burn(curs, MAXN);
				if (eita.second < ans) {
					ans = eita.second;
					bs = eita.first;
				}
			} while (next_permutation(curs.begin(), curs.end()));
			if (ans != (int) 1e9)
				break;
		// }
	}
	cout << "The burning sequence for C" << MAXN << " is " << ans << '\n';
	cout << "bs = [";
	for (int i = 0; i < (int) bs.size(); i++) {
		if (i) cout << ' ';
		cout << bs[i];
	}
	cout << "]\n";
	cout << "Time elapsed: " << 1.0 * (clock() - z) / CLOCKS_PER_SEC << "s.\n";
	return 0;
}