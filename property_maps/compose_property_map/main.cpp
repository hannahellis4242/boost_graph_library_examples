#include "boost/property_map/compose_property_map.hpp"
#include "boost/property_map/vector_property_map.hpp"
#include <iostream>

int main() {
  typedef boost::vector_property_map<size_t> PropertyMap1;
  typedef boost::vector_property_map<std::string> PropertyMap2;
  PropertyMap1 prop_map1;
  {
    boost::put(prop_map1, 0, 2);
    boost::put(prop_map1, 1, 0);
    boost::put(prop_map1, 2, 0);
    boost::put(prop_map1, 3, 1);
    boost::put(prop_map1, 4, 3);
  }
  PropertyMap2 prop_map2;
  {
    boost::put(prop_map2, 0, "Sausage");
    boost::put(prop_map2, 1, "Bacon");
    boost::put(prop_map2, 2, "Egg");
    boost::put(prop_map2, 3, "Beans");
  }
  typedef boost::compose_property_map<PropertyMap2, PropertyMap1> CPM;
  CPM cpm(prop_map2, prop_map1);

  // for whatever reason, you can't use boost::get, you need just get for
  // compose_property_map
  std::cout << "get(cpm,2) = " << get(cpm, 2) << std::endl;
  // you also need to use put and not boost::put
  put(cpm, 3, "Kevin");

  return 0;
}
