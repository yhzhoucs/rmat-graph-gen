// Copyright (c) 2015, The Regents of the University of California (Regents)
// See LICENSE.txt for license details

#include <iostream>
#include <vector>
#include <fstream>

#include "benchmark.h"

using namespace std;

int main(int argc, char* argv[]) {
  CLApp cli(argc, argv, "breadth-first search");
  if (!cli.ParseArgs())
    return -1;
  Builder b(cli);
  Graph g = b.MakeGraph();
  g.PrintStats();
  std::ofstream out{cli.get_filename(), std::ios::out | std::ios::trunc};
  if (!out.is_open()) {
    std::cerr << "Error: open file failed" << std::endl;
    return -1;
  }
  auto vertex_number = g.num_nodes();
  auto edge_number = g.num_edges();
  auto d_edge_number = g.num_edges_directed();
  out << "# vertex_number edge_number directed_edge_number\n";
  out << "# " << vertex_number << " " << edge_number << " " << d_edge_number << std::endl;
  for (int64_t u = 0; u < vertex_number; ++u) {
    for (auto v : g.out_neigh(u)) {
      out << u << " " << v << std::endl;
    }
  }
  out.close();
  return 0;
}
