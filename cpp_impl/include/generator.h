#include <random>
#include <optional>
#include <cinttypes>
#include <vector>

class Generator {
public:
    using EdgeList = std::vector<std::pair<std::uint64_t, std::uint64_t>>;
    Generator(std::size_t scale, std::size_t edge_number, std::optional<unsigned> seed,
        float a, float b, float c);
    ~Generator() = default;
    void remap(EdgeList& edges);
    EdgeList generate();
#if defined (_OPENMP)
    EdgeList generate_par();
    void remap_par(EdgeList& edges);
#endif
private:
    std::size_t scale;
    std::size_t edge_number;
    std::optional<unsigned> seed;
    float a;
    float b;
    float c;
};
