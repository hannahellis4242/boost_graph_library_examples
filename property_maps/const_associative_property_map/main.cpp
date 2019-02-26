#include "boost/property_map/property_map.hpp"
#include <iostream>
#include <map>

int main() {
  typedef std::map<std::string, std::string> Map;
  typedef boost::const_associative_property_map<Map> PropertyMap;
  // need a map to store things in
  Map m;
  m.insert(std::make_pair("Hannah", "Purple"));
  // property_map needs to take the map as an argument of it's constructor
  PropertyMap prop_map(m);
  // const_associative_property_map is only readable and not writable, so
  // can't use square brackets to set and get values
  // prop_map["Alice"] = "Blue";
  // can't use boost::put to set values
  // boost::put(prop_map, "Bob", "Red");
  // can use boost::get to get values but only if the key already exists
  /*std::cout << "boost::get(prop_map,\"Alice\") = "
            << boost::get(prop_map, "Alice") << std::endl;*/
  std::cout << "boost::get(prop_map,\"Hannah\") = "
            << boost::get(prop_map, "Hannah") << std::endl;
  return 0;
}
