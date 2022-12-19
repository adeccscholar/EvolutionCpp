#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include <optional>
#include <algorithm>
#include <regex>
#include <chrono>
#include "AOC_Day16.h"
#include "AOC_Read.h"

using namespace std::literals::string_literals;

namespace Day16 {

   // first percent value between 0 and 100, second vector with destinations
   // eventuell tuple statt pair, um Erweiterbarkeit zu erleichtern
   using distance_value = std::optional<std::pair<size_t, size_t>>;
   using distance_data  = std::map<std::string, distance_value>;
   using valve_data     = std::tuple<size_t, std::vector<std::string>, distance_data>;
   using valves_data    = std::map<std::string, valve_data>;

   // control during calculations, opened valve and time
   using workflow_data = std::map<std::string, std::tuple<size_t, size_t>>;

  // using distances_grid = std::map<std::pair<std::string, std::string>, std::optional<std::pair<size_t, size_t>>>;

   std::ostream& operator << (std::ostream& out, valves_data const& data) {
      for (auto const& line : data) {
         out << line.first << " with " << std::get<0>(line.second) << "% and targets ";
         for (size_t i = 0u; auto const& v : std::get<1>(line.second)) out << (i++ ? ", " : "") << v;
         out << " -> " << std::get<2>(line.second).size() << "\n";
         }
      return out;
      }

   std::ostream& operator << (std::ostream& out, distance_data::value_type const& data) {
      if (data.second)
         out << " - " << data.first << " in " << data.second->first << " with " << data.second->second << "%";
      return out;
      }

   std::ostream& operator << (std::ostream& out, distance_data const& data) {
      std::for_each(data.begin(), data.end(), [&out](auto const& val) { out << val << "\n"; });
      return out;
      }

   std::pair<std::string, valve_data> Parse(std::string const& data) {
      static const std::regex parser_input("^(\\bValve\\b)\\s*([A-Z]{2})\\s*(\\bhas flow rate\\b)\\s*=\\s*([0-9]{1,3})"
         ";\\s*[\\btunnels lead to valves\\b | \\btunnel leads to valve\\b]\\s*(.{1,80})$");
      static const std::regex parser_valves("[A-Z]{2}");

      static const std::string strFmtValve   = "$2"; // Valve
      static const std::string strFmtPercent = "$4"; // Percent
      static const std::string strFmtTargets = "$5"; // list of neighbors
      std::pair<std::string, valve_data> result;

      if (std::regex_match(data, parser_input)) {
         std::string strValve   = std::regex_replace(data, parser_input, strFmtValve);
         std::string strPercent = std::regex_replace(data, parser_input, strFmtPercent);
         std::string strTargets = std::regex_replace(data, parser_input, strFmtTargets);

         size_t err_pos;
         size_t iValue = stoi(strPercent, &err_pos);
         if (err_pos < strPercent.length()) throw std::runtime_error("error reading percent in "s + data);
         else {
            std::vector<std::string> vecTargets;
            std::sregex_token_iterator it(strTargets.begin(), strTargets.end(), parser_valves);
            std::sregex_token_iterator end;
            for (; it != end; ++it) {
               vecTargets.push_back(it->str());
               }
            result = { strValve, { iValue, vecTargets, { } } };
            }
         }
      else {
         throw std::runtime_error("unexpected row in inputfile"s + data);
         }
      return result;
      } 

   // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
   // Valve OZ has flow rate = 17; tunnels lead to valves UB, HK, JM, ZK, ER
   valves_data ReadInput(std::string const& strFilename) {
      valves_data retval;
      auto ifs = input(strFilename);
      std::string strRow;
      while(std::getline(ifs, strRow)) {
         retval.insert(Parse(strRow));
         }
      return retval;
      }


   // rekursiver Aufbau der Entfernungen
   void Build(valves_data& data, std::string const& start, std::vector<std::string> const& targets, size_t dept, distance_data& distances) {
      if (dept == 1) {
         // initialize the distances
         for (auto const& [valve, other] : data)
            if (valve != start) distances.insert({ valve, std::nullopt });

         if (auto elem = data.find(start); elem != data.end()) {
            for (auto const& target : targets) { 
               if(auto child = data.find(target); child != data.end()) {
                  distances[target] = { dept, std::get<0>(child->second) };
                  Build(data, start, std::get<1>(child->second), dept + 1, distances);
                  }
               }
            }
         else throw std::runtime_error("internal error, element for item "s + start + " not found");
         }
      else {
         for (auto& target : targets) {
            if(target != start && (!distances[target] || distances[target]->first > dept)) {
               if (auto child = data.find(target); child != data.end()) {
                  distances[target] = { dept, std::get<0>(child->second) };
                  Build(data, start, std::get<1>(child->second), dept + 1, distances);
                  }
               }
            }
         }
      };


   // erase_if C++20 und mehr like range
   void Clean(valves_data& data) {
      std::for_each(data.begin(), data.end(), [](auto& val) mutable {
         std::erase_if(std::get<2>(val.second), [](auto const& elem) {
            auto const& [key, value] = elem;
            return value ? value->second == 0 : true;
            });
         });
      }

   void Build(valves_data& data) {
      auto func_start = std::chrono::high_resolution_clock::now();
      for (auto& elem : data) {
         Build(data, elem.first, std::get<1>(elem.second), 1, std::get<2>(elem.second));
         }
      Clean(data);
      auto func_ende = std::chrono::high_resolution_clock::now();
      auto time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);
      std::cout << "Build in " << time.count() / 1000. << " sec\n";

      //std::cout << data << "\n\n";
      }


   size_t Evaluate(size_t max_time, workflow_data& workflow, valves_data const& data) {
      size_t retval = 0u;
      std::for_each(workflow.begin(), workflow.end(), [&retval, &max_time, &data](auto const& val) {
            if(auto const& time = std::get<0>(val.second); time < max_time) {
               if (auto const& value = std::get<1>(val.second); value > 0) {
                  retval += (max_time - time) * value;
                  }
               }
            });
      return retval;
      }

   std::optional<size_t> MakeStep(std::string const& target, workflow_data workflow, size_t max_time, size_t next_time, valves_data const& data) {
      if (next_time >= max_time) return Evaluate(max_time, workflow, data);
      if (auto step = data.find(target); step != data.end()) {   // open only if effect opening need 1 second
         if (auto const& value = std::get<0>(step->second); value > 0) {
            ++next_time;
            workflow.insert({ target, { next_time, value } });
         }
         std::optional<size_t> max_value = Evaluate(max_time, workflow, data);
         for (auto const& [next, value_next] : std::get<2>(step->second)) {
            if(auto cntrl = workflow.find(next); cntrl == workflow.end()) {  // player 2, find_if
               if (value_next && (max_time > next_time + value_next->first) && value_next->second > 0) {
                  auto val = MakeStep(next, workflow, max_time, next_time + value_next->first, data);
                  if (val && *val > max_value) {
                     max_value = val;
                     }
                  }
               }
            }
         return max_value;
         }
      else throw std::runtime_error("Critical error: item with "s + target + " didn't found in data!");
      }

   void Targets(valves_data const& data, std::string const& target) {
      if (auto elem = data.find(target); elem != data.end()) {
         auto const& [key, value] = *elem;
         std::cout << "Target \"" << key << "\" with " << std::get<0>(value) << "%\n"
                   << "new targets are:\n"
                   << std::get<2>(value) << "\n";
         }
      else throw std::runtime_error("Couldn't find target "s + target + " in valves data");
   }

   void ReadValues_1(std::string const& strFilename) {
      auto func_start = std::chrono::high_resolution_clock::now();
      valves_data data = ReadInput(strFilename);
      auto func_ende = std::chrono::high_resolution_clock::now();
      auto time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);
      std::cout << "Read in " << time.count() / 1000. << " sec\n";

      Build(data);
      //Targets(data, "AA"s);
      //std::cout << "\n\n";

      func_start = std::chrono::high_resolution_clock::now();
      auto value = MakeStep("AA"s, { }, 30, 0, data);
      func_ende = std::chrono::high_resolution_clock::now();
      time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);
      if (value) std::cout << "Day 16/1: " << *value << " in " << time.count() / 1000. << " sec\n";
      else std::cout << "no value\n";
      }

   void ReadValues_2(std::string const& strFilename) {
   }
}
