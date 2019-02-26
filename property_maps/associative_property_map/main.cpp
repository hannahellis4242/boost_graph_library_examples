#include "boost/property_map/property_map.hpp"
#include <iostream>
#include <map>

int main() {
  typedef std::map<std::string, std::string> Map;
  typedef boost::associative_property_map<Map> PropertyMap;
  // need a map to store things in
  Map m;

  // property_map needs to take the map as an argument of it's constructor so it
  // can modifiy it
  PropertyMap prop_map(m);
  // can use square brackets to set and get values
  prop_map["Alice"] = "Blue";
  // can use boost::put to set values
  boost::put(prop_map, "Bob", "Red");
  // can use boost::get to get values
  std::cout << "boost::get(prop_map,\"Alice\") = "
            << boost::get(prop_map, "Alice") << std::endl;
  std::cout << "boost::get(prop_map,\"Bob\") = " << boost::get(prop_map, "Bob")
            << std::endl;
  std::cout << "boost::get(prop_map,\"Charlie\") = "
            << boost::get(prop_map, "Charlie") << std::endl;
  return 0;
}
