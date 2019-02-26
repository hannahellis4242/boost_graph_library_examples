#include "boost/property_map/property_map.hpp"
#include <iostream>
#include <map>

int main() {
  // property_map needs a value to reference
  int value;
  boost::ref_property_map<int, int> prop_map(value);
  // can use square brackets to set and get values
  prop_map[0] = 1;
  std::cout << "value of reference type now : " << value << std::endl;
  // can use boost::put to set values
  boost::put(prop_map, 1, 2);
  std::cout << "value of reference type now : " << value << std::endl;
  // but can use boost::get to get values
  std::cout << "boost::get(prop_map,0) = " << boost::get(prop_map, 0)
            << std::endl;
  std::cout << "boost::get(prop_map,1) = " << boost::get(prop_map, 1)
            << std::endl;
  std::cout << "boost::get(prop_map,2) = " << boost::get(prop_map, 2)
            << std::endl;
  return 0;
}
