#include <iostream>
#include <vector>
#include <map>
#include "tree.h"
#include "cost.h"
#include "dp.h"

#ifndef LEFT_STRATEGY_H_INCLUDED
#define LEFT_STRATEGY_H_INCLUDED

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
);

#endif
