#include "boost/property_map/shared_array_property_map.hpp"
#include <iostream>

int main() {
  typedef boost::typed_identity_property_map<int>
      OffsetPropMap; // property_map map that maps from the index type to an
                     // iterator offset
  boost::shared_array_property_map<std::string, OffsetPropMap> prop_map(5);
  // can use square brackets to set and get values
  prop_map[4] = "Alice";
  // can use boost::put to set values
  boost::put(prop_map, 1, "Bob");
  // can put values past the end of the array
  boost::put(prop_map, 6, "Charlie");
  // can use boost::get to get values
  for (int i = 0; i < 10; ++i) {
    std::cout << "boost::get(prop_map," << i
              << ") = " << boost::get(prop_map, i) << std::endl;
  }
  return 0;
}
