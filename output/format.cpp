#include <bits/stdc++.h>

using namespace std;

// instancia (0)
// qtd iteracoes (2)
// melhor sol (5)
// media sol (4)

vector<string> split(string s, char ch) {
	string cur = "";
	vector<string> answer;
	for (auto x : s) {
		if (x == ch) {
			answer.push_back(cur);
			cur = "";
		} else {
			cur += x;
		}
	}
	answer.push_back(cur);
	return answer;
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	vector<int> options = {0, 5};
	string line;
	while (getline(cin, line)) {
		vector<string> xd = split(line, ',');
		for (int i = 0; i < (int) options.size(); i++) {
			if (i) cout << ',';
			cout << xd[options[i]];
		}
		cout << '\n';
	}
	return 0;
}