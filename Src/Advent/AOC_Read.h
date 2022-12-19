#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

static constexpr auto input = [](std::string const& strFile) {
   std::ifstream ifs;
   ifs.open(strFile);
   if (!ifs.is_open()) {
      std::ostringstream os;
      os << "File " << strFile << " couldn't opened.";
      throw std::runtime_error(os.str());
   }
   return ifs;
};