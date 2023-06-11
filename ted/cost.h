#include <iostream>
#include <vector>

#ifndef COST_H_INCLUDED
#define COST_H_INCLUDED

class Cost {
    int n;
    std::vector<std::vector<uint64_t>> c;
public:
    bool valid();
    void generate_random(uint64_t r);
    void set(std::vector<std::vector<uint64_t>> c);
    uint64_t get(int a, int b);
    uint64_t get(int a, int b, bool swapped);
    Cost(int n);
};

#endif
