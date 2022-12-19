
#include <iostream>
#include <regex>
#include <tuple>
#include <stack>
#include <map>
#include <variant>
#include "AOC_Day7.h"
#include "AOC_Read.h"

using namespace std::literals::string_literals;

namespace Day7 {
   using stack = std::stack<size_t>;                       
   using data  = std::vector< size_t>;

   void ReadDirectory(std::string const& strFilename, int iVariante = 1) {
      static const std::regex parser_command("^(\\$) (.{1,3})( (.{1,50}))?");
      static const std::regex parser_content("^(.{1,50}) (.{1,50})");
      static const std::string fmt1("$1");
      static const std::string fmt2("$2");
      static const std::string fmt4("$4");
      auto ifs = input(strFilename);
      std::string strRow;

      stack workflow;
      data values;
      size_t iSumme = 0;

      while (std::getline(ifs, strRow)) {
         if(std::regex_match(strRow, parser_command)) {
            std::map<std::string, int> commands = { { "ls"s, 0 }, {"cd"s, 1} };
            std::string part2 = std::regex_replace(strRow, parser_command, fmt2);
            auto it = commands.find(part2);
            if (it == commands.end()) throw std::runtime_error("unexpected command");
            switch(it->second) {
               case 0:
                  // read mode
                  break;
               case 1:
                  {
                  std::string part4 = std::regex_replace(strRow, parser_command, fmt4);
                  if(part4 == "/"s) {
                     while(!workflow.empty()) workflow.pop();
                     workflow.push(0u);
                     } 
                  else if(part4 == ".."s) {
                     size_t v = workflow.top();
                     values.emplace_back(v);
                     //std::cout << v << "\n";
                     if (v <= 100000) iSumme += v;
                     workflow.pop();
                     workflow.top() += v;
                     }
                  else {
                     workflow.push(0u);
                    }
                  }
                  break;
               default: 
                  throw std::runtime_error("unhandled command");
               }
            }
         else {
            if(std::regex_match(strRow, parser_content)) {
               std::string part1 = std::regex_replace(strRow, parser_content, fmt1);
               std::string part2 = std::regex_replace(strRow, parser_content, fmt2);
               if (part1 != "dir"s) {
                  size_t pos;
                  workflow.top() += stoi(part1, &pos);
                  }
               }
            else {
               std::cout << "no match" << "\n";
               }
            }
         }

      while(!workflow.empty()) {
         size_t v = workflow.top();
         values.emplace_back(v);
         if (v <= 100000) iSumme += v;
         workflow.pop();
         if(!workflow.empty()) workflow.top() += v;
         }
      if(iVariante == 1) std::cout << "Day 7/1: " << iSumme << "\n";
      else {
         size_t used_size   = *std::max_element(values.begin(), values.end());  // max element is root
         size_t free_size   = 70000000u - used_size;
         size_t needed_size = 30000000u - free_size;
         // possible range and filter
         std::vector<size_t> canditates;
         std::copy_if(values.begin(), values.end(), std::back_inserter(canditates), [needed_size](auto x) { return x >= needed_size; });
         std::cout << "Day 7/2: " << *std::min_element(canditates.begin(), canditates.end()) << "\n";
         }
      }

   void ReadValues_1(std::string const& strFilename) {
      ReadDirectory(strFilename, 1);
      }
   void ReadValues_2(std::string const& strFilename) {
      ReadDirectory(strFilename, 2);
      }
   }