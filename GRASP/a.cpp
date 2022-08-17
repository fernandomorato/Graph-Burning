#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

int main() {
	ios::sync_with_stdio(false);
	cin.tie();
	int tt; cin >> tt;
	while (tt--) {
		int n; cin >> n;
		multiset<int> mtst[2];
		for (int k : {0, 1}) {
			for (int i = 0; i < n; i++) {
				int x;
				cin >> x;
				while (x % 2 == 0)
					x /= 2;
				mtst[k].insert(x);
			}
		}
		bool ok = true;
		while (!mtst[0].empty()) {
			int x = *mtst[1].rbegin();
			auto it = mtst[0].find(x);
			if (it != mtst[0].end()) {
				mtst[0].erase(it);
				mtst[1].erase(mtst[1].find(x));
			} else if (x == 1) {
				ok = false;
				break;
			} else {
				mtst[1].erase(mtst[1].find(x));
				mtst[1].insert(x / 2);
			}
		}
		cout << (ok ? "yes\n" : "no\n");
	}
	return 0;
}