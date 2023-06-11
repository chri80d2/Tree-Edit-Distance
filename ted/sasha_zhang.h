#include <iostream>
#include <vector>
#include <algorithm>
#include "tree.h"
#include "cost.h"

#ifndef SASHA_ZHANG_H_INCLUDED
#define SASHA_ZHANG_H_INCLUDED

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
);

#endif
