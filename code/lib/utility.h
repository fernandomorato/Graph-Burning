#include <bits/stdc++.h>

using namespace std;

template <typename T>
T standart_deviation(vector<T> &values) {
	int sz = (int) values.size();
	if (sz == 1)
		return T(0);
	T mean = accumulate(values.begin(), values.end(), 0.0) / sz;
	auto deviation = [&](T accumulator, const T &val) {
		return accumulator + (val - mean) * (val - mean);
	};
	return sqrt(accumulate(values.begin(), values.end(), 0.0, deviation) / (sz - 1));
}

template <typename T>
T standart_deviation(vector<T> &values, vector<int> indices) {
	vector<int> v((int) indices.size());
	for (int i = 0; i < (int) indices.size(); i++) {
		v[i] = values[indices[i]];
	}
	return standart_deviation(v);
}

// double graph_density(int n, int m) {
// 	return ;
// }