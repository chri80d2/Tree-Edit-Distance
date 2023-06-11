#include "left_strategy.h"

/*
    x: tree rooted at 0
    y: tree rooted at 0
    xf: subforest of tree x
    yf: subforest of tree y
    c: metric cost function
    dp: memoization table
*/
uint64_t d(
    Tree& x,
    Tree& y,
    Subforest xf,
    Subforest yf,
    Cost& c,
    DP& dp
) {
    if (xf == x.empty && yf == y.empty) {
        return 0;
    }
    State state({xf, yf});
    if (dp.exist(state.value)) {
        return dp.get(state.value);
    }
    int ins = -1, del = -1;
    return dp.set(state.value, std::min({
        xf == x.empty ? INT64_MAX : (
            d(x, y, x.dlr[xf.left][xf.right], yf, c, dp)+c.get(x.label[xf.left], del)
        ), 
        yf == y.empty ? INT64_MAX : (
            d(x, y, xf, y.dlr[yf.left][yf.right], c, dp)+c.get(ins, y.label[yf.left])
        ), 
        (xf == x.empty || yf == y.empty) ? INT64_MAX : (
            d(x, y, x.lswr[xf.left][xf.right], y.lswr[yf.left][yf.right], c, dp)+
            d(x, y, x.dls[xf.left][xf.right], y.dls[yf.left][yf.right], c, dp)+
            c.get(x.label[xf.left], y.label[yf.left])
        )
    }));
}

/*
    x: the first tree
    y: the second tree
    c: cost function
    return: the tree edit distance between x and y
*/
uint64_t ted_left_strategy(
    Tree& x,
    Tree& y,
    Cost c
) {
    x.compute(), y.compute();
    DP dp;
    uint64_t ted = d(x, y, Subforest(0, 0), Subforest(0, 0), c, dp);
    return dp.size();
    return ted;
}
