#include <iostream>

#include "boost/foreach.hpp"
#include "boost/graph/adjacency_list.hpp"

class MyEdge {
private:
  std::string name_;
  unsigned int weight_;

public:
  MyEdge() : name_(std::string()), weight_(0) {}
  MyEdge(const std::string &name, const unsigned int weight)
      : name_(name), weight_(weight) {}
  const std::string name() const { return name_; }
  unsigned int weight() const { return weight_; }
};

typedef boost::adjacency_list<boost::vecS, // out edge list use std::vector for
                                           // storage
                              boost::vecS, // vertex list use std::vector
                              boost::undirectedS, // undirected graph
                              std::string,        // vertex type is std::string
                              MyEdge // edge type is the MyEdge type
                              >
    Graph;
// other useful typedefs to have around
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Graph>::edge_descriptor edge_descriptor;

int main() {
  Graph graph;

  // going to add a few vertices
  const vertex_descriptor v0 = boost::add_vertex(graph);
  const vertex_descriptor v1 = boost::add_vertex(graph);
  const vertex_descriptor v2 = boost::add_vertex(graph);
  const vertex_descriptor v3 = boost::add_vertex(graph);
  const vertex_descriptor v4 = boost::add_vertex(graph);
  graph[v0] = "A";
  graph[v1] = "B";
  graph[v2] = "C";
  graph[v3] = "D";
  graph[v4] = "E";

  // to add an edge we call boost::add_edge( source , target , graph)
  const std::pair<edge_descriptor, bool> e = boost::add_edge(v0, v1, graph);
  // set the edge data on the edge
  graph[e.first] = MyEdge("an edge", 42);

  std::cout << "number of edges in graph : " << boost::num_edges(graph)
            << std::endl;

  // can iterate though the edges using boost::edges
  BOOST_FOREACH (const edge_descriptor edge, boost::edges(graph)) {
    const MyEdge myEdge = graph[edge];
    std::cout << myEdge.name() << " : " << myEdge.weight() << std::endl;
  }
  return 0;
}
