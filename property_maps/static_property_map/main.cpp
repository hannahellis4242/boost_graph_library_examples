#include "boost/property_map/property_map.hpp"
#include <iostream>
#include <map>

int main() {
  // property_map needs a value to return
  boost::static_property_map<int> prop_map(42);
  // static_property_map is only readable and not writable, so
  // can't use square brackets to set and get values
  // prop_map[0] = 1;
  // can't use boost::put to set values
  // boost::put(prop_map, 1, 2);
  // but can use boost::get to get values
  std::cout << "boost::get(prop_map,0) = " << boost::get(prop_map, 0)
            << std::endl;
  std::cout << "boost::get(prop_map,1) = " << boost::get(prop_map, 1)
            << std::endl;
  std::cout << "boost::get(prop_map,2) = " << boost::get(prop_map, 2)
            << std::endl;
  // can also pass any key type you like, won't make any difference
  std::cout << "boost::get(prop_map,\"Alice\") = "
            << boost::get(prop_map, "Alice") << std::endl;
  return 0;
}
