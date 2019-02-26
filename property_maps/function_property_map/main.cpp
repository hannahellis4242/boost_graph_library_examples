#include "boost/property_map/function_property_map.hpp"
#include <iostream>

int triple(const int x) { return 3 * x; }
class Doubler {
public:
  int operator()(const int x) const { return 2 * x; }
};

template <class property_map> std::string run(const property_map &prop_map) {
  std::stringstream ss;
  for (int i = 0; i < 3; ++i) {
    ss << "boost::get(prop_map," << i << ") = " << boost::get(prop_map, i)
       << std::endl;
  }
  return ss.str();
}

int main() {
  // using a free function
  {
    const boost::function_property_map<int (&)(const int), const int, int>
        prop_map =
            boost::make_function_property_map<const int, int,
                                              int (&)(const int)>(triple);
    std::cout << std::string(10, '-') << "free function" << std::string(10, '-')
              << std::endl;
    std::cout << run(prop_map);
    std::cout << std::string(10, '=') << "free function" << std::string(10, '=')
              << std::endl;
  }
  // using functor
  {
    const boost::function_property_map<Doubler, const int, int> prop_map =
        boost::make_function_property_map<const int, int, Doubler>(Doubler());
    std::cout << std::string(10, '-') << "functor" << std::string(10, '-')
              << std::endl;
    std::cout << run(prop_map);
    std::cout << std::string(10, '=') << "functor" << std::string(10, '=')
              << std::endl;
  }
  return 0;
}
