#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <functional>

#include "AOC_Read.h"

using namespace std::literals::string_literals;

// Kopie vorhandener Methoden aus adecc Scholar

//------------------------------------------------------------------------
std::string&& trim_left(std::string&& strVal, std::string const& strDelimiter = " "s) {
   strVal.erase(0, strVal.find_first_not_of(strDelimiter));
   return std::move(strVal);
}

//------------------------------------------------------------------------
std::string&& trim_right(std::string&& strVal, std::string const& strDelimiter = " "s) {
   strVal.erase(strVal.find_last_not_of(strDelimiter) + 1, strVal.length());
   return std::move(strVal);
}

//------------------------------------------------------------------------
std::string trim_left(std::string const& strText, std::string const& strDelimiter = " "s) {
   std::string strVal(strText);
   return trim_left(std::forward<std::string>(strVal), strDelimiter);
}

//------------------------------------------------------------------------
std::string trim_right(std::string const& strText, std::string const& strDelimiter = " "s) {
   std::string strVal(strText);
   return trim_right(std::forward<std::string>(strVal), strDelimiter);
}


int tokenize(std::string const& strInput, std::vector<int> const& grouping, std::vector<std::string>& vecData, int iSize = -1) {
   auto it = grouping.cbegin();
   int iStart = 0;
   int iCount = 0;
   vecData.clear();
   if (iSize >= 0) vecData.reserve(iSize);
   while (it != grouping.end()) {
      std::string strPart = strInput.substr(iStart, *it);
      vecData.emplace_back(std::forward<std::string>(strPart));
      iStart += *it;
      ++iCount;
      ++it;
      }
   return iCount;
   } 


namespace Day5 {

   void ReadValues(std::string const& strFilename, int iVariante) {
      auto ifs = input(strFilename);
      std::string strRow;
      std::vector<std::string> parts;
      bool boReadFirstPart = true;
      // 1st part with crates
      if (std::getline(ifs, strRow)) {
         auto len = static_cast<int>(strRow.length() + 1);
         if (len % 4) throw std::runtime_error("unexpected input, len wrong");
         else len /= 4;
         std::vector<std::stack<char>> stacks(len);
         std::vector<int> groups(len, 4);

         do {
            tokenize(strRow, groups, parts, len / 4);
            for (size_t i = 0; i < parts.size(); ++i) {
               std::string strSign = trim_left(parts[i], " ["s);
               strSign = trim_right(strSign, " ]"s);
               if (strSign.length() == 1) {
                  if (strSign[0] >= 'A' && strSign[0] <= 'Z') stacks[i].emplace(strSign[0]);
                  else {
                     boReadFirstPart = false;
                     break;
                     }
                  }
               else if(strSign.length() > 1) throw std::runtime_error("unexpected input");
               }
            } 
         while (boReadFirstPart && std::getline(ifs, strRow));

         // reverse stacks  
         for (auto& astack : stacks) {
            std::stack<char> tmp;
            tmp.swap(astack); // copy stack
            while (!tmp.empty()) {
               astack.emplace(tmp.top()); tmp.pop();
               }
            }

         std::getline(ifs, strRow); // empty row
         // 2nd part with commands
         static const std::vector<std::function<void(std::vector<std::stack<char>>&, int&, int, int)>> funcs{
                    { [](std::vector<std::stack<char>>& stacks, int& count, int from, int to) -> void {
                       for (; count > 0; --count) {
                          if (stacks[from].empty()) throw std::runtime_error("stack from is empty");
                          stacks[to].push(stacks[from].top());
                          stacks[from].pop();
                          }
                       }
                    },
                    { [](std::vector<std::stack<char>>& stacks, int& count, int from, int to) -> void {
                       std::vector<char> tmp;
                       tmp.reserve(count);
                       for (; count > 0; --count) {
                          if (stacks[from].empty()) throw std::runtime_error("stack from is empty");
                          tmp.emplace_back(stacks[from].top());
                          stacks[from].pop();
                          }
                       for (size_t i = tmp.size(); i > 0; --i) stacks[to].emplace(tmp[i - 1]);
                       }
                    }
                 };

         do {    
            std::string filler1 = ""s, filler2 = ""s, filler3 = ""s;
            int count, from, to; 
            ifs >> filler1 >> count >> filler2 >> from >> filler3 >> to;
            if (filler1 == "move"s) {
               funcs[iVariante](stacks, count, from - 1, to - 1);
               }
            } 
         while (!ifs.eof());

         std::cout << "Day 5/" << iVariante + 1 << ": ";
         for (auto& astack : stacks) std::cout << astack.top();
         std::cout << "\n";
         }
   }


   void ReadValues_1(std::string const& strFilename) {
      ReadValues(strFilename, 0);
      }
  

   void ReadValues_2(std::string const& strFilename) {
      ReadValues(strFilename, 1);
      }

}