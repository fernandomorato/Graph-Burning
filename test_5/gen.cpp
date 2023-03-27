#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());


int main(int argc, char **argv) {
	int n = atoi(argv[1]);
	int vx[] = {-1, 1, 0, 0};
	int vy[] = {0, 0, -1, 1};
	set<pair<int, int>> st;
	auto get = [&](int x, int y) {
		return n * x + y;
	};
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < 4; k++) {
				int ni = i + vx[k];
				int nj = j + vy[k];
				if (0 <= ni && ni < n && 0 <= nj && nj < n) {
					auto[a, b] = make_pair(get(i, j), get(ni, nj));
					if (a > b) swap(a, b);
					st.insert(pair<int, int>(a, b));
				}
			}
		}
	}
	cout << n * n << ' ' << st.size() << '\n';
	for (auto p : st) {
		cout << p.first << ' ' << p.second << '\n';
	}
	return 0;
}
