#include <bits/stdc++.h>

using namespace std;

string get(string s) {
	string ss = "";
	for (int i = 0; s[i] != ','; i++) {
		ss += s[i];
	}
	return ss;
}

int main() {
	string s;
	vector<string> vs;
	while (cin >> s) {
		vs.push_back(s);
	}
	sort(vs.begin(), vs.end(), [&](auto &a, auto &b) {
		return get(a) < get(b);
	});
	cout << "Instance,Time consumed,Time to solution,Iteration,Freq sol,Solution\n";
	for (auto x : vs)
		cout << x << '\n';
}
