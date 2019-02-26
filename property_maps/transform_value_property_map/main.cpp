#include "boost/property_map/property_map.hpp"
#include "boost/property_map/transform_value_property_map.hpp"
#include <iostream>
#include <map>

enum colour { red, orange, yellow, green, blue, purple };

std::string showColour(const colour c) {
  switch (c) {
  case red:
    return "red";
  case orange:
    return "orange";
  case yellow:
    return "yellow";
  case green:
    return "green";
  case blue:
    return "blue";
  case purple:
    return "purple";
  }
}

int main() {
  // setting up an associative_property_map
  typedef std::map<std::string, colour> Map;
  typedef boost::associative_property_map<Map> PropertyMap;
  Map m;
  PropertyMap prop_map(m);
  boost::put(prop_map, "Alice", blue);
  boost::put(prop_map, "Bob", red);
  boost::put(prop_map, "Charlie", green);

  // transform_value_property_map takes another property_map (can be any, just
  // using an associative_property_map here for an example),and applys the given
  // unary function to the result of the first property map.
  typedef std::string &UnaryFunctionType(const colour);
  typedef std::string FunctionResultType;
  boost::transform_value_property_map<UnaryFunctionType, PropertyMap,
                                      FunctionResultType>
      tfm_prop_map(showColour, prop_map);
  std::cout << "boost::get(prop_map,\"Bob\") = " << boost::get(prop_map, "Bob")
            << std::endl;
  std::cout << "boost::get(tfm_prop_map,\"Bob\") = "
            << boost::get(tfm_prop_map, "Bob") << std::endl;

  return 0;
}
