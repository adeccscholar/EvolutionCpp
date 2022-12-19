#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

#include "AOC_Read.h"


namespace Day2 {

   enum class EKind : char {
      Rock = 1,
      Paper = 2,
      Scissor = 3
   };

   enum class EResult : char {
      Lost = 0,
      Draw = 3,
      Win = 6
   };

   using Data_Step1 = std::pair<EKind, EKind>;
   using Data_Step2 = std::pair<EKind, EResult>;

   std::istream& operator >> (std::istream& istr, Data_Step1& data) {
      static std::map<char, EKind> values = {
         { 'A', EKind::Rock}, { 'B', EKind::Paper }, { 'C', EKind::Scissor },
         { 'X', EKind::Rock}, { 'Y', EKind::Paper }, { 'Z', EKind::Scissor }
      };
      char c1, c2;
      istr >> c1 >> std::skipws >> c2;
      data = std::make_pair(values[c1], values[c2]);
      return istr;
   }

   std::istream& operator >> (std::istream& istr, Data_Step2& data) {
      static std::map<char, EKind> values_kind = {
         { 'A', EKind::Rock}, { 'B', EKind::Paper }, { 'C', EKind::Scissor } };
      static std::map<char, EResult> values_result = {
         { 'X', EResult::Lost}, { 'Y', EResult::Draw }, { 'Z', EResult::Win } };
      char c1, c2;
      istr >> c1 >> std::skipws >> c2;
      data = std::make_pair(values_kind[c1], values_result[c2]);
      return istr;
   }


   // Rock defeats Scissors, Scissors defeats Paper, and Paper defeats Rock. 
   // If both players choose the same shape, the round instead ends in a draw.
   std::map<Data_Step1, EResult> analyze_step1 = {
      { { EKind::Rock, EKind::Rock },       EResult::Draw },
      { { EKind::Rock, EKind::Paper },      EResult::Win },
      { { EKind::Rock, EKind::Scissor },    EResult::Lost },

      { { EKind::Paper, EKind::Rock },      EResult::Lost },
      { { EKind::Paper, EKind::Paper },     EResult::Draw },
      { { EKind::Paper, EKind::Scissor },   EResult::Win  },

      { { EKind::Scissor, EKind::Rock },    EResult::Win },
      { { EKind::Scissor, EKind::Paper },   EResult::Lost },
      { { EKind::Scissor, EKind::Scissor }, EResult::Draw }
   };

   std::map<Data_Step2, EKind> analyze_step2 = {
      { { EKind::Rock,    EResult::Draw }, EKind::Rock },
      { { EKind::Rock,    EResult::Win },  EKind::Paper },
      { { EKind::Rock,    EResult::Lost }, EKind::Scissor },

      { { EKind::Paper,   EResult::Lost }, EKind::Rock },
      { { EKind::Paper,   EResult::Draw }, EKind::Paper },
      { { EKind::Paper,   EResult::Win  }, EKind::Scissor },

      { { EKind::Scissor, EResult::Win },  EKind::Rock },
      { { EKind::Scissor, EResult::Lost }, EKind::Paper },
      { { EKind::Scissor, EResult::Draw }, EKind::Scissor }
   };

   template <typename Analyzer>
   void ReadValues(std::string const& strFilename, std::string const& strText, Analyzer const& analyze) {
      auto ifs = input(strFilename);
      typename Analyzer::key_type data;
      int iScore = 0;
      do {
         ifs >> data;
         if(auto it = analyze.find(data); it != analyze.end()) {
            iScore += static_cast<int>(data.second);
            iScore += static_cast<int>(it->second);
            }
         } 
      while (!ifs.eof());
      std::cout << strText << ": " << iScore << "\n";
      }

   void ReadValues_1(std::string const& strFilename) {
      ReadValues(strFilename, "Day 2 / 1", analyze_step1);
      }

   void ReadValues_2(std::string const& strFilename) {
      ReadValues(strFilename, "Day 2 / 2", analyze_step2);
      }

}