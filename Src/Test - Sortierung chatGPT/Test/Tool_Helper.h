#pragma once

#include <functional>
#include <string>
#include <chrono>
#include <vector>

/**
 * @brief method to call a function and measure the used time
 * @tparam tty type for the time value (from std::chrono) for the measure
 * @tparam fty type for function which is called inside this template
 * @tparam ...Args Pack with arguments for the function call
 * @param time Reference Parameter from the type tty for the used time
 * @param function Parameter with the function which called
 * @param ...args Arguments Pack for the function call
 * @return original return value of the function which called
*/
template <typename tty, typename fty, typename... Args>
inline auto Call(tty& time, fty function, Args... args) {
   auto func_start = std::chrono::high_resolution_clock::now();
   auto ret = function(std::forward<Args>(args)...);
   auto func_ende = std::chrono::high_resolution_clock::now();
   time = std::chrono::duration_cast<tty>(func_ende - func_start);
   return ret;
}

inline std::string ReadHlp(std::string const& strRow, size_t& iPos) {
   size_t iEnd = strRow.find(";", iPos), iOldPos(iPos);
   iPos = iEnd + 1;
   return strRow.substr(iPos, iEnd - iOldPos);
}

inline std::vector<std::string> tokenize(std::string const& strInput, std::string const& strDelimiter, int iSize = -1) {
   std::vector<std::string> vecData;
   if (iSize >= 0) vecData.reserve(iSize);
   size_t iPos = 0, iEnd;
   do {
      iEnd = strInput.find(strDelimiter, iPos);
      std::string strPart = strInput.substr(iPos, iEnd - iPos);
      vecData.emplace_back(std::forward<std::string>(strPart));
      iPos = iEnd + strDelimiter.length();
   } while (iEnd != std::string::npos);
   return vecData;
}


//#include <string>
#include <array>
//#include <vector>
#include <charconv>
#include <cstdlib>
#include <concepts>


template <typename ty>
requires std::floating_point<ty>
inline std::string my_Double_to_String(ty const& value) {
   std::array<char, 20> target;
   auto [ptrValue, ec] = std::to_chars(target.data(), target.data() + target.size(), value);
   return std::string(target.data(), ptrValue);
}

//template <typename ty>
//requires std::floating_point<ty>
//inline std::string my_Double_to_String(ty const& value, int iPrecision) {
inline std::string my_Double_to_String(std::floating_point auto const& value, int iPrecision) {
   std::array<char, 20> target;
   auto [ptrValue, ec] = std::to_chars(target.data(), target.data() + target.size(), value, std::chars_format::fixed, iPrecision);
   return std::string(target.data(), ptrValue);
}


template <typename ty>
requires std::floating_point<ty>
inline std::string my_Double_to_String_G(ty const& value, int iPrecision) {
   std::array<char, 20> target;
   auto [ptrValue, ec] = std::to_chars(target.data(), target.data() + target.size(), value, std::chars_format::fixed, iPrecision);
   auto itPoint = std::find(target.begin(), target.end(), '.');
   *itPoint = ',';
   auto iLen = itPoint - target.begin();
   auto d = std::div((int)iLen, 3);
   if (d.quot > 0) {
      std::string strVal(target.data(), ptrValue);
      size_t start = d.rem == 0 ? 3 : d.rem;
      for (size_t a = start == 3 ? 1 : 0; a < d.quot; ++a) {
         strVal.insert(strVal.begin() + start, '.');
         start += 4;
      }
      return strVal;
   }
   return std::string(target.data(), ptrValue);
}