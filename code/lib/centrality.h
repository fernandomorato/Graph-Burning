// #include "graph.h"
#include <bits/stdc++.h>

using namespace std;

void eigenvector_centrality(vector<double> &centrality, vector<int> &v, vector<pair<int, int>> &e, mt19937 &rng) {
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
		for (auto &[a, b] : e) {
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

void betweenness_centrality(vector<double> &centrality, vector<int> &v, vector<pair<int, int>> &e, mt19937 &rng) {
	// vector<double> centrality(graph.size());
	// for (Vertex v : graph.getVertices()) {
	// 	stack<Vertex> s;
	// 	vector<vector<Vertex>> p;
	// 	vector<int> o(graph.size(), 0);
	// 	o[v.getId()] = 1;
	// 	vector<int>
	// }
}