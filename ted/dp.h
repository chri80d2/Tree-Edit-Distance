#include <ext/pb_ds/assoc_container.hpp>
#include <chrono>
#include "tree.h"

#ifndef DP_H_INCLUDED
#define DP_H_INCLUDED

struct custom_hash {
    static uint64_t splitmix64(uint64_t x);
    size_t operator()(std::pair<Subforest, Subforest> x) const;
};

struct State {
    std::pair<Subforest, Subforest> value;
    State(std::pair<Subforest, Subforest> value);
};

class DP {
    __gnu_pbds::gp_hash_table<std::pair<Subforest, Subforest>, uint64_t, custom_hash> dp;
public:
    DP();
    int size();
    bool exist(State state);
    uint64_t get(State state);
    uint64_t set(State state, uint64_t value);
};

#endif
