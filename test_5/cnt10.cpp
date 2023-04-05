#include <bits/stdc++.h>

using namespace std;

int main() {
	int x;
	int cnt = 0;
	while (cin >> x) {
		if (x == 42) cnt++;
	}
	cout << "O 42 apareceu " << cnt << " vezes.\n";
}