// reverse for lds...
vector<int> lis(const vector<int>& A) {
	vector<int> prev(sz(A));
	vector<pair<int, int>> res;
	for (int i = 0; i < sz(A); i++) {
		pair<int, int> el {A[i], i};
		// A[i]+1 here for non-decreasing
		auto it = lower_bound(all(res), pair<int, int> {A[i], 0}); 
		if (it == res.end()) res.push_back(el), it = --res.end();
		*it = el;
		prev[i] = (it == res.begin() ? 0 : (it-1)->snd); 
	}
	int L = sz(res), cur = res.back().snd;
	vector<int> ans(L);
	while (L--) ans[L] = cur, cur = prev[cur];
	return ans;
}

