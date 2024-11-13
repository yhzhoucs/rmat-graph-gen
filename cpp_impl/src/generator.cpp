#include "generator.h"
#include <iostream>
#include <algorithm>
#if defined (_OPENMP)
#include <omp.h>
#endif

Generator::Generator(std::size_t scale, std::size_t edge_number, std::optional<unsigned> seed, float a, float b, float c)
    : scale(scale), edge_number(edge_number), seed(seed), a(a), b(b), c(c) {}

void Generator::remap(EdgeList& edges) {
    std::size_t vertex_number = 1 << scale;
    std::vector<std::size_t> new_ids(vertex_number);
    std::iota(new_ids.begin(), new_ids.end(), 0);
    std::mt19937 rng(seed.value_or(std::random_device()()));
    std::shuffle(new_ids.begin(), new_ids.end(), rng);
    for (auto& edge : edges) {
        edge.first = new_ids[edge.first];
        edge.second = new_ids[edge.second];
    }
}

Generator::EdgeList Generator::generate() {
    EdgeList edges;
    edges.reserve(edge_number);
    std::mt19937 gen(seed.value_or(std::random_device()()));
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    for (std::size_t i = 0; i < edge_number; ++i) {
        std::uint64_t src = 0;
        std::uint64_t dst = 0;
        for (std::size_t depth = 0; depth < scale; ++depth) {
            src <<= 1;
            dst <<= 1;
            float r = dist(gen);
            if (r < a+b) {
                if (r > a)
                    ++dst;
            } else {
                ++src;
                if (r > a+b+c)
                    ++dst;
            }
        }
        edges.emplace_back(src, dst);
    }
    remap(edges);
    return edges;
}

#if defined (_OPENMP)
Generator::EdgeList Generator::generate_par() {
    EdgeList edges(edge_number, {0, 0});
    std::size_t chunk_size = (edge_number + omp_get_max_threads() - 1) / omp_get_max_threads();
    #pragma omp parallel
    {
        std::mt19937 gen;
        std::uniform_real_distribution<float> dist(0.0, 1.0);
        std::size_t chunk_idx = omp_get_thread_num();
        if (seed.has_value())
            gen.seed(seed.value() + chunk_idx);
        else
            gen.seed(std::random_device()());
        std::size_t first = chunk_idx * chunk_size;
        std::size_t last = std::min(first + chunk_size, edge_number);
        for ( ; first < last; ++first) {
            std::uint64_t src = 0;
            std::uint64_t dst = 0;
            for (std::size_t depth = 0; depth < scale; ++depth) {
                src <<= 1;
                dst <<= 1;
                float r = dist(gen);
                if (r < a+b) {
                    if (r > a)
                        dst += 1;
                } else {
                    src += 1;
                    if (r > a+b+c)
                        dst += 1;
                }
            }
            edges[first] = {src, dst};
        }
    }
    remap(edges);
    return edges;
}

void Generator::remap_par(EdgeList& edges) {
    std::size_t vertex_number = 1 << scale;
    std::vector<std::size_t> new_ids(vertex_number);
    std::iota(new_ids.begin(), new_ids.end(), 0);
    std::mt19937 rng(seed.value_or(std::random_device()()));
    std::shuffle(new_ids.begin(), new_ids.end(), rng);
    #pragma omp parallel for
    for (int i = 0; i < edges.size(); ++i) {
        edges[i].first = new_ids[edges[i].first];
        edges[i].second = new_ids[edges[i].second];
    }
}
#endif