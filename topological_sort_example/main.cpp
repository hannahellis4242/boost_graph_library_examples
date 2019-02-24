#include <fstream>
#include <iostream>

#include "boost/bind.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graphviz.hpp"
#include "boost/graph/topological_sort.hpp"

namespace example {
class Job {
private:
  unsigned int id_;

public:
  Job() : id_(0) {}
  Job(const unsigned int id) : id_(id) {}
  unsigned int id() const { return id_; }
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Job>
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
    const Job &job = graph_[v];
    os << "[label=\"" << job.id() << "\"]";
  }
  void operator()(std::ostream &os, const edge_descriptor) const {
    os << "[len=2]";
  }
};

Graph build_graph() {
  Graph graph;

  std::vector<vertex_descriptor> vertices;
  {
    for (int i = 0; i < 9; ++i) {
      const vertex_descriptor v = boost::add_vertex(graph);
      graph[v] = Job(i);
      vertices.push_back(v);
    }
  }

  // adding edges
  {
    boost::add_edge(vertices[0], vertices[4], graph);
    boost::add_edge(vertices[0], vertices[5], graph);
    boost::add_edge(vertices[0], vertices[7], graph);
    boost::add_edge(vertices[0], vertices[8], graph);

    boost::add_edge(vertices[1], vertices[2], graph);
    boost::add_edge(vertices[1], vertices[4], graph);

    boost::add_edge(vertices[2], vertices[4], graph);
    boost::add_edge(vertices[2], vertices[6], graph);

    boost::add_edge(vertices[3], vertices[5], graph);

    boost::add_edge(vertices[5], vertices[7], graph);

    boost::add_edge(vertices[8], vertices[7], graph);
  }

  return graph;
}

void run() {
  const Graph graph = build_graph();
  // dump to dot so we can see it
  {
    GraphWriter writer(graph);
    std::ofstream f("example.dot");
    boost::write_graphviz(f, graph, writer, writer, writer);
    f.close();
  }

  // output
  std::vector<vertex_descriptor> sorted_vertices;
  boost::topological_sort(graph, std::back_inserter(sorted_vertices));

  for (std::vector<vertex_descriptor>::iterator it = sorted_vertices.begin(),
                                                endIt = sorted_vertices.end();
       it != endIt; ++it) {
    const Job &job = graph[*it];
    std::cout << job.id() << std::endl;
  }
}
} // namespace example

int main() {
  example::run();
  return 0;
}
