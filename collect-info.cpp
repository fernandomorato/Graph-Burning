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
			cerr << line << ' ' << get(line) << '\n';
			output_name = get(line);
		}
		if (xqdl(line, "Solution value")) {
			solution_value = stoi(get(line));
		}
		if (xqdl(line, "Burning sequence")) {
			solution = get_sol(line);
			FILE *xd = fopen(("solutions/gr_bias-07/" + output_name + ".sol").c_str(), "w");
			fprintf(xd, "%d\n", solution_value);
			for (int i = 0; i < solution_value; i++) {
				fprintf(xd, "%d\n", solution[i]);
			}
			// printar no arquivo "intancia".sol
		}
	}
	return 0;
}