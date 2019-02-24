#include <fstream>
#include <iostream>
#include <sstream>

#include "boost/bind.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graphviz.hpp"
#include "boost/graph/vf2_sub_graph_iso.hpp"

namespace example_one {

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              std::string>
    Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Graph>::edge_descriptor edge_descriptor;

class GraphWriter {
private:
  const Graph &graph_;

public:
  GraphWriter(const Graph &graph) : graph_(graph) {}
  void operator()(std::ostream &os) const { os << "overlap = false;\n"; }
  void operator()(std::ostream &os, const vertex_descriptor v) const {
    os << "[label=\"" << graph_[v] << "\"]";
  }
  void operator()(std::ostream &os, const edge_descriptor) const {
    os << "[len=2]";
  }
};

Graph build_small_graph() {
  Graph graph;

  std::vector<vertex_descriptor> vertices;
  {
    char label = 'A';
    for (int i = 0; i < 5; ++i) {
      const vertex_descriptor v = boost::add_vertex(graph);
      graph[v] = std::string(1, label++);
      vertices.push_back(v);
    }
  }

  // adding edges
  {
    boost::add_edge(vertices[0], vertices[1], graph);
    boost::add_edge(vertices[0], vertices[2], graph);
    boost::add_edge(vertices[1], vertices[3], graph);
    boost::add_edge(vertices[2], vertices[3], graph);
    boost::add_edge(vertices[3], vertices[4], graph);
  }

  return graph;
}

Graph build_large_graph() {
  Graph graph;

  std::vector<vertex_descriptor> vertices;
  {
    char label = 'A';
    for (int i = 0; i < 13; ++i) {
      const vertex_descriptor v = boost::add_vertex(graph);
      graph[v] = std::string(1, label++);
      vertices.push_back(v);
    }
  }

  // adding edges
  {
    boost::add_edge(vertices[0], vertices[1], graph);
    boost::add_edge(vertices[0], vertices[2], graph);
    boost::add_edge(vertices[0], vertices[12], graph);

    boost::add_edge(vertices[1], vertices[3], graph);
    boost::add_edge(vertices[1], vertices[4], graph);

    boost::add_edge(vertices[2], vertices[6], graph);
    boost::add_edge(vertices[2], vertices[8], graph);
    boost::add_edge(vertices[2], vertices[11], graph);

    boost::add_edge(vertices[3], vertices[5], graph);

    boost::add_edge(vertices[6], vertices[7], graph);
    boost::add_edge(vertices[6], vertices[9], graph);

    boost::add_edge(vertices[9], vertices[10], graph);

    boost::add_edge(vertices[11], vertices[12], graph);
  }
  return graph;
}

class MatchCallBack {
private:
  const Graph &large_graph_;
  const Graph &small_graph_;
  unsigned int &count_;

public:
  MatchCallBack(const Graph &large_graph, const Graph &small_graph,
                unsigned int &count)
      : large_graph_(large_graph), small_graph_(small_graph), count_(count) {
    count_ = 0;
  }
  template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
  bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) {
    // CorrespondenceMap1To2 is a property map that maps a vertex in the small
    // graph to a matching vertex in the large graph CorrespondanceMap2To1 is a
    // property map that maps a vertex in the large graph to a matching vertex
    // in the small graph

    ++count_;
    typedef
        typename boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_iterator_pair;
    const vertex_iterator_pair vertexRange = boost::vertices(small_graph_);
    std::stringstream ss;
    ss << std::string(10, '-') << std::endl;
    ss << "match found : " << std::endl;
    for (vertex_iterator it = vertexRange.first, endIt = vertexRange.second;
         it != endIt; ++it) {
      const vertex_descriptor u = *it;
      const vertex_descriptor v = boost::get(f, u);
      ss << "\tsmall graph vertex " << small_graph_[u] << " matches to vertex "
         << large_graph_[v] << std::endl;
    }
    ss << std::string(10, '=') << std::endl;
    std::cout << ss.str();
    // this call back returns true if the search should continue, and false if
    // it should stop.
    return true;
  }
};

void run() {
  const Graph small_graph = build_small_graph();
  const Graph large_graph = build_large_graph();

  // print graphs
  {
    {
      GraphWriter writer(small_graph);
      std::ofstream f("small.dot");
      boost::write_graphviz(f, small_graph, writer, writer, writer);
      f.close();
    }

    {
      GraphWriter writer(large_graph);
      std::ofstream f("large.dot");
      boost::write_graphviz(f, large_graph, writer, writer, writer);
      f.close();
    }
  }

  unsigned int match_count = 0;
  MatchCallBack matchCallBack(large_graph, small_graph,
                              match_count); // callback is used for output
  const bool found =
      boost::vf2_subgraph_iso(small_graph, large_graph, matchCallBack);
  std::cout << std::string(20, '-') << std::endl;
  std::cout << std::boolalpha << "match found : " << found << std::endl;
  std::cout << "number of matches found : " << match_count << std::endl;
  std::cout << std::string(20, '=') << std::endl;
}
} // namespace example_one

namespace example_two {
enum colour_t { red, blue, green, black };
class Coloured {
protected:
  colour_t colour_;

public:
  Coloured() : colour_(black) {}
  Coloured(const colour_t colour) : colour_(colour) {}
  colour_t colour() const { return colour_; }
};
static bool operator==(const Coloured &a, const Coloured &b) {
  return a.colour() == b.colour();
}
class ColouredVertex : public Coloured {
private:
  std::string name_;

public:
  ColouredVertex() {}
  ColouredVertex(const std::string &name, const colour_t colour)
      : Coloured(colour), name_(name) {}
  const std::string &name() const { return name_; }
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              ColouredVertex, Coloured>
    Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Graph>::edge_descriptor edge_descriptor;

class GraphWriter {
private:
  const Graph &graph_;

public:
  GraphWriter(const Graph &graph) : graph_(graph) {}
  void operator()(std::ostream &os) const { os << "overlap = false;\n"; }
  void operator()(std::ostream &os, const vertex_descriptor v) const {
    const ColouredVertex &vertex = graph_[v];
    os << "["
       << "color=";
    switch (vertex.colour()) {
    case red:
      os << "red";
      break;
    case blue:
      os << "blue";
      break;
    case green:
      os << "green";
      break;
    case black:
      os << "black";
      break;
    }
    os << ",label=\"" << vertex.name() << "\"]";
  }
  void operator()(std::ostream &os, const edge_descriptor e) const {
    const Coloured &edge = graph_[e];
    os << "[len=2,color=";
    switch (edge.colour()) {
    case red:
      os << "red";
      break;
    case blue:
      os << "blue";
      break;
    case green:
      os << "green";
      break;
    case black:
      os << "black";
      break;
    }
    os << "]";
  }
};

Graph build_small_graph() {
  Graph graph;

  std::vector<vertex_descriptor> vertices;
  {{const vertex_descriptor v = boost::add_vertex(graph);
  graph[v] = ColouredVertex("A", red);
  vertices.push_back(v);
}
{
  const vertex_descriptor v = boost::add_vertex(graph);
  graph[v] = ColouredVertex("B", blue);
  vertices.push_back(v);
}
{
  const vertex_descriptor v = boost::add_vertex(graph);
  graph[v] = ColouredVertex("C", green);
  vertices.push_back(v);
}
} // namespace example_two

// adding edges
{
  {
    const edge_descriptor e =
        boost::add_edge(vertices[0], vertices[1], graph).first;
    graph[e] = Coloured(green);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[0], vertices[2], graph).first;
    graph[e] = Coloured(blue);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[1], vertices[2], graph).first;
    graph[e] = Coloured(red);
  }
}

return graph;
}

Graph build_large_graph() {
  Graph graph;

  std::vector<vertex_descriptor> vertices;
  {{const vertex_descriptor v = boost::add_vertex(graph);
  graph[v] = ColouredVertex("A", red);
  vertices.push_back(v);
}
{
  const vertex_descriptor v = boost::add_vertex(graph);
  graph[v] = ColouredVertex("B", blue);
  vertices.push_back(v);
}
{
  const vertex_descriptor v = boost::add_vertex(graph);
  graph[v] = ColouredVertex("C", green);
  vertices.push_back(v);
}
{
  const vertex_descriptor v = boost::add_vertex(graph);
  graph[v] = ColouredVertex("D", red);
  vertices.push_back(v);
}
{
  const vertex_descriptor v = boost::add_vertex(graph);
  graph[v] = ColouredVertex("E", red);
  vertices.push_back(v);
}
{
  const vertex_descriptor v = boost::add_vertex(graph);
  graph[v] = ColouredVertex("F", blue);
  vertices.push_back(v);
}
}

// adding edges
{
  {
    const edge_descriptor e =
        boost::add_edge(vertices[0], vertices[1], graph).first;
    graph[e] = Coloured(red);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[0], vertices[2], graph).first;
    graph[e] = Coloured(red);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[0], vertices[3], graph).first;
    graph[e] = Coloured(blue);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[1], vertices[2], graph).first;
    graph[e] = Coloured(blue);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[1], vertices[5], graph).first;
    graph[e] = Coloured(red);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[2], vertices[3], graph).first;
    graph[e] = Coloured(green);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[2], vertices[4], graph).first;
    graph[e] = Coloured(blue);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[2], vertices[5], graph).first;
    graph[e] = Coloured(red);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[3], vertices[4], graph).first;
    graph[e] = Coloured(green);
  }
  {
    const edge_descriptor e =
        boost::add_edge(vertices[4], vertices[5], graph).first;
    graph[e] = Coloured(green);
  }
}
return graph;
}

class MatchCallBack {
private:
  const Graph &large_graph_;
  const Graph &small_graph_;
  unsigned int &count_;

public:
  MatchCallBack(const Graph &large_graph, const Graph &small_graph,
                unsigned int &count)
      : large_graph_(large_graph), small_graph_(small_graph), count_(count) {
    count_ = 0;
  }
  template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
  bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) {
    // CorrespondenceMap1To2 is a property map that maps a vertex in the small
    // graph to a matching vertex in the large graph CorrespondanceMap2To1 is a
    // property map that maps a vertex in the large graph to a matching vertex
    // in the small graph

    ++count_;
    typedef
        typename boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_iterator_pair;
    const vertex_iterator_pair vertexRange = boost::vertices(small_graph_);
    std::stringstream ss;
    ss << std::string(10, '-') << std::endl;
    ss << "match found : " << std::endl;
    for (vertex_iterator it = vertexRange.first, endIt = vertexRange.second;
         it != endIt; ++it) {
      const vertex_descriptor u = *it;
      const vertex_descriptor v = boost::get(f, u);
      ss << "\tsmall graph vertex " << small_graph_[u].name()
         << " matches to vertex " << large_graph_[v].name() << std::endl;
    }
    ss << std::string(10, '=') << std::endl;
    std::cout << ss.str();
    // this call back returns true if the search should continue, and false if
    // it should stop.
    return true;
  }
};

class VertexAndEdgeCompairer {
private:
  const Graph &small_graph_;
  const Graph &large_graph_;

public:
  VertexAndEdgeCompairer(const Graph &small_graph, const Graph &large_graph)
      : small_graph_(small_graph), large_graph_(large_graph) {}
  bool operator()(const edge_descriptor edge_in_small,
                  const edge_descriptor edge_in_large) const {
    // edges match when they are the same colour
    const Coloured &small_colour = small_graph_[edge_in_small];
    const Coloured &large_colour = large_graph_[edge_in_large];
    return small_colour == large_colour;
  }
  bool operator()(const vertex_descriptor vertex_in_small,
                  const vertex_descriptor vertex_in_large) const {
    // vertices match when they are the same colour
    const Coloured &small_colour = small_graph_[vertex_in_small];
    const Coloured &large_colour = large_graph_[vertex_in_large];
    return small_colour == large_colour;
  }
};

void run() {
  const Graph small_graph = build_small_graph();
  const Graph large_graph = build_large_graph();

  {
    {
      GraphWriter writer(small_graph);
      std::ofstream f("small2.dot");
      boost::write_graphviz(f, small_graph, writer, writer, writer);
      f.close();
    }
    {
      GraphWriter writer(large_graph);
      std::ofstream f("large2.dot");
      boost::write_graphviz(f, large_graph, writer, writer, writer);
      f.close();
    }
  }
  VertexAndEdgeCompairer compairer(small_graph, large_graph);
  unsigned int match_count = 0;
  MatchCallBack matchCallBack(large_graph, small_graph, match_count);
  std::vector<vertex_descriptor> sub_graph_visit_order;
  {
    typedef boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_iterator_pair;
    const vertex_iterator_pair itPair = boost::vertices(small_graph);
    sub_graph_visit_order =
        std::vector<vertex_descriptor>(itPair.first, itPair.second);
  }
  const bool found =
      boost::vf2_subgraph_iso(small_graph, large_graph, matchCallBack,
                              boost::get(boost::vertex_index, small_graph),
                              boost::get(boost::vertex_index, large_graph),
                              sub_graph_visit_order, compairer, compairer);
  std::cout << std::string(20, '-') << std::endl;
  std::cout << std::boolalpha << "match found : " << found << std::endl;
  std::cout << "number of matches found : " << match_count << std::endl;
  std::cout << std::string(20, '=') << std::endl;
}
} // namespace example_two

int main() {
  example_one::run();
  example_two::run();
  return 0;
}
