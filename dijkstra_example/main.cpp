#include <fstream>
#include <iostream>

#include "boost/bind.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/dijkstra_shortest_paths.hpp"
#include "boost/graph/graphviz.hpp"

namespace example_with_class_as_edge_type {
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

std::pair<Graph, vertex_descriptor> build_graph() {
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

  return std::make_pair(graph, vertices[0]);
}

void run() {
  const std::pair<Graph, vertex_descriptor> graphAndVertex = build_graph();
  const Graph &graph = graphAndVertex.first;
  const vertex_descriptor v = graphAndVertex.second;
  // dump to dot so we can see it
  {
    GraphWriter writer(graph);
    std::ofstream f("example_with_class_as_edge_type.dot");
    boost::write_graphviz(f, graph, writer, writer, writer);
    f.close();
  }
  // outputs
  std::vector<vertex_descriptor> predecessors(boost::num_vertices(graph));
  std::vector<unsigned int> distances(boost::num_vertices(graph));

  // use dijkstra_shortest_paths
  boost::dijkstra_shortest_paths(
      graph, v,
      boost::weight_map(boost::make_transform_value_property_map(
                            boost::bind(&EdgeType::weight, _1),
                            boost::get(boost::edge_bundle, graph)))
          .distance_map(boost::make_iterator_property_map(
              distances.begin(), boost::get(boost::vertex_index, graph)))
          .predecessor_map(boost::make_iterator_property_map(
              predecessors.begin(), boost::get(boost::vertex_index, graph))));

  typedef boost::graph_traits<Graph>::vertex_iterator v_iterator;
  const std::pair<v_iterator, v_iterator> vertices = boost::vertices(graph);
  std::cout << std::string(5, '-') << "example_with_class_as_edge_type"
            << std::string(5, '-') << std::endl;
  for (v_iterator it = vertices.first, endIt = vertices.second; it != endIt;
       ++it) {
    const vertex_descriptor vertex = *it;
    const std::string &vertex_name = graph[vertex];
    const vertex_descriptor predecessor = predecessors[vertex];
    const unsigned int dist = distances[vertex];
    std::cout << "vertex : " << vertex << " : " << vertex_name << std::endl;
    std::cout << "predecessor : " << predecessor << " : " << graph[predecessor]
              << std::endl;
    std::cout << "distance : " << dist << std::endl;
    std::cout << std::endl;
  }
  std::cout << std::string(5, '=') << "example_with_class_as_edge_type"
            << std::string(5, '=') << std::endl;
}
} // namespace example_with_class_as_edge_type

namespace example_pod_edge_type {
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              std::string, unsigned int>
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
    os << "[len=2,label=\"" << graph_[e] << "\"]";
  }
};

std::pair<Graph, vertex_descriptor> build_graph() {
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
      graph[e] = 7;
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[0], vertices[2], graph).first;
      graph[e] = 9;
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[0], vertices[5], graph).first;
      graph[e] = 14;
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[1], vertices[2], graph).first;
      graph[e] = 10;
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[1], vertices[3], graph).first;
      graph[e] = 15;
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[2], vertices[3], graph).first;
      graph[e] = 11;
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[2], vertices[5], graph).first;
      graph[e] = 2;
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[3], vertices[4], graph).first;
      graph[e] = 6;
    }
    {
      const edge_descriptor e =
          boost::add_edge(vertices[4], vertices[5], graph).first;
      graph[e] = 9;
    }
  }

  return std::make_pair(graph, vertices[0]);
}

void run() {
  const std::pair<Graph, vertex_descriptor> graphAndVertex = build_graph();
  const Graph &graph = graphAndVertex.first;
  const vertex_descriptor v = graphAndVertex.second;
  // dump to dot so we can see it
  {
    GraphWriter writer(graph);
    std::ofstream f("example_pod_edge_type.dot");
    boost::write_graphviz(f, graph, writer, writer, writer);
    f.close();
  }
  // outputs
  std::vector<vertex_descriptor> predecessors(boost::num_vertices(graph));
  std::vector<unsigned int> distances(boost::num_vertices(graph));

  // use dijkstra_shortest_paths
  // property maps can be chained in any order you like it would seem.
  /*boost::dijkstra_shortest_paths(
      graph, v,
      boost::weight_map(boost::get(boost::edge_bundle, graph))
          .distance_map(boost::make_iterator_property_map(
              distances.begin(), boost::get(boost::vertex_index, graph)))
          .predecessor_map(boost::make_iterator_property_map(
              predecessors.begin(), boost::get(boost::vertex_index, graph))));*/

  boost::dijkstra_shortest_paths(
      graph, v,
      boost::distance_map(
          boost::make_iterator_property_map(
              distances.begin(), boost::get(boost::vertex_index, graph)))
          .predecessor_map(boost::make_iterator_property_map(
              predecessors.begin(), boost::get(boost::vertex_index, graph)))
          .weight_map(boost::get(boost::edge_bundle, graph)));
  typedef boost::graph_traits<Graph>::vertex_iterator v_iterator;
  const std::pair<v_iterator, v_iterator> vertices = boost::vertices(graph);
  std::cout << std::string(5, '-') << "example_pod_edge_type"
            << std::string(5, '-') << std::endl;
  for (v_iterator it = vertices.first, endIt = vertices.second; it != endIt;
       ++it) {
    const vertex_descriptor vertex = *it;
    const std::string &vertex_name = graph[vertex];
    const vertex_descriptor predecessor = predecessors[vertex];
    const unsigned int dist = distances[vertex];
    std::cout << "vertex : " << vertex << " : " << vertex_name << std::endl;
    std::cout << "predecessor : " << predecessor << " : " << graph[predecessor]
              << std::endl;
    std::cout << "distance : " << dist << std::endl;
    std::cout << std::endl;
  }
  std::cout << std::string(5, '=') << "example_pod_edge_type"
            << std::string(5, '=') << std::endl;
}
} // namespace example_pod_edge_type

int main() {
  example_with_class_as_edge_type::run();
  example_pod_edge_type::run();
  return 0;
}
