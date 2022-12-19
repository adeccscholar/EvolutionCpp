#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>

#include "AOC_Day4.h"
#include "AOC_Read.h"

namespace Day4 {

   void ReadValues_1(std::string const& strFilename) {
      auto ifs = input(strFilename);
      int iSumme = 0;
      std::pair<int, int> Data1, Data2;
      do {
         char c1, c2, c3;
         ifs >> Data1.first >> c1 >> Data1.second >> c2 >> Data2.first >> c3 >> Data2.second;
         if (Data1.first <= Data2.first && Data1.second >= Data2.second || 
             Data2.first <= Data1.first && Data2.second >= Data1.second) ++iSumme;
         } 
      while (!ifs.eof());
      std::cout << "Day 4/1: " << iSumme << "\n";
      }

   void ReadValues_2(std::string const& strFilename) {
      auto ifs = input(strFilename);
      int iSumme = 0;
      std::pair<int, int> Data1, Data2;
      do {
         char c1, c2, c3;
         ifs >> Data1.first >> c1 >> Data1.second >> c2 >> Data2.first >> c3 >> Data2.second;
         if (Data1.first >= Data2.first && Data1.first <= Data2.second) ++iSumme;
         else if (Data2.first >= Data1.first && Data2.first <= Data1.second) ++iSumme;
         } 
      while (!ifs.eof());
      std::cout << "Day 4/1: " << iSumme << "\n";
      }

}