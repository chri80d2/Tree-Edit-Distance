#include "sasha_zhang.h"

/*
    adj: adjacency list of a forest
    return: keyroots of the forest
*/
std::vector<int> keyroots(std::vector<std::vector<int>>& adj, std::vector<int>& RL) {
    int n = (int)adj.size();
    std::vector<int> A(n, 1);
    std::vector<int> R;
    for (int i = 0; i < n; i++) {
        if (A[RL[i]]) {
            A[RL[i]] = 0;
            R.push_back(i);
        }
    }
    std::reverse(R.begin(), R.end());
    return R;
}

/*
    x: the first tree
    y: the second tree
    c: cost function
    return: the tree edit distance between x and y
*/
uint64_t ted_sasha_zhang(
    Tree& x,
    Tree& y,
    Cost c
) {
    x.compute();
    y.compute();
    int n = x.n, m = y.n;
    std::vector<int> XRL = x.RL;
    std::vector<int> YRL = y.RL;
    std::vector<int> XL = x.label;
    std::vector<int> YL = y.label;
    std::vector<int> XR = keyroots(x.adj, XRL);
    std::vector<int> YR = keyroots(y.adj, YRL);
    int ins = -1, del = -1;
    std::vector<std::vector<uint64_t>> d(n, std::vector<uint64_t>(m));
    std::vector<std::vector<uint64_t>> D(n+1, std::vector<uint64_t>(m+1));
    int cnt = 0;
    for (auto k : XR) {
        for (auto l : YR) {
            D[XRL[k]+1][YRL[l]+1] = 0;
            for (int i = XRL[k]; i >= k; i--) {
                cnt++;
                D[i][YRL[l]+1] = D[i+1][YRL[l]+1]+c.get(XL[i], del);
            }
            for (int j = YRL[l]; j >= l; j--) {
                cnt++;
                D[XRL[k]+1][j] = D[XRL[k]+1][j+1]+c.get(ins, YL[j]);
            }
            for (int i = XRL[k]; i >= k; i--) {
                for (int j = YRL[l]; j >= l; j--) {
                    cnt++;
                    if (XRL[i] == XRL[k] && YRL[j] == YRL[l]) {
                        D[i][j] = std::min({
                            D[i+1][j]+c.get(XL[i], del),
                            D[i][j+1]+c.get(ins, YL[j]),
                            D[i+1][j+1]+c.get(XL[i], YL[j])
                        });
                        d[i][j] = D[i][j];
                        continue;
                    }
                    D[i][j] = std::min({
                        D[i+1][j]+c.get(XL[i], del),
                        D[i][j+1]+c.get(ins, YL[j]),
                        D[XRL[i]+1][YRL[j]+1]+d[i][j]
                    });
                }
            }
        }
    }
    return cnt;
    return d[0][0];
}
