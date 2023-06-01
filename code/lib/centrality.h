// #include "standart_algorithms.h"
#include <bits/stdc++.h>

using namespace std;

enum Centrality {EIGENVECTOR, BETWEENNESS};

void eigenvector_centrality(vector<double> &centrality, vector<int> &v, vector<pair<int, int>> &e, mt19937 &rng) {
	if ((int) v.size() == 1) {
		centrality[v[0]] = 1;
		return;
	}
	double EPS = 1e-9;
	int tamanho = (int) centrality.size();
	vector<double> x0(tamanho), x1(tamanho);
	uniform_real_distribution<double> unif(0, 1);
	double norm = 0;
	for (int i = 0; i < (int) v.size(); i++) {
		x1[v[i]] = unif(rng);
		norm += x1[v[i]] * x1[v[i]];
	}
	norm = sqrt(norm);
	for (int i = 0; i < (int) v.size(); i++) {
		x1[v[i]] /= norm;
	}
	int iter = 0;
	double lambda = 0, lambdaOld = 0, m = 0;
	while (iter <= 1000) {
		swap(x0, x1);
		x1.clear();
		for (auto &[a, b] : e) {
			x1[a] += x0[b];
			x1[b] += x0[a];
		}
		norm = 0;
		for (int i = 0; i < (int) v.size(); i++) {
			norm += x1[v[i]] * x1[v[i]];
		}
		norm = sqrt(norm);
		lambda = 0;
		for (int i = 0; i < (int) v.size(); i++) {
			x1[v[i]] /= norm;
			lambda += x1[v[i]] * x0[v[i]];
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

void betweenness_centrality(vector<double> &centrality, vector<int> &V, vector<pair<int, int>> &E) {
	map<int, vector<int>> adj; // lista de adjacencia local
	for (auto e : E) {
		adj[e.first].push_back(e.second);
		adj[e.second].push_back(e.first);
	}
	// A Faster Algorithm for Betweenness Centrality. Ulrik Brandes,
    // Journal of Mathematical Sociology 25(2):163-177, 2001.
    // https://doi.org/10.1080/0022250X.2001.9990249
	int n = (int) centrality.size();
	for (int s : V) {
		vector<int> S; // empty stack
		vector<vector<int>> P(n);
		vector<int> sigma(n, 0); sigma[s] = 1;
		vector<int>  d(n, -1); d[s] = 0;
		queue<int> Q;
		Q.push(s);
		while (!Q.empty()) {
			int v = Q.front();
			Q.pop();
			S.push_back(v);
			for (int w : adj[v]) {
				if (d[w] < 0) {
					Q.push(w);
					d[w] = d[v] + 1;
				}
				if (d[w] == d[v] + 1) {
					sigma[w] += sigma[v];
					P[w].push_back(v);
				}
			}
		}
		vector<double> del(n, 0.0);
		while (!S.empty()) {
			int w = S.back();
			S.pop_back();
			for (int v : P[w]) {
				del[v] = del[v] + 1.0  * sigma[v] / sigma[w] * (1 + del[w]);
				if (w != s)
					centrality[w] += del[w];
			}
		}
	}
}

void calculate_centrality(vector<double> &centrality, vector<int> &v, vector<pair<int, int>> &e, mt19937 &rng, int centralidade) { // aprender a usar o enum
	// switch (c) {
	// 	case EIGENVECTOR:
	// 		eigenvector_centrality(centrality, v, e, rng);
	// 	case BETWEENNESS:
	// 		betweenness_centrality(centrality, v, e, rng);
	// 	default:
	// 		assert(false);
	// }
	if (centralidade == 1)
		eigenvector_centrality(centrality, v, e, rng);
	else {
		assert(centralidade == 2);
		betweenness_centrality(centrality, v, e);
	}
}