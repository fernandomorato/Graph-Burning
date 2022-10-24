#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

vector<long double> get_scores(vector<long double> centrality_scores, vector<vector<int>> &adj_matrix, int maximo_iteracoes) {
	int n = (int) centrality_scores.size();
	long double sum = 0;
	for (auto x : centrality_scores) 
		sum += x * x;
	sum = sqrt(sum);
	for (auto &x : centrality_scores) {
		x /= sum;
	}
	for (int k = 0; k < maximo_iteracoes; k++) {
		cout << "Inicio [";
		for (int i = 0; i < n; i++) {
			if (i) cout << ", ";
			cout << centrality_scores[i];
		}
		cout << "]\n";
		vector<long double> temp(n);
		for (int i = 0; i < n; i++) {
			temp[i] = 0;
			for (int j = 0; j < n; j++) {
				temp[i] += (long double) adj_matrix[i][j] * centrality_scores[j];
			}
		}
		long double norm = 0;
		for (int i = 0; i < n; i++) {
			norm += temp[i] * temp[i];
		}
		cout << "norm = " << norm << " -> ";
		norm = sqrt(norm);
		cout << norm << '\n';
		cout << "Final [";
		for (int i = 0; i < n; i++) {
			if (i) cout << ", ";
			centrality_scores[i] = temp[i] / norm;
			cout << centrality_scores[i];
		}
		cout << "]\n";
	}
	return centrality_scores;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	int n, m;
	cin >> n >> m;
	vector<vector<int>> adj_matrix(n, vector<int>(n, 0));
	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		adj_matrix[a][b] = adj_matrix[b][a] = 1;
	}
	for (int i = 0; i < n; i++) {
		adj_matrix[i][i] = 1;
	}
	mt19937 rng(42);
	vector<long double> x(n);
	uniform_real_distribution<long double> unif(0, 1);
	for (int i = 0; i < n; i++) {
		// for (int j = 0; j < n; j++) {
		// 	x[i] += (long double) adj_matrix[i][j];
		// }
		x[i] = unif(rng);
	}
	x = get_scores(x, adj_matrix, 100);
	// auto xd = get_scores(adj_matrix, 1e-9, rng);
	// auto x = get<0>(xd);
	// auto vx = get<1>(xd);
	// int niter = get<2>(xd);
	// cout << vx << ' ' << niter << '\n';
	cout << "[";
	for (int i = 0; i < n; i++) {
		if (i) cout << ", ";
		cout << x[i];
	}
	cout << "]\n";
	return 0;
}