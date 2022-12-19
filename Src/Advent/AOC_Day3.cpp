#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <sstream>

#include "AOC_Day3.h"

namespace Day3 {

   int Value(char c) {
      if (c >= 'a' && c <= 'z') return 1 + c - 'a';
      else if (c >= 'A' && c <= 'Z') return 27 + c - 'A';
      else throw std::runtime_error("undefined input");
      }

   int analyze_row(std::string const& strRow) {
      if (strRow.length() % 2 == 1) throw std::runtime_error("wrong length for input");
      size_t iPartLen = strRow.length() / 2;
      std::string strPart1 = strRow.substr(0u, iPartLen);
      std::string strPart2 = strRow.substr(iPartLen, iPartLen);

      size_t pos = 0;
      int iSumme = 0;
      if((pos = strPart1.find_first_of(strPart2, pos)) != std::string::npos) {
         iSumme += Value(strPart1[pos++]);
         }
      return iSumme;
      }

   void ReadValues_1(std::string const& strFilename) {
      std::ifstream ifs;
      ifs.open(strFilename);
      if (!ifs.is_open()) {
         std::cerr << "File " << strFilename << " couldn't opened.\n";
         return;
         }
      else {
         std::string strRow;
         int iSumme = 0;
         while (std::getline(ifs, strRow)) {
            iSumme += analyze_row(strRow);
            }
         std::cout << "Day 3/1: Sum is " << iSumme << "\n";
         }
      }


   void ReadValues_2(std::string const& strFilename) {
      std::ifstream ifs;
      ifs.open(strFilename);
      if (!ifs.is_open()) {
         std::cerr << "File " << strFilename << " couldn't opened.\n";
         return;
         }
      else {
         std::string strRow;
         std::vector<std::string> lines;
         while (std::getline(ifs, strRow)) {
            std::sort(strRow.begin(), strRow.end());
            lines.emplace_back(strRow);
            }
         if (lines.size() % 3 != 0) throw std::runtime_error("wrong count of back bags, grouping impossible");
         else {
            int iSumme = 0;
            for (size_t i = 0; i < lines.size(); i += 3) {
               std::set<char> results, results2;
               std::set_intersection(lines[i].begin(), lines[i].end(), lines[i+1].begin(), lines[i+1].end(), std::inserter(results, results.begin()));
               std::set_intersection(results.begin(), results.end(), lines[i+2].begin(), lines[i+2].end(), std::inserter(results2, results2.begin()));
               if (results2.size() != 1) throw std::runtime_error("there isn't a unique char in this back bag");
               iSumme += Value(*results2.begin());
               } 
            std::cout << "Day 3/2: Sum is " << iSumme << "\n";
            }
      }
   }

}