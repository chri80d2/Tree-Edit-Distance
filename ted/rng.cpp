#include "rng.h"

std::mt19937 mt(std::chrono::steady_clock::now().time_since_epoch().count());

int rng(int l, int r) {
    return mt()%(r-l+1)+l;
}
