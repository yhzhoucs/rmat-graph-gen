#include "generator.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cxxopts.hpp>

int main(int argc, char *argv[]) {
    cxxopts::Options options("rmat_graph_gen", "A cpp-based RMAT graph generator");
    options.add_options()
        ("params",
            "a, b, c",
            cxxopts::value<std::vector<float>>())
        ("s,scale", "2^s vertices", cxxopts::value<std::size_t>())
        ("o,outfile", "output file", cxxopts::value<std::string>()->default_value("output.txt"))
        ("p,par", "generate in parallel", cxxopts::value<bool>()->default_value("false"))
        ("r,randseed", "random seed", cxxopts::value<unsigned>())
        ("d,degree", "average degree", cxxopts::value<std::size_t>()->default_value("16"))
        ("h,help", "Print usage");
    
    options.parse_positional({"params"});
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (!result.count("scale")) {
        std::cerr << "Scale is required" << std::endl;
        return 1;
    }
    std::size_t scale = result["scale"].as<std::size_t>();

    float a = 0.57, b = 0.19, c = 0.19;
    if (result.count("params")) {
        auto params = result["params"].as<std::vector<float>>();
        if (params.size() != 3) {
            std::cerr << "Invalid number of parameters" << std::endl;
            return 1;
        }
        a = params[0];
        b = params[1];
        c = params[2];
    }

    std::optional<unsigned> seed;
    if (result.count("randseed")) {
        seed = result["randseed"].as<unsigned>();
    }

    std::string outfile = result["outfile"].as<std::string>();
    std::size_t degree = result["degree"].as<std::size_t>();
    std::size_t edge_number = (1 << scale) * degree;

    Generator gen(scale, edge_number, seed, a, b, c);
    Generator::EdgeList edges;

    bool par = result["par"].as<bool>();
#if defined(_OPENMP)
    if (par) {
        edges = gen.generate_par();
    } else {
        edges = gen.generate();
    }
#else
    if (par) {
        std::cerr << "Parallel generation is not supported"
            "\nCheck OpenMP support in your system" << std::endl;
        return 1;
    } else {
        edges = gen.generate();
    }
#endif

    std::cout << "Generated " << edges.size() << " edges" << std::endl;
    namespace fs = std::filesystem;
    auto path = fs::path(outfile);
    if (path.has_parent_path() && !fs::exists(path.parent_path())) {
        fs::create_directories(path.parent_path());
    }
    std::ofstream out(outfile, std::ios::out | std::ios::trunc);
    for (const auto& edge : edges) {
        out << edge.first << " " << edge.second << "\n";
    }
    out.flush();
    out.close();

    return 0;
}