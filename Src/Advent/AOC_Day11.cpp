#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
#include <functional>
#include <regex>
#include <optional>

#include "AOC_Day11.h"
#include "AOC_Read.h"

using namespace std::literals::string_literals;

namespace Day11 {

   // ----- definition for the monkey class
   class TMonkey {
      friend std::ostream& operator << (std::ostream& out, TMonkey const& data);
   public:
      enum class EOperations : char {
         Addition       = '+',
         Multiplication = '*',
         Division       = '/',
         Substraction   = '-',
         Modulo         = '%',
         undefined
         };

      using operation_ty = std::map<EOperations, std::function<size_t(size_t, size_t )>>;
      using monkeys_ty   = std::map<size_t, TMonkey>;
   private:
      size_t iID = 0;
      std::vector<size_t> currently;
      EOperations operation_worry_level = EOperations::undefined;
      std::optional<size_t> iValue_worry_level = std::nullopt;
      size_t iTest = 0u;
      std::pair<size_t, size_t> destinations = { 0, 0 };

      size_t i_count_items_inspected = 0;

      static const operation_ty operations;

   public:
      TMonkey(void) = default;
      TMonkey(size_t id) : TMonkey() { iID = id; }
      TMonkey(TMonkey const&) = default;

      void set_operation_worry_level(std::pair<EOperations, std::optional<size_t>> const& val) {
         operation_worry_level = val.first;
         iValue_worry_level = val.second;
         }

      void set_test_value(size_t value) { iTest = value;  }
      void set_destination(bool cond, size_t target) {
         if (cond) destinations.first = target;
         else destinations.second = target;
         }

      void add_item(size_t val) { currently.emplace_back(val);  }

      std::vector<size_t>& get_list(void) { return currently; }
      std::vector<size_t> const& get_list(void) const { return currently; }

      size_t Current_Level(void) const {
         return std::accumulate(currently.begin(), currently.end(), size_t(0));
         }

      size_t get_test(void) const { return iTest; }
      size_t const& Inspected_Items(void) const { return i_count_items_inspected; }
      void start_process() { i_count_items_inspected = 0u; }

      void process_step(TMonkey::monkeys_ty& monkeys, size_t damage_rate = 3u, size_t const& reduce = 0u) {
         std::vector<size_t> processing = std::move(currently);
         auto op = operations.find(operation_worry_level);
         if (op == operations.end()) throw std::runtime_error("the requested operation "s + static_cast<char>(operation_worry_level) + " couldn't found");
         else {
            i_count_items_inspected += processing.size();
            std::for_each(processing.begin(), processing.end(), [this, &monkeys, &damage_rate, &reduce, op](auto const& val) {
               size_t iValue = iValue_worry_level ? *iValue_worry_level : val;
               iValue = op->second(val, iValue);
               if (reduce > 1) iValue %= reduce;
               iValue /= damage_rate;
               auto target = iValue % iTest == 0 ? destinations.first : destinations.second;
               if (auto to = monkeys.find(target); to != monkeys.end()) {
                  to->second.add_item(iValue);
                  }
               else throw std::runtime_error("a monkey with requested id = "s + std::to_string(target) + "isn't there"s);
               });
            }
         }

   };

   
   const TMonkey::operation_ty TMonkey::operations = {
     { EOperations::Addition,       [](size_t a, size_t b) -> size_t { return a + b;  } },
     { EOperations::Multiplication, [](size_t a, size_t b) -> size_t { return a * b;  } },
     { EOperations::Division,       [](size_t a, size_t b) -> size_t { return a / b;  } },
     { EOperations::Substraction,   [](size_t a, size_t b) -> size_t { return a - b;  } },
     { EOperations::Modulo,         [](size_t a, size_t b) -> size_t { return a % b;  } },
     { EOperations::undefined,      [](size_t, size_t) -> size_t { throw std::runtime_error("undefined operations");  } }
   };


   TMonkey::monkeys_ty monkeys;



   std::ostream& operator << (std::ostream& out, TMonkey const& data) {
      out << "Monkey with ID = " << data.iID << "\n";
      out << "  contained items:";
      std::for_each(data.currently.begin(), data.currently.end(), [](auto val) { std::cout << " " << val; });
      out << "\n  Operation " << static_cast<char>(data.operation_worry_level) << " with ";
      if (!data.iValue_worry_level) out << " old";
      else out << *data.iValue_worry_level;
      out << "\n  value for test condition " << data.iTest;
      out << "\n  if worried is true throw to " << data.destinations.first << " and if false throw to " << data.destinations.second;
      out << "\n  processed items = " << data.i_count_items_inspected;
      return out;
   }

   void Parse(TMonkey& monkey, std::string& strRow) {
      static auto constexpr read_condition = [](std::string& strRow) -> size_t {
         static const std::regex parser_value("^throw to monkey\\s*([0]|[1-9][0-9]*)");
         if (std::regex_match(strRow, parser_value)) {
            static std::string fmt_value("$1");
            std::string strValue = std::regex_replace(strRow, parser_value, fmt_value);
            size_t pos;
            size_t iValue = std::stoi(strValue, &pos);
            if (pos != strValue.length()) throw std::runtime_error("unexpected value for target in condition in "s + strRow + " / "s + strValue);
            else {
               return iValue;
               }
            }
         else {
            throw std::runtime_error("can't parse \""s + strRow + "\" as target for a condition"s);
            }
         };

      static auto constexpr read_test = [](std::string& strRow) -> size_t {
         static const std::regex parser_value("^divisible by\\s*([0]|[1-9][0-9]*)");
         if (std::regex_match(strRow, parser_value)) {
            static std::string fmt_value("$1");
            std::string strValue = std::regex_replace(strRow, parser_value, fmt_value);
            size_t pos;
            size_t iValue = std::stoi(strValue, &pos);
            if (pos != strValue.length()) throw std::runtime_error("unexpected value for test condition in "s + strRow + " / "s + strValue);
            else {
               return iValue;
            }
         }
         else {
            throw std::runtime_error("can't parse \""s + strRow + "\" as value for a test condition"s);
         }
      };

      static auto constexpr read_op = [](std::string& strRow) -> std::pair<TMonkey::EOperations, std::optional<size_t>> {
         static const std::regex parser_operation("^(new)\\s*(\\=)\\s*(old)\\s+(.)\\s* ([a-z0-9]+)");
         static const std::string fmt_operation("$4");
         static const std::string fmt_value("$5");
         std::pair<TMonkey::EOperations, std::optional<size_t>> ret = { TMonkey::EOperations::undefined, std::nullopt };
         if (std::regex_match(strRow, parser_operation)) {
            std::string strOperation = std::regex_replace(strRow, parser_operation, fmt_operation);
            switch(strOperation[0]) {
               case '+': ret.first = TMonkey::EOperations::Addition; break;
               case '-': ret.first = TMonkey::EOperations::Substraction; break;
               case '*': ret.first = TMonkey::EOperations::Multiplication; break;
               case '/': ret.first = TMonkey::EOperations::Division; break;
               case '%': ret.first = TMonkey::EOperations::Modulo; break;
               default: throw std::runtime_error("unexpected operation "s + strOperation + " in "s + strRow);
               }

            std::string strValue = std::regex_replace(strRow, parser_operation, fmt_value);
            if (strValue == "old"s) ret.second = std::nullopt;
            else {
               size_t pos;
               size_t iValue = std::stoi(strValue, &pos);
               if (pos != strValue.length()) throw std::runtime_error("unexpected value for operation in "s + strRow + " / "s + strValue );
               else {
                  ret.second = iValue;
                  }
               }
            }
         else {
            throw std::runtime_error("operation can't parsed: "s + strRow);
            }
         return ret;
         };

      static auto constexpr split_list = [](std::vector<size_t>& values, std::string& strRow)  {
         values.clear();
         size_t iPos = 0, iEnd;
         do {
            iEnd = strRow.find(",", iPos);
            std::string strPart = strRow.substr(iPos, iEnd - iPos);
            strPart.erase(0, strPart.find_first_not_of(" \t"));
            strPart.erase(strPart.find_last_not_of(" \t") + 1, strPart.length());
            size_t pos_part;
            size_t iValue = std::stoi(strPart, &pos_part);
            if (pos_part != strPart.length()) throw std::runtime_error("unexpected argument in list for items"s);
            else {
               values.emplace_back(iValue);
               }
            iPos = iEnd + 1;
            } 
         while (iEnd != std::string::npos);
         };

      static std::map<std::string, std::function<void(TMonkey&, std::string&)>> symbols = {
           { "Starting items:", [](TMonkey& monkey, std::string& strRow) { split_list(monkey.get_list(), strRow);  } },
           { "Operation:",      [](TMonkey& monkey, std::string& strRow) { monkey.set_operation_worry_level(read_op(strRow));  }  },
           { "Test:",           [](TMonkey& monkey, std::string& strRow) { monkey.set_test_value(read_test(strRow)); }  },
           { "If true:",        [](TMonkey& monkey, std::string& strRow) { monkey.set_destination(true, read_condition(strRow)); }  },
           { "If false:",       [](TMonkey& monkey, std::string& strRow) { monkey.set_destination(false, read_condition(strRow)); }  }
         };

      for(auto& [symbol, func] : symbols) {
         if(auto pos = strRow.find(symbol); pos != std::string::npos) {
            strRow.erase(0, pos + symbol.length() + 1);
            func(monkey, strRow);
            break;
            }
         }

      }

   size_t CheckResults(TMonkey::monkeys_ty const& monkeys) {
      std::vector<size_t> inspected;
      std::for_each(monkeys.begin(), monkeys.end(), [&inspected](auto const& val) {
         inspected.emplace_back(val.second.Inspected_Items());
         });

      if (inspected.size() < 2) throw std::runtime_error("count of processed monkeys to small.");
      else {
         std::sort(inspected.begin(), inspected.end(), [](auto lhs, auto rhs) { return lhs > rhs; });
         return inspected[0] * inspected[1];
         }
      }

   void Output(std::ostream& out, TMonkey::monkeys_ty const& monkeys) {
      for (auto const& [id, monkey] : monkeys) out << id << ":\n" << monkey << "\n\n";
      }


   void ReadValues(std::string const& strFilename, TMonkey::monkeys_ty& monkeys) {
      static std::regex parser_monkey("^(Monkey) ([0]|[1-9][0-9]*)(\\:)$");
      static const std::string fmt_id("$2");
      monkeys.clear();
      auto ifs = input(strFilename);
      std::string strRow;
      auto current_monkey = monkeys.end();
      while (std::getline(ifs, strRow)) {
         if (strRow.empty()) continue;
         strRow.erase(0, strRow.find_first_not_of(" \t"));
         strRow.erase(strRow.find_last_not_of(" \t") + 1, strRow.length());
         if(std::regex_match(strRow, parser_monkey)) {
            std::string strID = std::regex_replace(strRow, parser_monkey, fmt_id);
            if (strID.empty()) throw std::runtime_error("Input didn't contained an ID for the new Monkey");
            else {
               size_t pos;
               size_t iID = std::stoi(strID, &pos);
               if (pos != strID.length()) throw std::runtime_error("unexpected argument for Monkey with "s + strID);
               else {
                  bool success;
                  std::tie(current_monkey, success) = monkeys.insert({ iID, TMonkey(iID) });
                  if(!success) throw std::runtime_error("couldn't insert monkey with ID = "s + std::to_string(iID));
                  }
               }
            }
         else {
            if (current_monkey == monkeys.end()) throw std::runtime_error("unexpected input file, current monkey is empty.");
            else Parse(current_monkey->second, strRow);
            }
         }
      }



   void ReadValues_1(std::string const& strFilename) {
      TMonkey::monkeys_ty monkeys;
      ReadValues(strFilename, monkeys);

      std::for_each(monkeys.begin(), monkeys.end(), [](auto& val) mutable { val.second.start_process(); });
      for(size_t i = 0; i < 20; ++i) {
         std::for_each(monkeys.begin(), monkeys.end(), [&monkeys](auto& val) mutable { val.second.process_step(monkeys); });
         }

      std::cout << "Day 11/1: " << CheckResults(monkeys) << "\n";
      }
    
   void CheckData(size_t step, bool& boChecked, TMonkey::monkeys_ty const& monkeys) {
      // data for test algorithm 
      static std::map<size_t, std::vector<size_t>> to_control = {
            {     1, {     2,     4,     3,     6 } },
            {    20, {    99,    97,     8,   103 } },
            {  1000, {  5204,  4792,   199,  5192 } },
            {  2000, { 10419,  9577,   392, 10391 } },
            {  3000, { 15638, 14358,   587, 15593 } },
            {  4000, { 20858, 19138,   780, 20797 } },
            {  5000, { 26075, 23921,   974, 26000 } },
            {  6000, { 31294, 28702,  1165, 31204 } },
            {  7000, { 36508, 33488,  1360, 36400 } },
            {  8000, { 41728, 38268,  1553, 41606 } },
            {  9000, { 46945, 43051,  1746, 46807 } },
            { 10000, { 52166, 47830,  1938, 52013 } } };

      if(boChecked) {
         if(auto control = to_control.find(step); control != to_control.end()) {
            std::vector<size_t> inspected;
            std::for_each(monkeys.begin(), monkeys.end(), [&inspected](auto const& val) {
                 inspected.emplace_back(val.second.Inspected_Items());
                 });
            if(control->second != inspected) {
               boChecked = false;
               }
            }
         }
      }

   /*
std::cout << std::setw(5) << i + 1 << ":";
std::for_each(monkeys.begin(), monkeys.end(), [](auto const& val) { std::cout << " " << std::setw(6) << val.second.Inspected_Items();  });
std::cout << "\n";
*/

   void ReadValues_2(std::string const& strFilename) {

      TMonkey::monkeys_ty monkeys;
      ReadValues(strFilename, monkeys);

      size_t value_reduce = 1u;
      std::for_each(monkeys.begin(), monkeys.end(), [&value_reduce](auto& val) mutable {
            val.second.start_process();
            value_reduce *= val.second.get_test();
           });

      // bool boChecked = true;
      for (size_t i = 0; i < 10000; ++i) {
          std::for_each(monkeys.begin(), monkeys.end(), [&monkeys, &value_reduce](auto& val) mutable { 
                  val.second.process_step(monkeys, 1u, value_reduce);
                  });
          // CheckData(i + 1, boChecked, monkeys);
          }
      
      // if(boChecked == true) { std::cout << "successful with " << test << "\n";  }
         
      std::cout << "Day 11/2: " << CheckResults(monkeys) << "\n";
      }
 

}
