#pragma once

#include <iostream>  // ostream
#include <utility>   // pair

/**
 * @brief Floating point data type to be used for storing the geographic position.
 * @tparam ty floating point type for the geografic location
*/
template <typename ty>
using Location = std::pair<ty, ty>;

template <typename ty>
inline std::ostream& operator << (std::ostream& out, Location<ty> const& data) {
   out << "(" << data.first << ", " << data.second << ")";
   return out;
}


template< typename ty>
inline void Calculate(Location<ty> const& a, Location<ty> const& b) {
   std::cout << a << " -> " << b << "\n";
}
