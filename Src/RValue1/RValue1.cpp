// RValue1.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <string>
#include <algorithm>




/*
std::string trim(std::string const& strVal) {
   std::string tmp = strVal;   ////// KOPIE
   tmp.erase(0, tmp.find_first_not_of("\t "));
   tmp.erase(tmp.find_last_not_of(" \t\f") + 1, tmp.length());
   return tmp;
   }
*/
// --------------------------------------------------------------------------------

std::string&& ltrim(std::string&& strVal) {
   strVal.erase(0, strVal.find_first_not_of("\t "));
   return std::move(strVal);  // RVal()
}

std::string&& rtrim(std::string&& strVal) {
   strVal.erase(strVal.find_last_not_of(" \t\f") + 1, strVal.length());
   return std::move(strVal);  // RVal()
}

std::string&& trim(std::string&& strVal) {
   return rtrim(ltrim(std::forward<std::string>(strVal)));
}

std::string trim(std::string const& strVal) {
   return trim(std::string(strVal));   ////// KOPIE
   }

// std::move() ... "std::RVal"
// std::forward<type>()
// std::swap

int main()
{
   std::string strTest1 = "   Hallo Welt!    ";
   std::cout << "|" << strTest1 << "|\n";
   std::string strTest2 = "Test";
   strTest2 = trim(std::forward<std::string>(strTest1));
   //std::string strTest2 = trim(strTest1);
   //std::string strTest2 = trim("   Hallo Welt!    ");   // erzeugt ein Einmalobjekt (Wegwerfobjekt)
   std::cout << "|" << strTest1 << "|\n";
   std::cout << "|" << strTest2 << "|\n";
}
