#include <catch2/catch_test_macros.hpp>
#include "generator.h"

TEST_CASE("Generator produces correct number of edges", "[Generator]") {
    std::size_t scale = 10;
    std::size_t edge_number = 1000;
    Generator gen(scale, edge_number, std::nullopt, 0.25, 0.25, 0.25);
    auto edges = gen.generate();
    REQUIRE(edges.size() == edge_number);
}

TEST_CASE("Generator produces edges within correct range", "[Generator]") {
    std::size_t scale = 10;
    std::size_t edge_number = 1000;
    Generator gen(scale, edge_number, std::nullopt, 0.25, 0.25, 0.25);
    auto edges = gen.generate();
    std::size_t vertex_number = 1 << scale;
    for (const auto& edge : edges) {
        REQUIRE(edge.first < vertex_number);
        REQUIRE(edge.second < vertex_number);
    }
}

TEST_CASE("Generator produces reproducible results with seed", "[Generator]") {
    std::size_t scale = 10;
    std::size_t edge_number = 1000;
    unsigned seed = 42;
    Generator gen1(scale, edge_number, seed, 0.25, 0.25, 0.25);
    Generator gen2(scale, edge_number, seed, 0.25, 0.25, 0.25);
    auto edges1 = gen1.generate();
    auto edges2 = gen2.generate();
    REQUIRE(edges1 == edges2);
}

#if defined (_OPENMP)
TEST_CASE("Generator parallel produces correct number of edges", "[Generator][Parallel]") {
    std::size_t scale = 10;
    std::size_t edge_number = 1000;
    Generator gen(scale, edge_number, std::nullopt, 0.25, 0.25, 0.25);
    auto edges = gen.generate_par();
    REQUIRE(edges.size() == edge_number);
}

TEST_CASE("Generator parallel produces edges within correct range", "[Generator][Parallel]") {
    std::size_t scale = 10;
    std::size_t edge_number = 1000;
    Generator gen(scale, edge_number, std::nullopt, 0.25, 0.25, 0.25);
    auto edges = gen.generate_par();
    std::size_t vertex_number = 1 << scale;
    for (const auto& edge : edges) {
        REQUIRE(edge.first < vertex_number);
        REQUIRE(edge.second < vertex_number);
    }
}

TEST_CASE("Generator parallel produces reproducible results with seed", "[Generator][Parallel]") {
    std::size_t scale = 10;
    std::size_t edge_number = 1000;
    unsigned seed = 42;
    Generator gen1(scale, edge_number, seed, 0.25, 0.25, 0.25);
    Generator gen2(scale, edge_number, seed, 0.25, 0.25, 0.25);
    auto edges1 = gen1.generate_par();
    auto edges2 = gen2.generate_par();
    REQUIRE(edges1 == edges2);
}
#endif