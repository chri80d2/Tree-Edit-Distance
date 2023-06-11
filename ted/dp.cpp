#include "dp.h"

uint64_t custom_hash::splitmix64(uint64_t x) {
    // http://xorshift.di.unimi.it/splitmix64.c
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}
size_t custom_hash::operator()(std::pair<Subforest, Subforest> x) const {
    static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
    return
        42*splitmix64(x.first.left+FIXED_RANDOM)+
        1764*splitmix64(x.first.right+FIXED_RANDOM)+
        74088*splitmix64(x.second.left+FIXED_RANDOM)+
        3421*splitmix64(x.second.right+FIXED_RANDOM)
    ;
}
State::State(std::pair<Subforest, Subforest> value) : value(value) {}
DP::DP() {}
int DP::size() {
    return (int)dp.size();
}
bool DP::exist(State state) {
    return dp.find(state.value) != dp.end();
}
uint64_t DP::get(State state) {
    return dp[state.value];
}
uint64_t DP::set(State state, uint64_t value) {
    return dp[state.value] = value;
}
