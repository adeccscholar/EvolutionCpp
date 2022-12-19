#include <vector>
#include <tuple>
#include <set>
#include <map>
#include <utility>
#include <functional>
#include <algorithm>
#include <regex>
#include <cmath>

#include "AOC_Day10.h"
#include "AOC_Read.h"

using namespace std::literals::string_literals;

namespace Day10 {

   // benötigte Datentypen
   using process_ty  = std::tuple<int, std::function<void (int, std::function<void(int, int)>)>>;
   using input_ty    = std::map<std::string, process_ty>;

   // Rechenwerk, könnte auch eine eigene Klasse sein
   int iResult;
   int iCycle = 0;
   int iRegValue_X = 1;

   // Wichtige Zyklen, als Regel abbildbar, aber der Anpassparkeit ein Zahlenfeld
   std::set<int> points = { 20, 60, 100, 140, 180, 220 }; // that is, during the 20th, 60th, 100th, 140th, 180th, and 220th cycles

   auto Accumulate = [](int c, int val) {
      //std::cout << c << " " << val << "\n";
      if (points.find(c) != points.end()) iResult += c * val;
      };

   auto Draw = [](int c, int val) {
      int pos = (c - 1) % 40;
      int test = std::abs(pos - val);
      std::cout << (std::abs(pos - val) < 2 ? "#" : ".");
      if (pos == 39) std::cout << "\n";
      };

   input_ty processes = { { "addx"s, { 1, [](int value, std::function<void (int, int)> action) { 
                                                           for(auto i = 0; i < 2; ++i) {
                                                              iCycle++;
                                                              action(iCycle, iRegValue_X);
                                                              }
                                                              iRegValue_X += value;  
                                                           }  } },
                          { "noop"s, { 0, [](int value, std::function<void(int, int)> action) {
                                                           iCycle++;
                                                           action(iCycle, iRegValue_X); 
                                                           } } }
                        };

   

   void ReadValues(std::string const& strFilename, std::function<void(int, int)> action) {
      static const std::regex parser_command("^([a-z]{1,10})( ([\\+|-]?(0|([1-9][0-9]*))))?");
      static const std::string fmt1("$1");
      static const std::string fmt2("$3");

      iResult = 0;
      iCycle = 0;
      iRegValue_X = 1;

      auto ifs = input(strFilename);
      std::string strRow;
      while (std::getline(ifs, strRow)) {
         if (!std::regex_match(strRow, parser_command)) {
            throw std::runtime_error("unexpected input in this row "s + strRow);
            }
         std::string strCommand = std::regex_replace(strRow, parser_command, fmt1);
         if (auto proc = processes.find(strCommand); proc != processes.end()) {
            switch (std::get<0>(proc->second)) {
               case 0:
                  std::get<1>(proc->second)(1, action);
                  break;
               case 1:
                  {
                  size_t pos;
                  std::string strArgument = std::regex_replace(strRow, parser_command, fmt2);
                  int iArgument = stoi(strArgument, &pos);
                  if (pos != strArgument.length()) throw std::runtime_error("unexpected argument for command "s + strCommand + ": "s + strArgument);
                  else
                     std::get<1>(proc->second)(iArgument, action);
                  }
                  break;
               default: throw std::runtime_error("unexpected count of arguments for command "s + strCommand);
               }
            }
         else
            throw std::runtime_error("unexpected command; "s + strCommand);
         }

      }

   void ReadValues_1(std::string const& strFilename) {
      try {
         ReadValues(strFilename, Accumulate);
         std::cout << "Day 10/1: " << iResult << "\n";
         }
      catch(std::exception& ex) {
         std::cerr << "error: " << ex.what() << "\n";
         }
      }

   void ReadValues_2(std::string const& strFilename) {
      try {
         std::cout << "Day 10/2: \n";
         ReadValues(strFilename, Draw);
         }
      catch (std::exception& ex) {
         std::cerr << "error: " << ex.what() << "\n";
         }
      }
   }

