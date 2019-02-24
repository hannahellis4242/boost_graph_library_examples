#include "boost/property_map/function_property_map.hpp"
#include <iostream>

namespace free_func {
int doubler(const int x) { return 2 * x; }
void run() {
  const boost::function_property_map<int (&)(const int), const int, int>
      prop_map =
          boost::make_function_property_map<const int, int, int (&)(const int)>(
              doubler);
  std::cout << std::string(10, '-') << "free_func" << std::string(10, '-')
            << std::endl;
  std::cout << boost::get(prop_map, 1) << std::endl;
  std::cout << boost::get(prop_map, 2) << std::endl;
  std::cout << boost::get(prop_map, 3) << std::endl;
  std::cout << std::string(10, '=') << "free_func" << std::string(10, '=')
            << std::endl;
}
} // namespace free_func

namespace with_functor {
class Doubler {
public:
  int operator()(const int x) const { return 2 * x; }
};
void run() {
  const boost::function_property_map<Doubler, const int, int> prop_map =
      boost::make_function_property_map<const int, int, Doubler>(Doubler());
  std::cout << std::string(10, '-') << "with_functor" << std::string(10, '-')
            << std::endl;
  std::cout << boost::get(prop_map, 1) << std::endl;
  std::cout << boost::get(prop_map, 2) << std::endl;
  std::cout << boost::get(prop_map, 3) << std::endl;
  std::cout << std::string(10, '=') << "with_functor" << std::string(10, '=')
            << std::endl;
}
} // namespace with_functor

int main() {
  free_func::run();
  with_functor::run();
  return 0;
}
