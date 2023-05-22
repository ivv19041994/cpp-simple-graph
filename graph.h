#include <vector>
#include <limits>
#include <optional>

namespace ivv{
template<
    typename Weight
    //template <typename...> typename RandomAccessContainer = std::vector
>
class Graph {
public:

    template<typename T>
    using RandomAccessContainer = std::vector<T>;

    template<typename T>
    using Optional = std::optional<T>;

    struct Edge {
        size_t from;
        size_t to;
        Weight weight;
    };
    
    struct Path {
        Weight weight;
        RandomAccessContainer<size_t> vertexs;
    };
    
    //Interface:
    //Graph(size_t vertexCount, const ForwardContainer& edges, bool directed = true);
    //Graph(size_t vertexCount, ForwardIterator begin, ForwardIterator end, bool directed = true);
    //Optional<Path> getPath(size_t from, size_t to);
    
    template<typename ForwardContainer>
    Graph(size_t vertexCount, const ForwardContainer& edges, bool directed = true)
    : Graph(vertexCount, edges.begin(), edges.end(), directed) {
        
    }
    
    template<typename ForwardIterator>
    Graph(size_t vertexCount, ForwardIterator begin, ForwardIterator end, bool directed = true)
    : weights_(vertexCount, RandomAccessContainer<Weight>(vertexCount, haveNotPath))
    , parent_(vertexCount, RandomAccessContainer<size_t>(vertexCount, neighbors)) {
        for(; begin != end; ++begin) {
            weights_[begin->from][begin->to] = begin->weight;
            
            if(!directed) {
                weights_[begin->to][begin->from] = begin->weight;
            }
        }
        
        for(size_t i = 0; i < vertexCount; ++i) {
            weights_[i][i] = 0;
        }
        
        for(size_t k = 0; k < vertexCount; ++k) {
            for(size_t i = 0; i < vertexCount; ++i) {
                for(size_t j = 0; j < vertexCount; ++j) {
                    
                    if(
                        weights_[i][k] == haveNotPath ||
                        weights_[k][j] == haveNotPath) {
                        continue;
                    }
                    auto throwK = weights_[i][k] + weights_[k][j];
                    if(throwK < weights_[i][j]) {
                        weights_[i][j] = throwK;
                        parent_[i][j] = k;
                    }
                }
            }
        }
    }
    
    Optional<Path> getPath(size_t from, size_t to) {
        if(weights_[from][to] == haveNotPath) {
            return {};
        }
        if(from == to) {
            return Path{weights_[from][to], {from}};
        }
        
        return Path{weights_[from][to], path(from, to)};
    }
    
    private:
    const size_t neighbors = std::numeric_limits<size_t>::max();
    const Weight haveNotPath = std::numeric_limits<Weight>::max();
    
    RandomAccessContainer<RandomAccessContainer<Weight>> weights_;
    RandomAccessContainer<RandomAccessContainer<size_t>> parent_;
    
    RandomAccessContainer<size_t> path(size_t from, size_t to) {
        
        if(parent_[from][to] == neighbors) {
            return {from, to};
        }
        auto left = path(from, parent_[from][to]);
        auto right = path(parent_[from][to], to);
        for(auto it = right.begin() + 1; it != right.end(); ++it) {
            left.push_back(*it);
        }
        return left;
    }
};

}//namespace ivv