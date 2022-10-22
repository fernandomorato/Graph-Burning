#include <bits/stdc++.h>

using namespace std;

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

double f(vector<double> a, vector<double> b) {
	// quero fazer sum(abs(a - b))
	double ans = 0;
	for (int i = 0; i < (int) a.size(); i++) {
		ans += fabs(a[i] - b[i]);
	}
	return ans;
}

tuple<vector<double>, double, int> get_scores(vector<vector<int>> &adj_matrix, double EPS) {
	int n = (int) adj_matrix.size();
	vector<double> x0(n, 0);
	vector<double> x1(n, 1.0 / n);
	int iter = 0;
	double m = 0;
	while (f(x0, x1) > EPS && iter < 2) {
		swap(x0, x1);
		for (int i = 0; i < n; i++) {
			x1[i] = 0;
			for (int j = 0; j < n; j++) {
				x1[i] += (double) adj_matrix[i][j] * x0[j];
			}
		}
		m = 0;
		for (int i = 0; i < n; i++) {
			m = max(m, fabs(x1[i]));
		}
		for (int i = 0; i < n; i++) {
			x1[i] /= m;
		}
		iter++;
	}
	return make_tuple(x1, m, iter);
}

vector<double> get_scores(vector<double> centrality_scores, vector<vector<int>> &adj_matrix, int maximo_iteracoes) {
	int n = (int) centrality_scores.size();
	double sum = 0;
	for (auto x : centrality_scores) 
		sum += x;
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
		vector<double> temp(n);
		for (int i = 0; i < n; i++) {
			temp[i] = 0;
			for (int j = 0; j < n; j++) {
				temp[i] += (double) adj_matrix[i][j] * centrality_scores[j];
			}
		}
		double norm = 0;
		for (int i = 0; i < n; i++) {
			norm += temp[i] * temp[i];
		}
		cout << "norm = " << norm << " -> ";
		// norm /= (double) n;
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
	vector<vector<int>> adj_matrix;
	int n, m;
	cin >> n >> m;
	adj_matrix.resize(n);
	for (int i = 0; i < n; i++) {
		adj_matrix[i].resize(n);
	}
	for (int i = 0; i < m; i++) {
		int a, b;
		cin >> a >> b;
		adj_matrix[a][b] = adj_matrix[b][a] = 1;
	}
	// vector<double> x(n);
	// for (int i = 0; i < n; i++) {
	// 	for (int j = 0; j < n; j++) {
	// 		x[i] += (double) adj_matrix[i][j];
	// 	}
	// }
	// x = get_scores(x, adj_matrix, 100);
	auto xd = get_scores(adj_matrix, 1e-9);
	auto x = get<0>(xd);
	auto vx = get<1>(xd);
	int niter = get<2>(xd);
	cout << "[";
	for (int i = 0; i < n; i++) {
		if (i) cout << ", ";
		cout << x[i];
	}
	cout << "]\n";
	return 0;
}