#include "cost.h"
#include "rng.h"

bool Cost::valid() {
    if (n <= 0 || n != (int)c.size() || n != (int)c[0].size()) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (c[i][j] < 0) {
                return false;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (c[i][j] != c[j][i]) {
                return false;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (c[i][j] > c[i][k]+c[k][j]) {
                    return false;
                }
            }
        }
    }
    return true;
}
void Cost::generate_random(uint64_t r) {
    if (n <= 0) {
        return;
    }
    std::vector<uint64_t> A(n);
    for (int i = 0; i < n; i++) {
        A[i] = rng(0, r);
    }
    c = std::vector<std::vector<uint64_t>>(n, std::vector<uint64_t>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c[i][j] = abs(A[i]-A[j]);
        }
    }
}
void Cost::set(std::vector<std::vector<uint64_t>> c) {
    this->c = c;
}
uint64_t Cost::get(int a, int b) {
    if (a < 0 || a > n) {
        a = n-1;
    }
    if (b < 0 || b > n) {
        b = n-1;
    }
    return c[a][b];
}
uint64_t Cost::get(int a, int b, bool swapped) {
    if (swapped) {
        std::swap(a, b);
    }
    if (a < 0 || a > n) {
        a = n-1;
    }
    if (b < 0 || b > n) {
        b = n-1;
    }
    return c[a][b];
}
Cost::Cost(int n) : n(n+1) {}
