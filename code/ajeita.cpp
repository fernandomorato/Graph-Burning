#include <bits/stdc++.h>

using namespace std;

string get(string s) {
	string resu = "";
	for (int i = 0; i < (int) s.size(); i++) {
		if (s[i] == ',')
			break;
		resu += s[i];
	}
	return resu;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	vector<string> vs;
	string s;
	while (cin >> s) {
		vs.push_back(s);
	}
	sort(vs.begin(), vs.end(), [&](auto a, auto b) {
		return get(a) < get(b);
	});
	for (auto ss : vs)
		cout << ss << '\n';
	return 0;
}