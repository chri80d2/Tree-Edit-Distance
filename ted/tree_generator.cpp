#include "tree_generator.h"
#include <assert.h>

/*
    https://en.wikipedia.org/wiki/Pr%C3%BCfer_sequence
*/
Tree prufer_to_tree(std::vector<int> A) {
    int n = (int)A.size();
    Tree tree(n+2);
    for (int i = 0; i < n+2; i++) {
        tree.label[i] = rng(0, n+1);
    }
    std::vector<int> degree(n+2, 1);
    for (auto x : A) {
        degree[x]++;
    }
    for (auto x : A) {
        for (int i = 0; i < n+2; i++) {
            if (degree[i] == 1) {
                tree.add_edge(x, i);
                degree[x]--;
                degree[i]--;
                break;
            }
        }
    }
    int u = -1, v = -1;
    for (int i = 0; i < n+2; i++) {
        if (degree[i] == 1) {
            if (u == -1) {
                u = i;
                continue;
            }
            v = i;
            break;
        }
    }
    tree.add_edge(u, v);
    return tree;
}

Tree short_binary(int n) {
    Tree tree(n);
    for (int i = 0; i < n; i++) {
        tree.label[i] = rng(0, n-1);
    }
    for (int i = 1; i <= n; i++) {
        if (i*2 <= n) {
            tree.add_edge(i-1, i*2-1);
        }
        if (i*2+1 <= n) {
            tree.add_edge(i-1, i*2);
        }
    }
    return tree;
}

Tree long_left(int n) {
    Tree tree(n);
    for (int i = 0; i < n; i++) {
        tree.label[i] = rng(0, n-1);
    }
    for (int i = 1; i < n/2; i++) {
        tree.add_edge(i-1, i);
    }
    for (int i = 0; i < n/2; i++) {
        tree.add_edge(i, i+n/2);
    }
    if (n != 1 && n%2) {
        tree.add_edge(n/2-1, n-1);
    }
    return tree;
}

Tree long_right(int n) {
    Tree tree(n);
    for (int i = 0; i < n; i++) {
        tree.label[i] = rng(0, n-1);
    }
    for (int i = 0; i < n/2; i++) {
        tree.add_edge(i, i+n/2);
    }
    if (n != 1 && n%2) {
        tree.add_edge(n/2-1, n-1);
    }
    for (int i = 1; i < n/2; i++) {
        tree.add_edge(i-1, i);
    }
    return tree;
}

Tree zig_zag(int n) {
    Tree tree(n);
    for (int i = 0; i < n; i++) {
        tree.label[i] = rng(0, n-1);
    }
    for (int i = 1; i < n/2; i += 2) {
        tree.add_edge(i-1, i);
    }
    for (int i = 0; i < n/2; i++) {
        tree.add_edge(i, i+n/2);
    }
    if (n != 1 && n%2) {
        tree.add_edge(n/2-1, n-1);
    }
    for (int i = 2; i < n/2; i += 2) {
        tree.add_edge(i-1, i);
    }
    return tree;
}
