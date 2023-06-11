#include "klein.h"

/*
    x: tree rooted at 0
    y: tree rooted at 0
    xf: subforest of tree x
    yf: subforest of tree y
    c: cost function
    dp: memoization table
*/
void trace(
    Tree& x,
    Tree& y,
    Subforest xf,
    Subforest yf,
    Cost& c,
    DP& dp
) {
    if (xf == x.empty && yf == y.empty) {
        return;
    }
    int ins = -1, del = -1;
    if (xf == x.empty || x.size[xf.left] <= x.size[xf.right]) {
        if (
            xf != x.empty && 
            dp.get(State({x.dlr[xf.left][xf.right], yf}))+c.get(x.label[xf.left], del) == dp.get(State({xf, yf}))
        ) {
            std::cout << "del " << xf.left << std::endl;
            trace(x, y, x.dlr[xf.left][xf.right], yf, c, dp);
        } else if (
            yf != y.empty && 
            dp.get(State({xf, y.dlr[yf.left][yf.right]}))+c.get(ins, y.label[yf.left]) == dp.get(State({xf, yf}))
        ) {
            std::cout << "ins " << yf.left << std::endl;
            trace(x, y, xf, y.dlr[yf.left][yf.right], c, dp);
        } else {
            std::cout << "rep " << xf.left << " " << yf.left << std::endl;
            trace(x, y, x.lswr[xf.left][xf.right], y.lswr[yf.left][yf.right], c, dp);
            trace(x, y, x.dls[xf.left][xf.right], y.dls[yf.left][yf.right], c, dp);
        }
        return;
    }
    if (
        xf != x.empty && 
        dp.get(State({x.drr[xf.left][xf.right], yf}))+c.get(x.label[xf.right], del) == dp.get(State({xf, yf}))
    ) {
        std::cout << "del " << xf.right << std::endl;
        trace(x, y, x.drr[xf.left][xf.right], yf, c, dp);
    } else if (
        yf != y.empty && 
        dp.get(State({xf, y.drr[yf.left][yf.right]}))+c.get(ins, y.label[yf.right]) == dp.get(State({xf, yf}))
    ) {
        std::cout << "ins " << yf.right << std::endl;
        trace(x, y, xf, y.drr[yf.left][yf.right], c, dp);
    } else {
        std::cout << "rep " << xf.right << " " << yf.right << std::endl;
        trace(x, y, x.rswr[xf.left][xf.right], y.rswr[yf.left][yf.right], c, dp);
        trace(x, y, x.drs[xf.left][xf.right], y.drs[yf.left][yf.right], c, dp);
    }
    return;
}

/*
    x: tree rooted at 0
    y: tree rooted at 0
    xf: subforest of tree x
    yf: subforest of tree y
    c: metric cost function
    dp: memoization table
*/
uint64_t dist(
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
    if (xf == x.empty || x.size[xf.left] <= x.size[xf.right]) {
        return dp.set(state.value, std::min({
            xf == x.empty ? INT64_MAX : (
                dist(x, y, x.dlr[xf.left][xf.right], yf, c, dp)+c.get(x.label[xf.left], del)
            ), 
            yf == y.empty ? INT64_MAX : (
                dist(x, y, xf, y.dlr[yf.left][yf.right], c, dp)+c.get(ins, y.label[yf.left])
            ), 
            (xf == x.empty || yf == y.empty) ? INT64_MAX : (
                dist(x, y, x.lswr[xf.left][xf.right], y.lswr[yf.left][yf.right], c, dp)+
                dist(x, y, x.dls[xf.left][xf.right], y.dls[yf.left][yf.right], c, dp)+
                c.get(x.label[xf.left], y.label[yf.left])
            )
        }));
    }
    return dp.set(state, std::min({
        xf == x.empty ? INT64_MAX : (
            dist(x, y, x.drr[xf.left][xf.right], yf, c, dp)+c.get(x.label[xf.right], del)
        ), 
        yf == y.empty ? INT64_MAX : (
            dist(x, y, xf, y.drr[yf.left][yf.right], c, dp)+c.get(ins, y.label[yf.right])
        ), 
        (xf == x.empty || yf == y.empty) ? INT64_MAX : (
            dist(x, y, x.rswr[xf.left][xf.right], y.rswr[yf.left][yf.right], c, dp)+
            dist(x, y, x.drs[xf.left][xf.right], y.drs[yf.left][yf.right], c, dp)+
            c.get(x.label[xf.right], y.label[yf.right])
        )
    }));
}

/*
    x: the first tree
    y: the second tree
    c: cost function
    return: the tree edit distance between x and y
*/
uint64_t ted_klein(
    Tree& x,
    Tree& y,
    Cost c
) {
    x.compute(), y.compute();
    if (x.n < y.n) {
        std::swap(x, y);
    }
    DP dp;
    uint64_t ted = dist(x, y, Subforest(0, 0), Subforest(0, 0), c, dp);
    if (y.n < x.n) {
        std::swap(x, y);
    }
    return dp.size();
    return ted;
}
