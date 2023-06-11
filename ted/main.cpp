#include <iostream>
#include <iomanip>
#include <chrono>
#include "tree.h"
#include "left_strategy.h"
#include "sasha_zhang.h"
#include "klein.h"
#include "demaine.h"
#include "tree_generator.h"
#include "rng.h"
#include "cost.h"

void from_input() {
    int n;
    std::cin >> n;
    Tree x(n);
    for (int i = 0; i < 2*(n-1); i++) {
        int u, v;
        std::cin >> u >> v;
        if (u < v) {
            x.add_edge(u, v);
        }
    }
    for (int i = 0; i < n; i++) {
        std::cin >> x.label[i];
    }
    int m;
    std::cin >> m;
    Tree y(m);
    for (int i = 0; i < 2*(m-1); i++) {
        int u, v;
        std::cin >> u >> v;
        if (u < v) {
            y.add_edge(u, v);
        }
    }
    for (int i = 0; i < m; i++) {
        std::cin >> y.label[i];
    }
    Cost c(std::max(x.n, y.n));
    c.generate_random(10);
    assert(c.valid());
    uint64_t sasha_zhang = ted_sasha_zhang(x, y, c);
    uint64_t klein = ted_klein(x, y, c);
    uint64_t demaine = ted_demaine(x, y, c);
    std::cout << "sasha zhang: " << sasha_zhang << std::endl;
    std::cout << "klein: " << klein << std::endl;
    std::cout << "demaine: " << demaine << std::endl;
    x.print_tree();
    y.print_tree();
}

void random() {
    std::vector<Tree> trees;
    int N = 10, T = 100;
    for (int t = 0; t < T; t++) {
        int n = rng(N, N);
        std::vector<int> A(n-2);
        for (int i = 0; i < n-2; i++) {
            A[i] = rng(0, n-1);
        }
        Tree tree = prufer_to_tree(A);
        trees.push_back(tree);
    }
    int cnt = 0;
    for (int i = 0; i < T; i++) {
        for (int j = i+1; j < T; j++) {
            Tree x = trees[i], y = trees[j];
            Cost c(std::max(x.n, y.n));
            c.generate_random(10);
            assert(c.valid());
            uint64_t left_strategy = ted_left_strategy(x, y, c);
            uint64_t sasha_zhang = ted_sasha_zhang(x, y, c);
            uint64_t klein = ted_klein(x, y, c);
            uint64_t demaine = ted_demaine(x, y, c);
            if (sasha_zhang != left_strategy || klein != sasha_zhang || demaine != klein) {
                std::cout << "left strategy: " << left_strategy << std::endl;
                std::cout << "sasha zhang: " << sasha_zhang << std::endl;
                std::cout << "klein: " << klein << std::endl;
                std::cout << "demaine: " << demaine << std::endl;
                x.print_tree();
                y.print_tree();
                return;
            } else {
                cnt++;
            }
        }
    }
    std::cout << "correctly computed ted for " << cnt << " pair of trees" << std::endl;
}

void tree_types() {
    int N = 50, start = 3;
    std::vector<std::vector<Tree>> trees(4, std::vector<Tree>(start));
    for (int n = start; n <= N; n++) {
        trees[0].push_back(short_binary(n));
        trees[1].push_back(long_left(n));
        trees[2].push_back(long_right(n));
        std::vector<int> A(n-2);
        for (int i = 0; i < n-2; i++) {
            A[i] = rng(0, n-1);
        }
        trees[3].push_back(prufer_to_tree(A));
    }
    for (int i = 0; i < 4; i++) {
        for (int n = start; n <= N; n++) {
            trees[i][n].compute();
        }
    }
    std::vector<std::string> A = {
        "short binary",
        "long left",
        "long right",
        "random"
    };
    std::chrono::time_point<std::chrono::high_resolution_clock> t1;
    std::chrono::time_point<std::chrono::high_resolution_clock> t2;
    std::chrono::duration<double> left_strategy_s, sasha_zhang_s, klein_s, demaine_s;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << N << std::endl;
            std::cout << A[i] << " and " << A[j] << std::endl;
            for (int n = start; n <= N; n++) {
                Tree x = trees[i][n], y = trees[j][n];
                Cost c(std::max(x.n, y.n));
                c.generate_random(10);
                //assert(c.valid());
                t1 = std::chrono::high_resolution_clock::now();
                uint64_t left_strategy = ted_left_strategy(x, y, c);
                t2 = std::chrono::high_resolution_clock::now();
                left_strategy_s = t2-t1;
                t1 = std::chrono::high_resolution_clock::now();
                uint64_t sasha_zhang = ted_sasha_zhang(x, y, c);
                t2 = std::chrono::high_resolution_clock::now();
                sasha_zhang_s = t2-t1;
                t1 = std::chrono::high_resolution_clock::now();
                uint64_t klein = ted_klein(x, y, c);
                t2 = std::chrono::high_resolution_clock::now();
                klein_s = t2-t1;
                t1 = std::chrono::high_resolution_clock::now();
                uint64_t demaine = ted_demaine(x, y, c);
                t2 = std::chrono::high_resolution_clock::now();
                demaine_s = t2-t1;
                std::cout << 
                    left_strategy << " " << 
                    sasha_zhang << " " << 
                    klein << " " << 
                    demaine << " " <<
                    int(left_strategy_s.count()*1000) << " " <<
                    int(sasha_zhang_s.count()*1000) << " " <<
                    int(klein_s.count()*1000) << " " <<
                    int(demaine_s.count()*1000) <<
                std::endl;
            }
        }
    }
}

int main() {

    tree_types();

    return 0;
}
/*
g++ -std=c++17 -Wl,--stack,268435456 main.cpp klein.cpp rng.cpp sasha_zhang.cpp tree_generator.cpp tree.cpp demaine.cpp cost.cpp dp.cpp left_strategy.cpp -o a.exe
*/
