// Distress Signal

#include <variant>
#include <vector>
#include <stack>
#include <optional>
#include <algorithm>

#include "AOC_Day13.h"
#include "AOC_Read.h"

using namespace std::literals::string_literals;

namespace Day13 {

   // Day 13, Distress Signal ... https://adventofcode.com/2022/day/13
   //
   // Problem scheint für viele mit imperativer Programmierung nicht lösbar
   // Eindeutig geschummelt, da ein Beispiel speziell für Funktionale Programmierung
   // Funktionen höherer Ordnung
   // Foldering (also termed reduce, accumulate, aggregate, compress, or inject)
   // Foldering zum Lesen rekursiver Datenstrukturen, Plus Abbildung auf sich selbst
   // 
   // 
   // Set C++14 Foldering in C++ ABER zur compile-time
   // 
   //  template<typename ... Args>
   //  void print(Args ... args) {
   //    ([](const auto& x) { cout << x << "\n"; }(args), ...);
   //    }
   // 
   // für compile-time sehr gut unterstützt, hier aber in der Eingabe
   // 


   template<typename ... Args>
   void print(Args ... args) {
       ([](const auto& x) { std::cout << x << " "; }(args), ...);
       }

   void Test() {
      print("Hallo ", "Welt\n", "Die Anwort auf alle Fragen ist"s, 42, "\nPi = ", 3.1415);
      }

   // auf den ersten Blick lösbar mit Variants, wirklich?
 
   using type_list  = std::vector<size_t>;
   using type_param = std::variant<size_t, type_list>;
   using param_list = std::vector<type_param>;

   std::ostream& operator << (std::ostream& out, type_list const& vals) {
      if (vals.size() == 0u) out << "[ ]";
      else
         for (size_t i = 0u; auto const& val : vals) out << (i++ ? ", " : "[ ") << val;
      return out << " ]";
      }

   std::ostream& operator << (std::ostream& out, param_list const& vals) {
      for (size_t i = 0u; auto const& v : vals) {
         std::visit([&i, &out](auto&& arg) { out << (i++ ? ", " : "") << arg << ""; }, v);
         }
      return out;
      }

   std::strong_ordering operator <=> (type_param const& lhs, type_param const& rhs) {
      if (std::holds_alternative<size_t>(lhs)) {
         if (std::holds_alternative<size_t>(rhs)) return std::get<size_t>(lhs) <=> std::get<size_t>(rhs);
         else {
            type_list left_list = { std::get<size_t>(lhs) };
            return left_list <=> std::get<type_list>(rhs);
            }
         }
      else { 
         if (std::holds_alternative<size_t>(rhs)) {
            type_list right_list = { std::get<size_t>(rhs) };
            return std::get<type_list>(lhs) <=> right_list;
            }
         else {
            return std::get<type_list>(lhs) <=> std::get<type_list>(rhs);
            }
         }
      }

   std::strong_ordering operator <=> (param_list const& lhs, param_list const& rhs) {
      auto index = std::make_pair<size_t, size_t>(0u, 0u);
      for(;index.first < lhs.size(); ++index.first, ++index.second) {
         if(index.second == rhs.size()) return std::strong_ordering::greater;
         if (auto val = lhs[index.first] <=> rhs[index.second]; val != std::strong_ordering::equal) return val;
         } 
      if (index.second < rhs.size()) return std::strong_ordering::less;
      return std::strong_ordering::equal;
      }

    void Test1(void) {
       param_list values1, values2;
       values1.emplace_back(1u);
       type_list l1 = { 12u, 0, 25u };
       values1.emplace_back(l1);
       values1.emplace_back(0u);

       values2 = values1;

       std::cout << "values1: " << values1 << "\n";
       std::cout << "values2: " << values2 << "\n";

       std::cout << "\n\n" << std::boolalpha;

       // Compare non equal containers
       std::cout << "values1 == values2 returns " << (values1 == values2) << '\n';
       std::cout << "values1 != values2 returns " << (values1 != values2) << '\n';
       std::cout << "values1 <  values2 returns " << (values1 < values2) << '\n';
       std::cout << "values1 <= values2 returns " << (values1 <= values2) << '\n';
       std::cout << "values1 >  values2 returns " << (values1 > values2) << '\n';
       std::cout << "values1 >= values2 returns " << (values1 >= values2) << '\n';
       }

    // mit funktionaler Programmierung lösbar, da es auf Lambda Kalkül beruht (Referenz auf sich selbst)
    // einen Knoten binden
    // Ausflug in die mathematische Logik (kombinatorische Logik)
    // parallel zu Lambda Kalkül (Church und Kleene)
    // Curry - Curry Paradoxon (Aussagenlogik)
    // Fixed-point combinator   (Ein Fixpunkt-Kombinator ist ein mathematischer Operator in Form einer Funktion 
    // höherer Ordnung, welcher von einer Funktion f einen ihrer Fixpunkte liefert.
    // --------------- Ende des 1. Beispiels zur Erklärung des Variants
    // https://en.wikipedia.org/wiki/Fixed-point_combinator

    template <typename ty>
    using VarTy = std::variant<size_t, std::vector<ty>>;

    template <template <typename> class K>
    struct Fixed : K<Fixed<K>> {
       using K<Fixed>::K;
       };

    using param_type2 = Fixed<VarTy>;
    using param_list2 = std::vector<param_type2>;


    std::ostream& operator << (std::ostream& out, param_type2 const& data) {
       if (std::holds_alternative<size_t>(data)) out << std::get<size_t>(data);
       else {
          param_list2 const& list = std::get<param_list2>(data);
          if (list.size() == 0) out << "[ ]";
          else {
             for (size_t pos = 0u; auto const& val : list) out << (pos++ ? ", " : "[ ") << val;
             out << " ]";
             }
          }
       return out;
       }

   std::strong_ordering Compare(param_type2 const& lhs, param_type2 const& rhs) {
      if(std::holds_alternative<size_t>(lhs) && std::holds_alternative<size_t>(rhs)) {
          return std::get<size_t>(lhs) <=> std::get<size_t>(rhs);
          }
      else if(std::holds_alternative<size_t>(lhs)) {
          param_list2 left_list = param_list2 { std::get<size_t>(lhs) };
          return Compare(left_list, rhs);
          }
      else if(std::holds_alternative<size_t>(rhs)) {
          param_list2 right_list = param_list2{ std::get<size_t>(rhs) };
          return Compare(lhs, right_list);
       }
      else {
         param_list2 const& lstLeft  = std::get<param_list2>(lhs);
         param_list2 const& lstRight = std::get<param_list2>(rhs);
         auto max_size = std::pair(lstLeft.size(), lstRight.size());
         for(auto [l, r] = std::make_pair(lstLeft.begin(), lstRight.begin());l != lstLeft.end() && r != lstRight.end();++l,++r) {
            if (auto val = Compare(*l, *r); val != 0) return val;
            }
         return max_size.first <=> max_size.second;
         }
      }
   
   // keine Template type deduction ... daher nur direkter Aufbau

    void Test2() {
       //param_type2 param1 = param_list2 { 1u, param_list2 { param_list2 { 7u, 2u }, 4u, 6u }, 3u };
       param_type2 param1 = param_list2{ 1u, param_list2 { param_list2 { 7u, param_list2 { 2u, 3u } }, 4u, 6u }, 3u };
       //param_type2 param2 = param_list2 { 1u, param_list2 { param_list2 { 7u, 2u }, 4u, 6u }, 3u };
       param_type2 param2 = param_list2{ 1u, param_list2 { param_list2 { param_list2 { 7u, 2u }, 2u }, 4u, 6u }, 3u };
       //param_type2 param2 = param_list2{ 1u, param_list2 { param_list2 { 7u, 2u }, param_list2 { 4u }, 6u }, 3u };
       std::cout << param1 << "\n";
       std::cout << param2 << "\n";

       std::cout << "\n\n" << std::boolalpha;

       // Compare non equal containers
       std::cout << "values1 == values2 returns " << (Compare(param1, param2) == 0) << "\n";
       std::cout << "values1 != values2 returns " << (Compare(param1, param2) != 0) << "\n";
       std::cout << "values1 <  values2 returns " << (Compare(param1, param2) < 0) << "\n";
       std::cout << "values1 <= values2 returns " << (Compare(param1, param2) <= 0) << "\n";
       std::cout << "values1 >  values2 returns " << (Compare(param1, param2) > 0) << "\n";
       std::cout << "values1 >= values2 returns " << (Compare(param1, param2) >= 0) << "\n";

    }


   using value_list = std::vector<std::pair<param_type2, param_type2>>;

   param_type2 parse(std::string input) {
      static auto trim = [](std::string&& text) {
         text.erase(0, text.find_first_not_of(" \t"));
         text.erase(text.find_last_not_of(" \t") + 1, text.length());
         return text;
         };

      param_type2 retval;
      std::stack<param_list2> workflow;
      size_t pos = 0u, end = input.length();
      while ((end = input.find_first_of(",[]", pos)) != std::string::npos) {
         std::optional<size_t> value = std::nullopt;
         std::string strPart = trim(input.substr(pos, end - pos));
         if(strPart.length() > 0) {
            size_t err_pos = 0u;
            value = std::stoi(strPart, &err_pos);
            if (err_pos != strPart.size()) throw std::runtime_error("unexpected chars in input");
            }
         if (value && workflow.empty()) throw std::runtime_error("workflow is empty");

         switch(input[end]) {
            case '[':
               if(value) workflow.top().push_back(*value);
               workflow.push(param_list2{ }); 
               break;
            case ',':
               if (!workflow.empty()) {
                  if (value) workflow.top().push_back(*value);
                  }
               else throw std::runtime_error("no param list open");
               break;
            case ']':
               if (!workflow.empty()) {
                  if(value) workflow.top().push_back(*value);
                  param_type2 elem = workflow.top();
                  workflow.pop();
                  if (!workflow.empty())
                     workflow.top().push_back(elem);
                  else
                     retval = elem;
                  }
               else throw std::runtime_error("] don't match to a open bracket");
               break;
            default: throw std::runtime_error("critical, unhandled char in parsing");
            }
         pos = end + 1;
         }
      if (!workflow.empty()) throw std::runtime_error("wrong input");
      return retval;
   }



   value_list ReadPairs(std::string const& strFilename) {
      value_list all_values;
      auto ifs = input(strFilename);
      std::string strRow;
      size_t lrows = 0u;
      std::pair<param_type2, param_type2> value;
      while (std::getline(ifs, strRow)) {
         if (lrows % 3 == 0) value.first = parse(strRow);
         else if(lrows % 3 == 1) {
                 value.second = parse(strRow);
                 all_values.push_back(value);
                 }
         ++lrows;
         }

      return all_values;
      }

   std::vector<param_type2> ReadAll(std::string const& strFilename) {
      std::vector<param_type2> all_values;
      auto ifs = input(strFilename);
      std::string strRow;
      size_t lrows = 0u;
      std::pair<param_type2, param_type2> value;
      while (std::getline(ifs, strRow)) {
         if(strRow.length() > 0) {
            all_values.emplace_back(parse(strRow));
            }
         }
      return all_values;
   }

   void ReadValues_1(std::string const& strFilename) {
      try {
         auto values = ReadPairs(strFilename);
         size_t sum = 0u, line = 1u;
         for(auto const& val : values) {
            //std::cout << val.first << "\n"
            //   << val.second << "\n";
            if (Compare(val.first, val.second) < 0) sum += line;
            ++line;
            //std::cout << line++ << ". pair " << (Compare(val.first, val.second) < 0 ? "kleiner" : "nein") << "\n";
            }
         std::cout << "Day 13/1: " << sum << "\n";
         }
      catch(std::exception& ex) {
         std::cerr << "error: " << ex.what() << "\n";
         }
      }

   void ReadValues_2(std::string const& strFilename) {
      std::vector<param_type2> values = ReadAll(strFilename);
      param_type2 p1 = parse("[[2]]"), p2 = parse("[[6]]");
      values.push_back(p1);
      values.push_back(p2);

      std::sort(values.begin(), values.end(), [](auto const& lhs, auto const& rhs) { return Compare(lhs, rhs) < 0;  });

      size_t value = 1u;
      for (size_t index = 1u; auto const& val : values) { 
         if (Compare(val, p1) == 0 || Compare(val, p2) == 0) {
            value *= index;
            }
         ++index;
         // std::cout << val << "\n";
         }
      std::cout << "Day 13/2: " << value << "\n";
      }
}
