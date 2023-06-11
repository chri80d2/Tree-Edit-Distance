#include "demaine.h"

uint64_t dist(
    Tree& x,
    Tree& y,
    Subforest xf,
    Subforest yf,
    Cost& c,
    DP& dp,
    bool swapped
);

/*
    x: the first tree
    y: the second tree
    xf: subforest of tree x
    yf: subforest of tree y
    c: cost function
    dp: memoization table
    swapped: are x and y swapped
    node: heavy node
*/
void step2(
    Tree& x,
    Tree& y,
    Subforest xf,
    Subforest yf,
    Cost& c,
    DP& dp,
    bool swapped, 
    int node
) {
    int heavy = x.heavy_child(node);
    for (auto nei : x.adj[node]) {
        if (nei != x.P[node] && nei != heavy) {
            State state(swapped ? std::make_pair(yf, Subforest(nei, nei)) : std::make_pair(Subforest(nei, nei), yf));
            dp.set(State(state), dist(x, y, Subforest(nei, nei), yf, c, dp, swapped));
        }
    }
    if (heavy != -1) {
        step2(x, y, xf, yf, c, dp, swapped, heavy);
    }
}

/*
    x: the first tree
    y: the second tree
    xf: subforest of tree x
    yf: subforest of tree y
    c: cost function
    dp: memoization table
    swapped: are x and y swapped
*/
uint64_t step3(
    Tree& x,
    Tree& y,
    Subforest xf,
    Subforest yf,
    Cost& c,
    DP& dp,
    bool swapped
) {
    if (xf == x.empty && yf == y.empty) {
        return 0;
    }
    State state(swapped ? std::make_pair(yf, xf) : std::make_pair(xf, yf));
    if (dp.exist(state.value)) {
        return dp.get(state.value);
    }
    int ins = -1, del = -1;
    if (xf.left == xf.right || xf.left != x.subforest_heavy[xf.left][xf.right]) {
        return dp.set(state.value, std::min({
            xf == x.empty ? INT64_MAX : (
                step3(x, y, x.dlr[xf.left][xf.right], yf, c, dp, swapped)+c.get(x.label[xf.left], del, swapped)
            ), 
            yf == y.empty ? INT64_MAX : (
                step3(x, y, xf, y.dlr[yf.left][yf.right], c, dp, swapped)+c.get(ins, y.label[yf.left], swapped)
            ), 
            (xf == x.empty || yf == y.empty) ? INT64_MAX : (
                step3(x, y, x.lswr[xf.left][xf.right], y.lswr[yf.left][yf.right], c, dp, swapped)+
                step3(x, y, x.dls[xf.left][xf.right], y.dls[yf.left][yf.right], c, dp, swapped)+
                c.get(x.label[xf.left], y.label[yf.left], swapped)
            )
        }));
    }
    return dp.set(state, std::min({
        xf == x.empty ? INT64_MAX : (
            step3(x, y, x.drr[xf.left][xf.right], yf, c, dp, swapped)+c.get(x.label[xf.right], del, swapped)
        ), 
        yf == y.empty ? INT64_MAX : (
            step3(x, y, xf, y.drr[yf.left][yf.right], c, dp, swapped)+c.get(ins, y.label[yf.right], swapped)
        ), 
        (xf == x.empty || yf == y.empty) ? INT64_MAX : (
            step3(x, y, x.rswr[xf.left][xf.right], y.rswr[yf.left][yf.right], c, dp, swapped)+
            step3(x, y, x.drs[xf.left][xf.right], y.drs[yf.left][yf.right], c, dp, swapped)+
            c.get(x.label[xf.right], y.label[yf.right], swapped)
        )
    }));
}

/*
    x: the first tree
    y: the second tree
    xf: subforest of tree x
    yf: subforest of tree y
    c: cost function
    dp: memoization table
    swapped: are x and y swapped
*/
uint64_t dist(
    Tree& x,
    Tree& y,
    Subforest xf,
    Subforest yf,
    Cost& c,
    DP& dp,
    bool swapped
) {
    if (xf == x.empty && yf == y.empty) {
        return 0;
    }
    State state(swapped ? std::make_pair(yf, xf) : std::make_pair(xf, yf));
    if (dp.exist(state)) {
        return dp.get(state);
    }
    assert(xf.left == xf.right);
    assert(yf.left == yf.right);
    assert(x.size[xf.left] == x.subforest_size[xf.left][xf.right]);
    assert(y.size[yf.left] == y.subforest_size[yf.left][yf.right]);
    if (x.size[xf.left] < y.size[yf.left]) {
        return dist(y, x, yf, xf, c, dp, !swapped);
    }
    assert(xf != x.empty);
    step2(x, y, xf, yf, c, dp, swapped, xf.left);
    return step3(x, y, xf, yf, c, dp, swapped);
}

/*
    x: the first tree
    y: the second tree
    c: cost function
    return: the tree edit distance between x and y
*/
uint64_t ted_demaine(
    Tree& x,
    Tree& y,
    Cost c
) {
    x.compute(), y.compute();
    DP dp;
    uint64_t ted = dist(x, y, Subforest(0, 0), Subforest(0, 0), c, dp, false);
    return dp.size();
    return ted;
}
