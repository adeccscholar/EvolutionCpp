#include <map>
#include <variant>
#include <optional>
#include <tuple>
#include <functional>
#include <type_traits>
#include <regex>

#include "AOC_Day21.h"
#include "AOC_Read.h"

using namespace std::literals::string_literals;

namespace Day21 {

   enum class EOperations : char {
      Addition       = '+',
      Multiplication = '*',
      Division       = '/',
      Substraction   = '-',
      Modulo         = '%',
      undefined
      };

   using base_ty = unsigned long long;
   using operation_ty = std::map<EOperations, std::pair<std::function<base_ty (base_ty, base_ty)>, EOperations>>;

   const operation_ty operations = {
      { EOperations::Addition,       { [](base_ty a, base_ty b) -> base_ty { return a + b;  }, EOperations::Substraction } },
      { EOperations::Substraction,   { [](base_ty a, base_ty b) -> base_ty { return a - b;  }, EOperations::Addition } },
      { EOperations::Multiplication, { [](base_ty a, base_ty b) -> base_ty { return a * b;  }, EOperations::Division } },
      { EOperations::Division,       { [](base_ty a, base_ty b) -> base_ty { return a / b;  }, EOperations::Multiplication } },
      { EOperations::Modulo,         { [](base_ty a, base_ty b) -> base_ty { return a % b;  }, EOperations::undefined } },
      { EOperations::undefined,      { [](base_ty, base_ty) -> base_ty { throw std::runtime_error("undefined operation in mission."); }, EOperations::undefined } }
   };


   struct TOperation {
      std::string operand1;
      std::string operand2;
      EOperations _operator; 
      };

   using expression_ty   = std::variant<base_ty, TOperation>;
   using expressions_ty  = std::map<std::string, expression_ty>;
   using expressions_vec = std::vector<std::pair<std::string, expression_ty>>;

   struct VisitHelp {
      std::ostream& out;
      VisitHelp(std::ostream& p) : out(p) { }
      void operator()(base_ty const& val) { out << val; }
      void operator()(TOperation const& val) { out << val.operand1 << " " << static_cast<char>(val._operator) << " " << val.operand2; }
      };

   std::ostream& operator << (std::ostream& out, expression_ty const& data) {
      std::visit(VisitHelp(out), data);
      return out;
      }

   std::ostream& Print(std::ostream& out, expressions_ty& expressions, std::string const& variable, std::string const& end = ""s) {
      if (auto expression = expressions.find(variable); expression != expressions.end()) {
         auto& [key, value] = *expression;
         out << key << ": " << value << end;
         }
      else throw std::runtime_error("variable with the name \""s + variable + "\" not found in the mission."s);
      return out;
      }


   bool IsContant(expressions_ty& expressions, std::string const& variable) {
      if (auto expression = expressions.find(variable); expression != expressions.end()) {
         auto& [key, value] = *expression;
         return (std::holds_alternative<base_ty>(value) ? true : false);
         }
      else throw std::runtime_error("variable with the name \""s + variable + "\" not found in the mission."s);
      }


   void Set(expressions_ty& expressions, std::string const& variable, base_ty llValue) {
      if (auto expression = expressions.find(variable); expression != expressions.end()) {
         auto& [key, value] = *expression;
         value = llValue;
         }
      else throw std::runtime_error("variable with the name \""s + variable + "\" not found in the mission."s);
      }

   base_ty Get(expressions_ty& expressions, std::string const& variable, bool compress = true) {
      if(auto expression = expressions.find(variable); expression != expressions.end()) {
         auto& [key, value] = *expression;
         if (std::holds_alternative<base_ty>(value)) return std::get<base_ty>(value);
         else {
            auto const& statement = std::get<TOperation>(value);
            if(auto op = operations.find(statement._operator); op != operations.end()) {
               base_ty llValue = op->second.first(Get(expressions, statement.operand1, compress), Get(expressions, statement.operand2, compress));
               if(compress) value = llValue;
               return llValue;
               }
            else {
               throw std::runtime_error("operator "s + static_cast<char>(statement._operator) + " for \""s + key + "\" not found in the mission."s);
               }
            }
         }
      else {
         throw std::runtime_error("variable with the name \""s + variable + "\" not found in the mission."s);
         }
      return 0ll;
      }

   base_ty Get(expressions_ty const& expressions, std::string const& variable) {
      if (auto expression = expressions.find(variable); expression != expressions.end()) {
         auto& [key, value] = *expression;
         if (std::holds_alternative<base_ty>(value)) return std::get<base_ty>(value);
         else {
            auto const& statement = std::get<TOperation>(value);
            if (auto op = operations.find(statement._operator); op != operations.end()) {
               base_ty llValue = op->second.first(Get(expressions, statement.operand1), Get(expressions, statement.operand2));
               return llValue;
            }
            else {
               throw std::runtime_error("operator "s + static_cast<char>(statement._operator) + " for \""s + key + "\" not found in the mission."s);
            }
         }
      }
      else {
         throw std::runtime_error("variable with the name \""s + variable + "\" not found in the mission."s);
      }
      return 0ll;
   }


/*
cczh: sllz + lgvd
zczc: 2
ptdq: humn - dvpt
*/
   std::pair<std::string, expression_ty> Parse(std::string const& line) {
      std::string strVariable;
      expression_ty expression;
      static const std::regex parser_input("^([a-z]{4})\\s*:\\s*([0-9]{1,4}|[a-z]{4}\\s*(.)\\s*[a-z]{4})");
      static const std::regex parser_expression("\\s*([a-z]{4})\\s*(.)\\s*([a-z]{4})");
      static const std::string strFmtVariable   = "$1";
      static const std::string strFmtExpression = "$2";
      static const std::string strFmtOperand1   = "$1";
      static const std::string strFmtOperation  = "$2";
      static const std::string strFmtOperand2   = "$3";
      if (std::regex_match(line, parser_input)) {
         strVariable = std::regex_replace(line, parser_input, strFmtVariable);
         std::string strExpression = std::regex_replace(line, parser_input, strFmtExpression);
         if (std::regex_match(strExpression, parser_expression)) {
            TOperation operation;
            operation.operand1 = std::regex_replace(strExpression, parser_expression, strFmtOperand1);
            std::string strOperation = std::regex_replace(strExpression, parser_expression, strFmtOperation);
            operation.operand2 = std::regex_replace(strExpression, parser_expression, strFmtOperand2);
            
            switch (strOperation[0]) {
               case '+': operation._operator = EOperations::Addition; break;
               case '-': operation._operator = EOperations::Substraction; break;
               case '*': operation._operator = EOperations::Multiplication; break;
               case '/': operation._operator = EOperations::Division; break;
               case '%': operation._operator = EOperations::Modulo; break;
               default: throw std::runtime_error("unexpected operation "s + strOperation + " in "s + line);
               }

            expression = operation;
            return { strVariable, expression };
         } 
         else {
            size_t err_pos;
            base_ty llValue = stoll(strExpression, &err_pos);
            if (err_pos < strExpression.length()) throw std::runtime_error("error reading expression \""s + line + "\".");

            expression = llValue;
            return { strVariable, expression };
            }
         }
      else {
         throw std::runtime_error("can't parse the line \""s + line + "\".");
         }

      }

   expressions_ty ReadAll(std::string const& strFilename) {
      auto ifs = input(strFilename);
      std::string strRow;
      expressions_ty expressions;
      while (std::getline(ifs, strRow)) {
         auto [variable, expression] = Parse(strRow);
         expressions.insert({ variable, expression });
         }
      return expressions;
      }

   void ReadValues_1(std::string const& strFilename) {
      expressions_ty expressions = ReadAll(strFilename);
      base_ty llValue = Get(expressions, "root");
      std::cout << "Day 21/1: " << llValue << "\n";
   }


   std::tuple<expressions_vec, std::string, base_ty> Follow(expressions_ty const& expressions, std::string const& from, std::string const& to = "root"s) {
      expressions_vec reverse;
      std::string start = from, next;
      base_ty start_value;

      expression_ty expression;
      expression = Get(expressions, from);
      reverse.push_back({ from, expression });

      while(start != to) {
         if(auto it = std::find_if(expressions.begin(), expressions.end(), [&next, &start](auto const& val) {
                if(std::holds_alternative<TOperation>(val.second)) {
                   TOperation op = std::get<TOperation>(val.second);
                   if (op.operand1 == start) {
                      start = val.first;
                      next = op.operand2;
                      return true;
                      } 
                   if(op.operand2 == start) {
                      start = val.first;
                      next = op.operand1;
                      return true;
                      }
                   }
                return false;
                   }); it != expressions.end()) {
            reverse.push_back(*it);
            //std::cout << it->first << ": " << it->second << "\n";
            }
         }
      if(auto it = expressions.find(next); it != expressions.end()) {
         //reverse.push_back(*it);
         start_value = Get(expressions, next);
         //std::cout << it->first << ": " << it->second << " = " << start_value << "\n";
         }
      return { reverse, next, start_value };
   }



   //std::cout << "\n\nDay 21/2: " << llValue << " - " << llHuman << "\n";

   /*
   if (auto expression = expressions.find("root"); expression != expressions.end()) {
      auto& [key, value] = *expression;
      if (std::holds_alternative<base_ty>(value)) throw std::runtime_error("term \"root\" don't contain a expression."s);
      else {
         auto & statement = std::get<TOperation>(value);
         Print(std::cout, expressions, statement.operand1, "\n");
         Print(std::cout, expressions, statement.operand2, "\n");
         auto llOp1 = Get(expressions, statement.operand1, false);
         auto llOp2 = Get(expressions, statement.operand2, false);
         std::cout << "check: operation was " << static_cast<char>(statement._operator) << "\n"
                   << statement.operand1 << " = " << llOp1 << "\n"
                   << statement.operand2 << " = " << llOp2 << "\n";
         Print(std::cout, expressions, "humn", " = ");
         std::cout << Get(expressions, "humn", false) << "\n";
         statement._operator = EOperations::Substraction;

         }

      //Set(expressions, "humn", 301ll);

      auto llValue = Get(expressions, "root", false);
      auto llHuman = Get(expressions, "humn", false);
      std::cout << "\n\n\n";
      */

      /*
         }
      else {
         throw std::runtime_error("variable with the name \"root\" not found in the mission."s);
         }
         */


   void ReadValues_2(std::string const& strFilename) {
      expressions_ty expressions = ReadAll(strFilename);
      base_ty solution = 0ll;

      
      if (auto expr_root = expressions.find("root"); expr_root != expressions.end()) {
         auto& [key, value] = *expr_root;
         if (std::holds_alternative<base_ty>(value)) throw std::runtime_error("term \"root\" don't contain a expression."s);
         else {
            auto& statement = std::get<TOperation>(value);
            statement._operator = EOperations::Substraction;
         }
      }
      else throw std::runtime_error("critical, root expression not found");
      
      auto [terms, start, val] = Follow(expressions, "humn", "root");
      std::cout << "\nstart with: root = 0 and " << start << " = " << val << "\n";
      //Set(expressions, start, val);

      std::string current = start, next;
      base_ty value = 0ll;
      for(size_t i = terms.size() - 1; i > 0; --i) {
         if (!std::holds_alternative<TOperation>(terms[i].second)) throw std::runtime_error("not solvable");
         auto const& term = std::get<TOperation>(terms[i].second);
         next = terms[i - 1].first;
         if (auto op = operations.find(term._operator); op != operations.end()) {
            if(auto rev_op = operations.find(op->second.second); rev_op != operations.end()) {
               base_ty new_value;
               if (term.operand1 == next) {
                  new_value = Get(expressions, term.operand2, false);
                  std::cout << term.operand2 << ": " << new_value << "  " << (IsContant(expressions, term.operand2) ? " ja " : " nein ") << "\n";
                  value = rev_op->second.first(value, new_value);
                  }
               else if (term.operand2 == next) {
                  new_value = Get(expressions, term.operand1, false);
                  std::cout << term.operand1 << ": " << new_value << "  " << (IsContant(expressions, term.operand1) ? " ja " : " nein ") << "\n";
                  if(term._operator == EOperations::Substraction || term._operator == EOperations::Division)
                     value = op->second.first(new_value, value);
                  else
                     value = rev_op->second.first(value, new_value);
                  }
               else throw std::runtime_error("operand not found");
               }
            else {
               throw std::runtime_error("reverse operation for this term not found");
               }
            }
         else {
            throw std::runtime_error("operation in this term not found");
            }
         std::cout << terms[i].first << ": " << terms[i].second << "\n";
         std::cout << next << ": " << value << "\n";
         }
      //std::cout << next << " = " << value;
   }
}
