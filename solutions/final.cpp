#include <bits/stdc++.h>

using namespace std;

bool xqdl(string str, string a) {
	if ((int) str.size() < (int) a.size())
		return false;
	for (int i = 0; i < (int) a.size(); i++) {
		if (str[i] != a[i])
			return false;
	}
	return true;
}

string get(string s) {
	string resu = "";
	for (int i = 0; i < (int) s.size(); i++) {
		if (s[i] == ' ') {
			resu = "";
			continue;
		}
		resu += s[i];
	}
	return resu;
}

vector<int> get_sol(string s) {
	vector<int> sol;
	string value = "";
	bool flag = false;
	for (int i = 0; i < (int) s.size(); i++) {
		if (s[i] == '=') {
			flag = true;
			continue;
		}
		if (!flag) continue;
		if ('0' <= s[i] && s[i] <= '9') {
			value += s[i];
		} else if (!value.empty()) {
			sol.push_back(stoi(value));
			value = "";
		}
	}
	if (!value.empty()) sol.push_back(stoi(value));
	return sol;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	string line;
	string output_name = "";
	int solution_value = 0;
	vector<int> solution;
	while (getline(cin, line)) {
		if (xqdl(line, "Instance")) {
			cerr << line << ' ' << get(line) << ' ';
			output_name = get(line);
			FILE *a = fopen(("gr/" + output_name + ".sol").c_str(), "r");
			FILE *b = fopen(("gr_bias-04/" + output_name + ".sol").c_str(), "r");
			FILE *c = fopen(("gr_bias-07/" + output_name + ".sol").c_str(), "r");
			FILE *d = fopen(("gr_reactive/" + output_name + ".sol").c_str(), "r");
			FILE *e = fopen(("gr_reactive_bias/" + output_name + ".sol").c_str(), "r");
			int v[5];
			fscanf(a, "%d", &v[0]);
			fscanf(b, "%d", &v[1]);
			fscanf(c, "%d", &v[2]);
			fscanf(d, "%d", &v[3]);
			fscanf(e, "%d", &v[4]);
			FILE *result;
			int xd = 1e9;
			for (int i = 0; i < 5; i++) {
				cerr << v[i] << ' ';
				xd = min(xd, v[i]);
			}
			cerr << '\n';
			int l1 = 0;
			if (v[0] == xd) {
				result = a;
				l1 = v[0];
			} else if (v[1] == xd) {
				result = b;
				l1 = v[1];
			} else if (v[2] == xd) {
			 	result = c;
			 	l1 = v[2];
			} else if (v[3] == xd) {
				result = d;
				l1 = v[3];
			} else if (v[4] == xd) {
				result = e;
				l1 = v[4];
			}
			FILE *of = fopen((output_name + ".sol").c_str(), "w");
			fprintf(of, "%d\n", l1);
			for (int i = 0; i < l1; i++) {
				int oo;
				fscanf(result, "%d", &oo);
				fprintf(of, "%d\n", oo);
			}
		}
	}
	return 0;
}