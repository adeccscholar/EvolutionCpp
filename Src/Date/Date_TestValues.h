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

namespace DateTest {

using tplTestStreams = std::tuple<TDate, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string>;
using vecTestStreams = std::vector<tplTestStreams>;


void Init(vecTestStreams& stream_cases);

extern std::vector<std::pair<std::string, std::string>> vecFormat;

template <int iTest = 1>
std::pair<size_t, size_t> Test(vecTestStreams const& stream_cases, std::ostream& err) {
   size_t iLines = 0u, iErrors = 0u;
   std::cout << "test for variante = " << iTest << " with format " << vecFormat[iTest].first << "\n";
   for(auto const& testcase : stream_cases) {
      TDate const& date = std::get<0>(testcase);
      std::istringstream ins(std::get<iTest>(testcase));
      TDate test;
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
