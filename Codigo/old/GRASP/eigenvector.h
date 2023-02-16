#include <bits/stdc++.h>

using namespace std;

tuple<vector<long double>, long double, int> get_scores(vector<vector<int>> &adj_matrix, long double EPS, mt19937 &rng) {
	int n = (int) adj_matrix.size();
	vector<long double> x0(n), x1(n);
	uniform_real_distribution<long double> unif(0, 1);
	long double norm = 0;
	for (int i = 0; i < n; i++) {
		x1[i] = unif(rng);
		norm += x1[i] * x1[i];
	}
	norm = sqrt(norm);
	cout << '\n';
	for (int i = 0; i < n; i++) {
		x1[i] /= norm;
	}
	int iter = 0;
	long double lambda = 0, lambdaOld = 0, m = 0;
	while (iter <= 100) {
		swap(x0, x1);
		for (int i = 0; i < n; i++) {
			x1[i] = 0;
			for (int j = 0; j < n; j++) {
				x1[i] += (long double) adj_matrix[i][j] * x0[j];
			}
		}
		norm = 0;
		for (int i = 0; i < n; i++) {
			norm += x1[i] * x1[i];
		}
		norm = sqrt(norm);
		lambda = 0;
		for (int i = 0; i < n; i++) {
			x1[i] /= norm;
			lambda += x1[i] * x0[i];
		}
		lambda = sqrt(lambda);
		iter++;
		if (fabs((lambda - lambdaOld) / lambda) < EPS)
			break;
		lambdaOld = lambda;
	}
	return make_tuple(x1, lambda, iter);
}