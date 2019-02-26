#include "boost/property_map/vector_property_map.hpp"
#include <iostream>

int main() {
  boost::vector_property_map<int> prop_map;
  //can use square brackets to set and get values
  prop_map[5] = 42;
  //can use boost::put to set values
  boost::put(prop_map, 13, 7);
  //can use boost::get to get values
  std::cout << "boost::get(prop_map,5) = " << boost::get(prop_map, 5)
            << std::endl;
  std::cout << "boost::get(prop_map,13) = " << boost::get(prop_map, 13)
            << std::endl;
  std::cout << "boost::get(prop_map,27) = " << boost::get(prop_map, 27)
            << std::endl;
  return 0;
}
