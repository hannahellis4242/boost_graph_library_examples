#include "boost/property_map/property_map.hpp"
#include <iostream>

int main() {
  boost::typed_identity_property_map<double> prop_map;
  // typed_identity_property_map is readable but not writable so
  // can't use square brackets to set and get values
  // prop_map[5.2] = 42.0;
  // can't use boost::put to set values
  // boost::put(prop_map, 1.3, 7.4);
  // can use boost::get to get values
  std::cout << "boost::get(prop_map,5.2) = " << boost::get(prop_map, 5.2)
            << std::endl;
  std::cout << "boost::get(prop_map,1.3) = " << boost::get(prop_map, 1.3)
            << std::endl;
  std::cout << "boost::get(prop_map,27.0) = " << boost::get(prop_map, 27.0)
            << std::endl;
  return 0;
}
