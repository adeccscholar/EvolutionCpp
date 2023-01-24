
#include "Date_TestValues.h"
#include "Date.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <tuple>
#include <locale>
#include <vector>
#include <array>
#include <ctime>


TDateDiff operator + (TDateDiff const& op1, TDateDiff const& op2) {
   return { op1.iDays + op2.iDays, op1.iMonths + op2.iMonths, op1.iYears + op2.iYears };
   }

TDateDiff operator - (TDateDiff const& op1, TDateDiff const& op2) {
   return { op1.iDays - op2.iDays, op1.iMonths - op2.iMonths, op1.iYears - op2.iYears };
   }


   

std::ostream& operator << (std::ostream& out, TDate const& data) {
    std::ostream::sentry cerberus(out);
    if(!bool(cerberus)) return out;
    
    std::tm tmbuf = data;
    std::time_put<char> const& timefacet = std::use_facet<std::time_put<char>>(out.getloc());
    std::string pattern = data.Format().length() > 0 ? data.Format() : std::string("%d.%m.%Y");
    if(timefacet.put(out, out, out.fill(), &tmbuf, pattern.data(), pattern.data() + pattern.length()).failed())  
       out.setstate(out.badbit);
    return out;
   }


 std::istream& operator >> (std::istream& instr, TDate& data) {
    instr.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
    std::istream::sentry cerberus(instr);
    if(bool(cerberus)) {
       std::ios::iostate state;
       std::tm tmbuf;
       std::string pattern = data.Format().length() > 0 ? data.Format() : std::string("%d.%m.%Y");
       std::time_get<char> const& timefacet = std::use_facet<std::time_get<char>>(instr.getloc());
       //std::string strName = instr.getloc().name();
       using strbuf = std::istreambuf_iterator<char>;
       strbuf from = strbuf(instr); 
       strbuf end = strbuf();
       timefacet.get(from, end, instr, state, &tmbuf, pattern.data(), pattern.data() + pattern.length()); 
       data = tmbuf;
       }
    return instr;    
    }


TDate operator + (TDate const& daValue, int iDay) {
   TDate ret(daValue);
   return ret += iDay;
   }

TDate operator + (TDate const& daValue, TDateDiff const& datediff) {
   TDate ret(daValue);
   ret.AddYears(datediff.Years());
   ret.AddMonths(datediff.Months());
   ret.AddDays(datediff.Days());
   return ret;
   }

TDate operator - (TDate const& daValue, int iDay) {
   TDate ret(daValue);
   return ret -= iDay;
   }

TDate operator - (TDate const& daValue, TDateDiff const& datediff) {
   TDate ret(daValue);
   ret.AddYears(-datediff.Years());
   ret.AddMonths(-datediff.Months());
   ret.AddDays(-datediff.Days());
   return ret;
   }

long  operator - (TDate const& daValue1, TDate const& daValue2) {
   return TDate::Difference(daValue2, daValue1);
   }


TDate& TDate::AddDays(int iDays) {
   int iD = Day();
   int iM = Month();
   int iY = Year();
   int m;

   if(iDays > 0) {
     iD += iDays;
     while(iD > (m = DaysInMonth(iY, iM))) {
        iD -= m;
        if(++iM > 12) {
           iY++;
           iM = 1;
           }
        }
     }
   else {
      iD += iDays;
      while(iD <= 0) {
          if(--iM < 1) {
             --iY;
             iM = 12;
             }
          iD += DaysInMonth(iY, iM);
          }
      }
   Day(iD);
   Month(iM);
   Year(iY);
   return *this;
   }

TDate& TDate::AddYears(int iYears) {
   bool boLeap = Month() == 2 && Day() > 28 ? true : false;
   int iD = Day();
   int iM = Month();
   int iY = Year();

   iY += iYears;
   if(boLeap) {
      if(IsLeapYear(iY)) iD = 29;
      else iD = 28;;
      }

   Day(iD);
   Month(iM);
   Year(iY);

   return *this; 
   }

TDate& TDate::AddMonths(int iMonths) {
   int iYears = iMonths / 12;

   if(iMonths < 0) {
      iMonths %= 12;
      }
   else {
      iMonths %= 12;
      }
   AddYears(iYears);

   Month(Month() + iMonths);
   if(Month() < 0) {
      Month(12 - Month());
      AddYears(-1);
      }
   else { 
      if(Month() > 12) {
         Month(Month()- 12);
         AddYears(1);
         }
      }
   return *this;
   }


long TDate::Difference(TDate const& date1, TDate const& date2) {
   long lRetVal = 0l;
   TDate dat1(date1), dat2(date2);
   while(dat2.Year() < dat1.Year()) {
      lRetVal += 365;
      if(dat2.IsLeapYear() && (dat2.Month() <= 2))  lRetVal++;
      dat2.Year(dat2.Year() + 1);
      if(dat2.IsLeapYear() && (dat2.Month() > 2))   lRetVal++; 
      }

   if(dat1 < dat2) return lRetVal - Difference(dat2, dat1);

   while(dat2.Month() < dat1.Month()) {
      lRetVal += TDate::_DaysInMonth()[dat2.Month()];
      dat2.Month(dat2.Month() + 1);
      }
 
   if(dat1 < dat2) lRetVal -= dat2.Day() - dat1.Day();
   else            lRetVal += dat1.Day() - dat2.Day();
   return lRetVal;
   }




int main(void) {
/*
    TDate date = "29.02.2020"s;
    TDate date2(date);
    date2.AddMonths(3);
    date.Format("%m/%d/%Y");
    date += 1_years + 2_months + 7_days + 2_weeks;
    std::cout << "Das Datum ist " << date << "\n";
    std::cout << "2. Datum " << date2 << "\n";
    std::cout << date2 - date << "\n";
    std::cout << "Kontrolle " << date2 + (date2 - date) << "\n";
  
*/
  TDate::SetDefaultFormat("%m/%d/%Y"s);
  DateTest::vecTestStreams stream_cases;
  DateTest::Init(stream_cases);
  for(auto iVariante : { 1, 5, 7, 8 }) {
     size_t iErrors, iLines;
     switch(iVariante) {
        case 1: std::tie(iLines, iErrors) = DateTest::Test<1>(stream_cases, std::cerr);
                break;
        case 2: std::tie(iLines, iErrors) = DateTest::Test<2>(stream_cases, std::cerr);
                break;
        case 3: std::tie(iLines, iErrors) = DateTest::Test<3>(stream_cases, std::cerr);
                break;
        case 4:
           std::tie(iLines, iErrors) = DateTest::Test<4>(stream_cases, std::cerr);
           break;
        case 5:
           std::tie(iLines, iErrors) = DateTest::Test<5>(stream_cases, std::cerr);
           break;
        case 6:
           std::tie(iLines, iErrors) = DateTest::Test<6>(stream_cases, std::cerr);
           break;
        case 7:
           std::tie(iLines, iErrors) = DateTest::Test<7>(stream_cases, std::cerr);
           break;
        case 8:
           std::tie(iLines, iErrors) = DateTest::Test<8>(stream_cases, std::cerr);
           break;
        case 9:
           std::tie(iLines, iErrors) = DateTest::Test<9>(stream_cases, std::cerr);
           break;
         }
     std::cout << "Test fuer istream " << iVariante << " mit " << iLines << " durchgefuehrt, " 
               << iErrors << " Fehler.";

     }
  std::getchar();
    }
