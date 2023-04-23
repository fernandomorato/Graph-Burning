#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	int a, b;
	set<pair<int, int>> st;
	while (cin >> a >> b) {
		a--, b--;
		if (a > b) swap(a, b);
		st.insert(pair<int, int>(a, b));
	}
	for (auto p : st)
		cout << p.first << ' ' << p.second << '\n';
	return 0;
}