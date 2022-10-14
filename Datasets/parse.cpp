#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());



int to_int(string s) {
	int x = 0;
	for (int i = 0; i < (int) s.size(); i++) {
		x = x * 10 + (s[i] - '0');
	}
	return x;
}

bool isNumeric(string s) {
	for (auto c : s)
		if (c < '0' || c > '9')
			return false;
	return true;
}

vector<string> split(string s, char ch) {
	vector<string> result;
	string aux = "";
	for (int i = 0; i < (int) s.size(); i++) {
		if (s[i] == ch) {
			result.push_back(aux);
			aux = "";
		} else {
			aux += s[i];
		}
	}
	if (!aux.empty())
		result.push_back(aux);
	return result;
}

void csv() {
	string fname;
	cin >> fname;
	fstream file("datasets/" + fname + ".csv", ios::in);
	if (file.is_open()) {
		int a, b;
		vector<int> nodes;
		set<pair<int, int>> edges;
		string line;
		while (getline(file, line)) {
			vector<string> linha = split(line, ',');
			if (!isNumeric(linha[0]))
				continue;
			a = to_int(linha[0]);
			b = to_int(linha[1]);
			nodes.push_back(a);
			nodes.push_back(b);
			if (a > b)
				swap(a, b);
			edges.insert(pair<int, int>(a, b));
			// cout << "aresta entre " << a << " e " << b << '\n';
		}
		sort(nodes.begin(), nodes.end());
		nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
		cout << (int) nodes.size() << ' ' << (int) edges.size() << '\n';
		for (auto e : edges) {
			cout << e.first << ' ' << e.second << '\n';
		}
	} else {
		cout << "Could not open file " << fname << "\n";;
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie();
	// csv();
	// return 0;
	string lixo;
	getline(cin, lixo);
	int a, b, m;
	cin >> a >> b >> m;
	vector<int> nodes;
	set<pair<int, int>> edges;
	for (int i = 0; i < m; i++) {
		double p;
		cin >> a >> b;
		// a--, b--;
		if (a == b) continue;
		nodes.push_back(a);
		nodes.push_back(b);
		if (a > b)
			swap(a, b);
		edges.insert(pair<int, int>(a, b));
	}
	sort(nodes.begin(), nodes.end());
	nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
	cout << (int) nodes.size() << ' ' << (int) edges.size() << '\n';
	for (auto e : edges) {
		cout << e.first << ' ' << e.second << '\n';
	}
	return 0;
}