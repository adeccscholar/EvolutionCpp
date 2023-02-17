//---------------------------------------------------------------------------

#ifndef Date_TestValuesH
#define Date_TestValuesH
//---------------------------------------------------------------------------

#include "date.h"

#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <utility>
#include <type_traits>

namespace DateTest {

using tplTestStreams = std::tuple<TDate, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string>;
using vecTestStreams = std::vector<tplTestStreams>;


void Init(vecTestStreams& stream_cases);

extern std::vector<std::pair<std::string, std::string>> vecFormat;

template<typename ty>
struct is_my_date {
   constexpr static bool value =  std::is_same<TDate, ty>::value;
   };

template <int iTest = 1, typename ty = TDate>
std::pair<size_t, size_t> Test(vecTestStreams const& stream_cases, std::ostream& err) {
   static_assert(is_my_date<ty>::value, "you must use an own Date class for this test.");
   size_t iLines = 0u, iErrors = 0u;
   std::cout << "test for variante = " << iTest << " with format " << vecFormat[iTest].first << "\n";
   for(auto const& testcase : stream_cases) {
      ty const& date = std::get<0>(testcase);
      std::istringstream ins(std::get<iTest>(testcase));
      ty test;
      test.Format(vecFormat[iTest].second);
      ++iLines;
      ins >> test;
      if(test != date) {
         err << " error while reading " << std::get<iTest>(testcase) << ", "
             << "expected is " << date << " occurs " << test << "\n";
         ++iErrors;
         }
      }

   return { iLines, iErrors };
   }


}

#endif
