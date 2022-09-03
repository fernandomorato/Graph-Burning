#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

int main(int argc, char **argv) {
	ios::sync_with_stdio(false);
	cin.tie();
	freopen("all-data.txt", "a", stdout);
	string line;
	while (getline(cin, line)) {
		cout << line << '\n';
	}
	return 0;
}