#include <iostream>
#include <vector>
#include <string_view>
#include <set>

#include "AOC_Day6.h"
#include "AOC_Read.h"

namespace Day6 {


   void Processes(int iVariante, std::string_view data, size_t len) {
      static constexpr auto contains = [](std::string_view src, char c) {
         return src.find(c) != std::string_view::npos ? true : false;
      };

      static constexpr auto to_set = [](std::string_view data, size_t from, size_t len) {
         std::set<char> test;
         for (auto c : data.substr(from, len)) test.insert(c);
         return test.size();
      };

      static constexpr auto test = [](std::string_view data, size_t len) {
         for (size_t i = 0; i < data.length() - len; ++i) {
            if (to_set(data, i, len) == len) return i + len;
         }
         return std::string_view::npos;
      };

      std::cout << "Day 6/" << iVariante << ": " << test(data, len) << "\n";
      }

   void ReadValues_1(std::string const& strFilename) {
      std::ifstream ifs = input(strFilename);
      std::string strRow;
      while(std::getline(ifs, strRow)) {
         Processes(1, { strRow.data() ,strRow.size() }, 4);
         }
      }

   void ReadValues_2(std::string const& strFilename) {
      std::ifstream ifs = input(strFilename);
      std::string strRow;
      while (std::getline(ifs, strRow)) {
         Processes(1, { strRow.data() ,strRow.size() }, 14);
         }
      }
}