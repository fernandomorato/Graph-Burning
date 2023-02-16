#include <bits/stdc++.h>

using namespace std;

bool prefixo(string a, string b) {
	if ((int) a.size() < (int) b.size())
		return false;
	for (int i = 0; i < (int) b.size(); i++)
		if (b[i] != a[i])
			return false;
	return true;
}

vector<string> split(string s, char ch) {
	string cur = "";
	vector<string> ans;
	for (auto c : s) {
		if (c == ch) {
			ans.push_back(cur);
			cur = "";
		} else {
			cur += c;
		}
	}
	ans.push_back(cur);
	return ans;
}

int main(int argc, char **argv) {
	ios::sync_with_stdio(false);
	cin.tie(0);
	string s(argv[1]);
	// freopen(argv[1], "r", stdin);
	freopen("info2.csv", "a", stdout);
	string line;
	int ans = 1e9;
	while (getline(cin, line)) {
		// cerr << line << '\n';
		if (!prefixo(line, "Burning Number: "))
			continue;
		auto x = split(line, ' ');
		ans = min(ans, stoi(x.back()));
	}
	cout << s << ' ' << ans << '\n';
	return 0;
}