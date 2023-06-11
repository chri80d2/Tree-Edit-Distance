#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <assert.h>

#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

class Subforest {
public:
    int left;
    int right;
    Subforest();
    Subforest(int left, int right);
};

bool operator==(const Subforest& x, const Subforest& y);
bool operator!=(const Subforest& x, const Subforest& y);
bool operator<(const Subforest& x, const Subforest& y);
bool operator<=(const Subforest& x, const Subforest& y);
bool operator>(const Subforest& x, const Subforest& y);
bool operator>=(const Subforest& x, const Subforest& y);

class Tree {
    bool is_computed;
    void subtree_size(int node, int parent);
    void left_right_preorder(int node, int parent, std::vector<int>& P);
    void left_right_postorder(int node, int parent, std::vector<int>& P);
    void right_left_postorder(int node, int parent, std::vector<int>& P);
    int right_leaf(int node, int parent, std::vector<int>& RL);
    int left_leaf(int node, int parent, std::vector<int>& LL);
    void dfs(int node, int p, std::vector<int>& parent);
public:
    int n;
    std::vector<std::vector<int>> adj;
    std::vector<int> label;
    std::vector<int> size;
    std::vector<int> P;
    std::vector<int> H;
    std::vector<std::vector<Subforest>> dlr; // delete left root
    std::vector<std::vector<Subforest>> drr; // delete right root
    std::vector<std::vector<Subforest>> dls; // delete left subtree
    std::vector<std::vector<Subforest>> drs; // delete right subtree
    std::vector<std::vector<Subforest>> lswr; // left subtree without root
    std::vector<std::vector<Subforest>> rswr; // right subtree without root
    std::vector<int> LRP;
    std::vector<int> RLP;
    std::vector<int> LRPI;
    std::vector<int> RLPI;
    std::vector<int> RL;
    std::vector<int> LL;
    std::vector<std::vector<int>> subforest_size;
    std::vector<std::vector<int>> subforest_heavy;
    Subforest empty;
    Tree();
    Tree(int n);
    Tree(int n, std::vector<std::vector<int>> adj, std::vector<int> label);
    void compute();
    int heavy_child(int node);
    void label_node(int node, int x);
    void add_edge(int u, int v);
    void print_tree();
};

#endif
