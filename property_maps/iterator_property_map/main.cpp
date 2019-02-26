#include "boost/property_map/property_map.hpp"
#include <iostream>

int main() {
  std::vector<std::string> xs;
  {
    xs.push_back("Alice");
    xs.push_back("Bob");
    xs.push_back("Charlie");
    xs.push_back("Dave");
    xs.push_back("Edward");
  }

  typedef std::vector<std::string>::iterator Iterator; // iterator type
  typedef boost::typed_identity_property_map<int>
      OffsetMap; // property_map map that maps from the index type to an
  // iterator offset
  typedef std::string ValueType;
  typedef std::string &ReferenceType;
  boost::iterator_property_map<Iterator, OffsetMap, ValueType, ReferenceType>
      prop_map(xs.begin());
  // can use square brackets to set and get values
  prop_map[2] = "Fred";
  // can use boost::put to set values
  boost::put(prop_map, 1, "Gemma");
  // even values past the end of the vector. However these will never get added
  // to the underlying container
  boost::put(prop_map, 7, "Harry");
  // can use boost::get to get values, anything past the end iterator will
  // just
  // give the default constructed type
  for (int i = 0; i < 10; ++i) {
    std::cout << "boost::get(prop_map," << i
              << ") = " << boost::get(prop_map, i) << std::endl;
  }
  std::cout << "underlying vector: " << std::endl;
  for (std::vector<std::string>::const_iterator it = xs.begin(),
                                                endIt = xs.end();
       it != endIt; ++it) {
    std::cout << '\t' << *it << std::endl;
  }
  return 0;
}
