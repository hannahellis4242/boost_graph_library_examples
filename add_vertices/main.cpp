#include <iostream>

#include "boost/foreach.hpp"
#include "boost/graph/adjacency_list.hpp"

class MyVertex {
private:
  std::string name_;

public:
  MyVertex() {}
  MyVertex(const std::string &name) : name_(name) {}
  const std::string name() const { return name_; }
};

typedef boost::adjacency_list<boost::vecS, // out edge list use std::vector for
                                           // storage
                              boost::vecS, // vertex list use std::vector
                              boost::undirectedS, // undirected graph
                              MyVertex // vertex type is the MyVertex type
                              >
    Graph;
// other useful typedefs to have around
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits<Graph>::edge_descriptor edge_descriptor;

int main() {
  Graph graph;

  // to add a vertex, call boost::add_vertex which will give you back a
  // vertex_descriptor for the vertex you have just added
  const vertex_descriptor v0 = boost::add_vertex(graph);
  // can now use the descriptor to access the vertex in the graph using []
  graph[v0] = MyVertex("Alice");

  // boost::num_vertices tells you how many vertices there are in the graph
  std::cout << "added " << boost::num_vertices(graph) << " to the graph"
            << std::endl;

  // going to add a few extra vertices
  {
    const vertex_descriptor v = boost::add_vertex(graph);
    graph[v] = MyVertex("Bob");
  }
  {
    const vertex_descriptor v = boost::add_vertex(graph);
    graph[v] = MyVertex("Charlie");
  }
  // we can iterate though the vertices by using boost::num_vertices
  BOOST_FOREACH (const vertex_descriptor v, boost::vertices(graph)) {
    // again we can access the vertex by using [] and the vertex_descriptor
    std::cout << "vertex " << v << " has name " << graph[v].name() << std::endl;
  }

  return 0;
}
