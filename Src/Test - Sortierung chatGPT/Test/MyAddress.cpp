#include "MyAddress.h"

#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std::literals;

void TAddress::_init(void) {
   strCity = ""s;
   strStreet = ""s;
   strStreetNumber = ""s;
   strZipCode = ""s;
   strUrbanUnit = ""s;
   strUrbanUnit_Old = ""s;
   strDistrict = ""s;
}
void TAddress::_copy(TAddress const& ref) {
   City(ref.City());
   Street(ref.Street());
   StreetNumber(ref.StreetNumber());
   ZipCode(ref.ZipCode());
   UrbanUnit(ref.UrbanUnit());
   UrbanUnit_Old(ref.UrbanUnit_Old());
   District(ref.District());
}

void TAddress::_swap(TAddress&& ref) noexcept {
   swap(strCity, ref.strCity);
   swap(strStreet, ref.strStreet);
   swap(strStreetNumber, ref.strStreetNumber);
   swap(strZipCode, ref.strZipCode);
   swap(strUrbanUnit, ref.strUrbanUnit);
   swap(strUrbanUnit_Old, ref.strUrbanUnit_Old);
   swap(strDistrict, ref.strDistrict);
}


// Compare the Addresses with DIN 5007-2 Norm, Streetnumber with numeric part and addition
bool TAddress::CompareDIN5007(TAddress const& other) const {
   // Compare the City part of Address using the rules of DIN 5007-2 norm
   std::string str_this_City = normalizeDIN(strCity);
   std::string str_other_City = normalizeDIN(other.strCity);
   if (str_this_City != str_other_City) {
      return str_this_City < str_other_City;
   }

   // Compare the Urban Unit part of Address using the rules of DIN 5007-2 norm
   std::string str_this_UrbanUnit = normalizeDIN(strUrbanUnit);
   std::string str_other_UrbanUnit = normalizeDIN(other.strUrbanUnit);
   if (str_this_UrbanUnit != str_other_UrbanUnit) {
      return str_this_UrbanUnit < str_other_UrbanUnit;
   }

   // Compare the District part of Address using the rules of DIN 5007-2 norm
   std::string str_this_District = normalizeDIN(strDistrict);
   std::string str_other_District = normalizeDIN(other.strDistrict);
   if (str_this_District != str_other_District) {
      return str_this_District < str_other_District;
   }

   // Compare zipcode for the address
   if (strZipCode != other.strZipCode) {
      return strZipCode < other.strZipCode;
   }

   // Compare the street part of Address using the rules of DIN 5007-2 norm
   std::string str_this_Street = normalizeDIN(strStreet);
   std::string str_other_Street = normalizeDIN(other.strStreet);
   if (str_this_Street != str_other_Street) {
      return str_this_Street < str_other_Street;
   }

   // Compare the street number part of address, split number and addition
   // sort number as numeric and addition alphanumeric
   int         i_this_StreetNumber;
   std::string str_this_StreetNumberAddition;
   int         i_other_StreetNumber;
   std::string str_other_StreetNumberAddition;
   Parse_StreetNumber(strStreetNumber, i_this_StreetNumber, str_this_StreetNumberAddition);
   Parse_StreetNumber(other.strStreetNumber, i_other_StreetNumber, str_other_StreetNumberAddition);

   if (i_this_StreetNumber != i_other_StreetNumber) {
      return i_this_StreetNumber < i_other_StreetNumber;
   }

   return str_this_StreetNumberAddition < str_other_StreetNumberAddition;
}

std::string TAddress::normalizeDIN(std::string const& strText) {
   std::string strResult = ""s;
   for (auto c : strText) {
      switch (c) {
      case '�': strResult += "ae"; break;
      case '�': strResult += "oe"; break;
      case '�': strResult += "ue"; break;
      case '�': strResult += "ss"; break;
      case '�': strResult += "Ae"; break;
      case '�': strResult += "Oe"; break;
      case '�': strResult += "Ue"; break;
      case '�': strResult += 'a';  break;
      case '�': strResult += 'e';  break;
      case '�': strResult += 'e';  break;
      case '�': strResult += 'c';  break;
      case '�': strResult += 'n';  break;
      default:  strResult += c;
      }
   }
   return strResult;
}

void TAddress::Parse_StreetNumber(std::string const& strInput, int& streetnumber, std::string& addition) {
   std::istringstream ins(strInput);
   ins >> streetnumber >> addition;
   std::transform(addition.begin(), addition.end(), addition.begin(), ::tolower);
}