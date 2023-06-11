#include <vector>
#include "tree.h"
#include "rng.h"

#ifndef TREE_GENERATOR_H_INCLUDED
#define TREE_GENERATOR_H_INCLUDED

Tree prufer_to_tree(std::vector<int> A);
Tree short_binary(int n);
Tree long_left(int n);
Tree long_right(int n);
Tree zig_zag(int n);

#endif
