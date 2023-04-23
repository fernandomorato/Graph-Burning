#include <bits/stdc++.h>

using namespace std;

template<typename T>
T standart_deviation(vector<T> &values) {
	int sz = (int) values.size();
	T mean = accumulate(values.begin(), values.end(), 0.0) / sz;
	auto deviation = [&](T accumulator, const T &val) {
		return accumulator + (val - mean) * (val - mean);
	};
	return sqrt(accumulate(values.begin(), values.end(), 0.0, deviation) / (sz - 1));
}

// double graph_density(int n, int m) {
// 	return ;
// }