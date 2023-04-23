#include <bits/stdc++.h>

using namespace std;

void calc_centrality_scores(vector<double> &centrality, vector<int> &v, vector<pair<int, int>> &e, mt19937 &rng) {
	double EPS = 1e-9;
	int tamanho = (int) centrality.size();
	vector<double> x0(tamanho), x1(tamanho);
	uniform_real_distribution<double> unif(0, 1);
	double norm = 0;
	for (int i = 0; i < tamanho; i++) {
		x1[i] = unif(rng);
		norm += x1[i] * x1[i];
	}
	norm = sqrt(norm);
	for (int i = 0; i < tamanho; i++) {
		x1[i] /= norm;
	}
	int iter = 0;
	double lambda = 0, lambdaOld = 0, m = 0;
	while (iter <= 1000) {
		swap(x0, x1);
		x1.clear();
		for (auto [a, b] : e) {
			x1[a] += x0[b];
			x1[b] += x0[a];
		}
		norm = 0;
		for (int i = 0; i < tamanho; i++) {
			norm += x1[i] * x1[i];
		}
		norm = sqrt(norm);
		lambda = 0;
		for (int i = 0; i < tamanho; i++) {
			x1[i] /= norm;
			lambda += x1[i] * x0[i];
		}
		lambda = sqrt(lambda);
		iter++;
		if (fabs((lambda - lambdaOld) / lambda) < EPS)
			break;
		lambdaOld = lambda;
	}
	for (int i = 0; i < (int) v.size(); i++) {
		centrality[v[i]] = x1[v[i]];
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	string s;
	while (cin >> s) {
		for (auto &c : s) {
			if (c == '.') {
				c = ',';
			} else if (c == ',') {
				c = ';';
			}
		}
		cout << s << '\n';
	}
	return 0;
	int n, m;
	cin >> n >> m;
	vector<int> v(n * n);
	iota(v.begin(), v.end(), 0);
	vector<pair<int, int>> e;
	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		e.emplace_back(a, b);
	}
	vector<double> c(n * n);
	mt19937 rng(42);
	calc_centrality_scores(c, v, e, rng);
	for (int i = 0; i < n * n; i++)
		cout << v[i] << "," << c[i] << '\n';
	return 0;
}