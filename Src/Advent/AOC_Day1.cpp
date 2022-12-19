#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>

#include "AOC_Day1.h"
#include "AOC_Read.h"

namespace Day1 {

void ReadValues_2(std::string const& strFilename) {
   std::ifstream ifs = input(strFilename);
   std::vector<std::pair<int, int>> values;
   std::string strRow;
   int iElf = 0, iSumFood = 0;
   while (std::getline(ifs, strRow)) {
      if (strRow.length() == 0) {
         values.emplace_back(std::make_pair(iElf, iSumFood));
         ++iElf;
         iSumFood = 0;
         }
      else {
         size_t pos;
         iSumFood += std::stoi(strRow, &pos);
         if (pos < strRow.length()) {
            std::cerr << "error reading row with " << strRow << "\n";
            return;
            }
         }
      }
   if (iSumFood > 0) values.emplace_back(std::make_pair(iElf, iSumFood));

   if (values.size() < 3) {
      std::cerr << "to less data to accumulate sum for top 3, size = " << values.size() << "\n";
      return;
      }
   else {
      std::sort(values.begin(), values.end(), [](auto const& lhs, auto const& rhs) { return lhs.second > rhs.second;  });
      auto iFood4Top3 = std::accumulate(values.begin(), values.begin() + 3, 0, [](int a, auto b) { return a + b.second; } );
      std::for_each(values.begin(), values.begin() + 3, [](auto val) { std::cout << val.first << ", " << val.second << "\n"; });
      std::cout << "Day 1/2:  Food for top 3 = " << iFood4Top3 << "\n";
      }
   }


void ReadValues_1(std::string const& strFilename) {
   std::ifstream ifs = input(strFilename);
   std::string strRow;
   int iElf = 0, iSumFood = 0;
   int iMaxElf = 0, iMaxSumFood = 0;
   while (std::getline(ifs, strRow)) {
      if (strRow.length() == 0) {
         if (iSumFood > iMaxSumFood) {
            iMaxSumFood = iSumFood;
            iMaxElf = iElf;
            }
         ++iElf;
         iSumFood = 0;
         }
      else {
         size_t pos;
         iSumFood += std::stoi(strRow, &pos);
         if (pos < strRow.length()) {
            std::cerr << "error reading row with " << strRow << "\n";
            return;
            }
         }
      }
   if (iSumFood > iMaxSumFood) {
      iMaxSumFood = iSumFood;
      iMaxElf = iElf;
      }

   std::cout << "Day 1/1: Elf No." << iMaxElf << " with " << iMaxSumFood << " of food\n";
   }

}