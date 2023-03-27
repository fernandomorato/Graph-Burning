#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	vector<string> v;
	string s;
	while (cin >> s) {
		v.push_back(s);
	}
	auto get = [&](string ss) {
		int num = 0;
		for (int i = 4; ss[i] != ','; i++) {
			num = num * 10 + int(ss[i] - '0');
		}
		return num;
	};
	sort(v.begin(), v.end(), [&](auto &a, auto &b) {
		return get(a) < get(b);
	});
	cout << "Instance,Time consumed,Time to sol,Iteration,Freq sol,Solution\n";
	for (auto x : v)
		cout << x << '\n';
	return 0;
}