#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>
#include "generator.h"

int main() {
    std::size_t scale = 16;
    std::size_t edge_number = (1<<18);
    Generator gen(scale, edge_number, std::nullopt, 0.25, 0.25, 0.25);

    ankerl::nanobench::Bench().run("Benchmaring generation in sequential mode", [&gen] {
        Generator::EdgeList el = gen.generate();
        ankerl::nanobench::doNotOptimizeAway(el);
    });

    ankerl::nanobench::Bench().run("Benchmarking generation in parallel mode", [&gen] {
        Generator::EdgeList el = gen.generate_par();
        ankerl::nanobench::doNotOptimizeAway(el);
    });

    return 0;
}