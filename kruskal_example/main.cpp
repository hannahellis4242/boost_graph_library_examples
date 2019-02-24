#include <fstream>
#include <iostream>

#include "boost/bind.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graphviz.hpp"
#include "boost/graph/kruskal_min_spanning_tree.hpp"

namespace example {
class EdgeType {
private:
  unsigned int weight_;
  std::string name_;

public:
  EdgeType() : weight_(0), name_("") {}
  EdgeType(const std::string &name, const unsigned int weight)
      : weight_(weight), name_(name) {}
  const std::string &name() const { return name_; }
  unsigned int weight() const { return weight_; }
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              std::string, EdgeType>
    Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Graph>::edge_descriptor edge_descriptor;

class GraphWriter {
private:
  const Graph &graph_;

public:
  GraphWriter(const Graph &graph) : graph_(graph) {}
  void operator()(std::ostream &) const {}
  void operator()(std::ostream &os, const vertex_descriptor v) const {
    os << "[label=\"" << graph_[v] << "\"]";
  }
  void operator()(std::ostream &os, const edge_descriptor e) const {
    const EdgeType &edgeData = graph_[e];
    os << "[len=2,label=\"" << edgeData.name() << " , " << edgeData.weight()
       << "\"]";
  }
};

Graph build_graph() {
  Graph graph;

  std::vector<vertex_descriptor> vertices;
  {
    char label = 'A';
    for (int i = 0; i < 6; ++i) {
      const vertex_descriptor v = boost::add_vertex(graph);
      graph[v] = std::string(1, label++);
      vertices.push_back(v);
    }
  }

  // adding edges
  {
    {
      const edge_descriptor e =
          boost::add_edge(vertices[0], vertices[1], graph).first;
      graph[e] = EdgeType("a", 7);
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[0], vertices[2], graph).first;
      graph[e] = EdgeType("g", 9);
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[0], vertices[5], graph).first;
      graph[e] = EdgeType("e", 14);
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[1], vertices[2], graph).first;
      graph[e] = EdgeType("i", 10);
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[1], vertices[3], graph).first;
      graph[e] = EdgeType("b", 15);
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[2], vertices[3], graph).first;
      graph[e] = EdgeType("h", 11);
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[2], vertices[5], graph).first;
      graph[e] = EdgeType("f", 2);
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[3], vertices[4], graph).first;
      graph[e] = EdgeType("c", 6);
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[4], vertices[5], graph).first;
      graph[e] = EdgeType("d", 9);
    }
  }

  return graph;
}

class SpanningTreeWriter {
private:
  const Graph &graph_;
  const std::set<edge_descriptor> edges_;

public:
  SpanningTreeWriter(const Graph &graph,
                     const std::vector<edge_descriptor> spanning_tree)
      : graph_(graph), edges_(spanning_tree.begin(), spanning_tree.end()) {}
  void operator()(std::ostream &) const {}
  void operator()(std::ostream &os, const vertex_descriptor v) const {
    os << "[label=\"" << graph_[v] << "\"]";
  }
  void operator()(std::ostream &os, const edge_descriptor e) const {
    const EdgeType &edgeData = graph_[e];
    if (edges_.count(e)) {
      os << "[len=2,color=\"red\",label=\"" << edgeData.name() << " , "
         << edgeData.weight() << "\"]";
    } else {
      os << "[len=2,color=\"black\",label=\"" << edgeData.name() << " , "
         << edgeData.weight() << "\"]";
    }
  }
};

void run() {
  const Graph graph = build_graph();
  // dump to dot so we can see it
  {
    GraphWriter writer(graph);
    std::ofstream f("example.dot");
    boost::write_graphviz(f, graph, writer, writer, writer);
    f.close();
  }
  // outputs
  std::vector<edge_descriptor> edges;

  boost::kruskal_minimum_spanning_tree(
      graph, std::back_inserter(edges),
      boost::weight_map(boost::make_transform_value_property_map(
          boost::bind(&EdgeType::weight, _1),
          boost::get(boost::edge_bundle, graph))));

  std::cout << std::string(5, '-') << "example_with_class_as_edge_type"
            << std::string(5, '-') << std::endl;
  for (std::vector<edge_descriptor>::iterator it = edges.begin(),
                                              endIt = edges.end();
       it != endIt; ++it) {
    const edge_descriptor edge = *it;
    const EdgeType &data = graph[edge];
    std::cout << "edge : " << data.name() << " , weight : " << data.weight()
              << std::endl;
    std::cout << std::endl;
  }
  std::cout << std::string(5, '=') << "example_with_class_as_edge_type"
            << std::string(5, '=') << std::endl;
  // dump the spanning tree
  {
    SpanningTreeWriter writer(graph, edges);
    std::ofstream f("spanning_tree.dot");
    boost::write_graphviz(f, graph, writer, writer, writer);
    f.close();
  }
}
} // namespace example

int main() {
  example::run();
  return 0;
}
