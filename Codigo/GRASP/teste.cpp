#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

vector<string> split(string s, char ch) {
	string cur = "";
	vector<string> retorno;
	for (int i = 0; i < (int) s.size(); i++) {
		if (s[i] == ch) {
			retorno.push_back(cur);
			cur = "";
		} else {
			cur += s[i];
		}
	}
	assert(!cur.empty());
	retorno.push_back(cur);
	return retorno;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	string s = "1000110";
	string linha;
	while (getline(cin, linha)) {
		int ptr = 0;
		vector<string> parsed = split(linha, ',');
		string word;
		vector<string> result;
		for (int i = 0; i < (int) parsed.size(); i++) {
			if (s[i] == '1')
				result.push_back(parsed[i]);
		}
		for (int i = 0; i < (int) result.size(); i++) {
			if (i) cout << ',';
			cout << result[i];
		}
		cout << '\n';
	}
	return 0;
}