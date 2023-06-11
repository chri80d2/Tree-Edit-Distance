#include "tree.h"

Subforest::Subforest() {}
Subforest::Subforest(int left, int right) : left(left), right(right) {}
bool operator==(const Subforest& x, const Subforest& y) {
    return x.left == y.left && x.right == y.right;
}
bool operator!=(const Subforest& x, const Subforest& y) {
    return !(x == y);
}
bool operator<(const Subforest& x, const Subforest& y) {
    return x.left == y.left ? x.right < y.right : x.left < y.left;
}
bool operator<=(const Subforest& x, const Subforest& y) {
    return x < y || x == y;
}
bool operator>(const Subforest& x, const Subforest& y) {
    return x.left == y.left ? x.right > y.right : x.left > y.left;
}
bool operator>=(const Subforest& x, const Subforest& y) {
    return x > y || x == y;
}
void Tree::subtree_size(int node, int parent) {
    P[node] = parent;
    size[node] = 1;
    for (auto nei : adj[node]) {
        if (nei == parent) {
            continue;
        }
        subtree_size(nei, node);
        size[node] += size[nei];
    }
}
void Tree::left_right_preorder(int node, int parent, std::vector<int>& P) {
    P.push_back(node);
    for (auto nei : adj[node]) {
        if (nei == parent) {
            continue;
        }
        left_right_preorder(nei, node, P);
    }
}
void Tree::left_right_postorder(int node, int parent, std::vector<int>& P) {
    for (auto nei : adj[node]) {
        if (nei == parent) {
            continue;
        }
        left_right_postorder(nei, node, P);
    }
    P.push_back(node);
}
void Tree::right_left_postorder(int node, int parent, std::vector<int>& P) {
    for (int i = (int)adj[node].size()-1; i >= 0; i--) {
        int nei = adj[node][i];
        if (nei == parent) {
            continue;
        }
        right_left_postorder(nei, node, P);
    }
    P.push_back(node);
}
int Tree::right_leaf(int node, int parent, std::vector<int>& RL) {
    int leaf = node;
    for (auto nei : adj[node]) {
        if (nei == parent) {
            continue;
        }
        leaf = right_leaf(nei, node, RL);
    }
    return RL[node] = leaf;
}
int Tree::left_leaf(int node, int parent, std::vector<int>& LL) {
    int leaf = node;
    for (int i = (int)adj[node].size()-1; i >= 0; i--) {
        int nei = adj[node][i];
        if (nei == parent) {
            continue;
        }
        leaf = left_leaf(nei, node, LL);
    }
    return LL[node] = leaf;
}
void Tree::dfs(int node, int p, std::vector<int>& parent) {
    parent[node] = p;
    for (auto nei : adj[node]) {
        if (nei == p) {
            continue;
        }
        dfs(nei, node, parent);
    }
}
Tree::Tree() : n(0), is_computed(false) {}
Tree::Tree(int n) : n(n), is_computed(false) {
    adj.resize(n);
    label.resize(n);
}
Tree::Tree(int n, std::vector<std::vector<int>> adj, std::vector<int> label):
    n(n), adj(adj), label(label), is_computed(false)
{}
void Tree::compute() {
    if (is_computed) {
        return;
    }
    is_computed = true;
    std::vector<int> order, order_t(n);
    left_right_preorder(0, -1, order);
    for (int i = 0; i < n; i++) {
        order_t[order[i]] = i;
    }
    std::vector<std::vector<int>> reordered_adj(n);
    std::vector<int> reordered_label(n);
    for (int i = 0; i < n; i++) {
        reordered_adj[i] = adj[order[i]];
        reordered_label[i] = label[order[i]];
        for (int j = 0; j < (int)reordered_adj[i].size(); j++) {
            reordered_adj[i][j] = order_t[reordered_adj[i][j]];
        }
    }
    adj = reordered_adj;
    label = reordered_label;
    std::vector<int> parent(n);
    dfs(0, -1, parent);
    for (int i = 1; i < n; i++) {
        adj[i].erase(std::find(adj[i].begin(), adj[i].end(), parent[i]));
        adj[i].insert(adj[i].begin(), parent[i]);
    }
    size.resize(n);
    P.resize(n+1, -1);
    subtree_size(0, -1);
    H.resize(n, -1);
    for (int node = 0; node < n; node++) {
        std::pair<int, int> heavy = {-1, -1};
        for (auto nei : adj[node]) {
            if (nei == P[node]) {
                continue;
            }
            heavy = std::max(heavy, {size[nei], nei});
        }
        H[node] = heavy.second;
    }
    empty = Subforest(n, n);
    dlr.resize(n+1, std::vector<Subforest>(n+1, empty));
    drr.resize(n+1, std::vector<Subforest>(n+1, empty));
    dls.resize(n+1, std::vector<Subforest>(n+1, empty));
    drs.resize(n+1, std::vector<Subforest>(n+1, empty));
    lswr.resize(n+1, std::vector<Subforest>(n+1, empty));
    rswr.resize(n+1, std::vector<Subforest>(n+1, empty));
    LRPI.resize(n), RLPI.resize(n), RL.resize(n), LL.resize(n);
    left_right_postorder(0, -1, LRP);
    right_left_postorder(0, -1, RLP);
    right_leaf(0, -1, RL);
    left_leaf(0, -1, LL);
    for (int i = 0; i < n; i++) {
        LRPI[LRP[i]] = i;
        RLPI[RLP[i]] = i;
    }
    std::vector<bool> r_removed(n, true), l_removed(n, true);
    subforest_size.resize(n+1, std::vector<int>(n+1));
    subforest_heavy.resize(n+1, std::vector<int>(n+1));
    int sz = 0;
    for (auto r : LRP) { // everything after r is removed
        r_removed[r] = false;
        std::fill(l_removed.begin(), l_removed.end(), true);
        sz = 0;
        std::pair<int, int> heavy = {-1, -1};
        for (auto l : RLP) { // everything after l is removed
            l_removed[l] = false;
            if (!r_removed[l]) {
                sz++;
            }
            if (r_removed[l] || l_removed[r]) { // make sure l and r is not removed
                continue;
            }
            heavy = std::max(heavy, {size[l], l});
            subforest_size[l][r] = sz;
            subforest_heavy[l][r] = heavy.second;
        }
    }
    std::fill(r_removed.begin(), r_removed.end(), true);
    for (auto r : LRP) {
        r_removed[r] = false;
        std::vector<int> last, lasti(n, -1);
        std::fill(l_removed.begin(), l_removed.end(), true);
        for (auto l : RLP) {
            l_removed[l] = false;
            if (!r_removed[l]) {
                lasti[l] = (int)last.size();
                last.push_back(l);
            }
            if (r_removed[l] || l_removed[r]) {
                continue;
            }
            int k = lasti[l] <= 0 ? -1 : last[lasti[l]-1];
            if (l == r && k != -1) {
                k = RLP[RLPI[l]-1];
            }
            if (k != -1) {
                dlr[l][r] = Subforest(k, r);
            }
            k = lasti[RL[l]] <= 0 ? -1 : last[lasti[RL[l]]-1];
            if (l == r && k != -1) {
                k = RLP[RLPI[RL[l]]-1];
            }
            if (k != -1) {
                dls[l][r] = Subforest(k, r);
            }
            if ((l == 0 && (int)adj[l].size()) || (int)adj[l].size() > 1) {
                lswr[l][r] = Subforest(
                    l == 0 ? *adj[l].begin() : *++adj[l].begin(), 
                    *adj[l].rbegin()
                );
            }
            if ((r == 0 && (int)adj[r].size()) || (int)adj[r].size() > 1) {
                rswr[l][r] = Subforest(
                    r == 0 ? *adj[r].begin() : *++adj[r].begin(), 
                    *adj[r].rbegin()
                );
            }
        }
    }
    std::fill(l_removed.begin(), l_removed.end(), true);
    for (auto l : RLP) {
        l_removed[l] = false;
        std::vector<int> last, lasti(n, -1);
        std::fill(r_removed.begin(), r_removed.end(), true);
        for (auto r : LRP) {
            r_removed[r] = false;
            if (!l_removed[r]) {
                lasti[r] = (int)last.size();
                last.push_back(r);
            }
            if (r_removed[l] || l_removed[r]) {
                continue;
            }
            int k = lasti[r] <= 0 ? -1 : last[lasti[r]-1];
            if (l == r && k != -1) {
                k = LRP[LRPI[r]-1];
            }
            if (k != -1) {
                drr[l][r] = Subforest(l, k);
            }
            if (l == r) {
                dlr[l][r].right = drr[l][r].right;
                drr[l][r].left = dlr[l][r].left;
            }
            k = lasti[LL[r]] <= 0 ? -1 : last[lasti[LL[r]]-1];
            if (l == r && k != -1) {
                k = LRP[LRPI[LL[r]]-1];
            }
            if (k != -1) {
                drs[l][r] = Subforest(l, k);
            }
        }
    }
}
int Tree::heavy_child(int node) {
    if (node == -1) {
        return 0;
    }
    return H[node];
}
void Tree::label_node(int node, int x) {
    label[node] = x;
}
void Tree::add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}
void Tree::print_tree() {
    std::cout << n << std::endl;
    for (int node = 0; node < n; node++) {
        for (auto nei : adj[node]) {
            std::cout << node << " " << nei << std::endl;
        }
    }
    for (int i = 0; i < n; i++) {
        std::cout << label[i] << " ";
    }
    std::cout << std::endl;
}
