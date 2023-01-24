#ifndef Date_H
#define Date_H

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <locale>
#include <tuple>
#include <array>
#include <ctime>

using namespace std::literals;
       
class TDateDiff {
    friend TDateDiff operator + (TDateDiff const& op1, TDateDiff const& op2);
    friend TDateDiff operator - (TDateDiff const& op1, TDateDiff const& op2);

    private:
       int iDays;
       int iMonths;
       int iYears;
    public: 
       TDateDiff(int days, int months, int years) : iDays(days), iMonths(months), iYears(years) { }
       TDateDiff(TDateDiff const& ref) : iDays(ref.iDays), iMonths(ref.iMonths), iYears(ref.iYears) { }
       int Days() const { return iDays; }
       int Months() const { return iMonths; }
       int Years() const { return iYears; }
   };

inline TDateDiff operator""_days(unsigned long long days) { return { static_cast<int>(days), 0, 0 }; }
inline TDateDiff operator""_months(unsigned long long months) { return { 0, static_cast<int>(months), 0 }; }
inline TDateDiff operator""_years(unsigned long long years) { return { 0, 0, static_cast<int>(years) }; }
inline TDateDiff operator""_weeks(unsigned long long weeks) { return { static_cast<int>(weeks) * 7,  0, 0 }; }


/// Klasse um Datumsfunktionen bereitzustellen und Literale zu zeigen
class TDate {
   friend std::ostream& operator << (std::ostream&, TDate const&);
   friend  std::istream& operator >> (std::istream&, TDate&);

   friend TDate operator + (TDate const& daValue, int iDay);
   friend TDate operator + (TDate const& daValue, TDateDiff const& datediff);
   friend TDate operator - (TDate const& daValue, int iDay);
   friend TDate operator - (TDate const& daValue, TDateDiff const& datediff);
   friend long  operator - (TDate const& daValue1, TDate const& daValue2);

   private:
      static inline std::string dateformat = "%d.%m.%Y"s;
      std::string strLocFormat;
      std::tuple<int, int, int> data;
   public:
      TDate(void) = default;

      TDate(int it, int im, int ij, std::string const& format = dateformat) : data(ij, im, it) { 
         strLocFormat = format;
         }

      TDate(TDate const& ref) : data(ref.data), strLocFormat(ref.strLocFormat) { }

      TDate(std::tm const& ref, std::string const& format = dateformat) { 
         strLocFormat = format;
         data = std::make_tuple(ref.tm_year + 1900, ref.tm_mon + 1, ref.tm_mday); 
         }

      TDate(std::time_t const& ref) : TDate(*std::localtime(&ref), strLocFormat) { 
         }

      TDate(std::string const& ref, std::string const& format = dateformat) {
         strLocFormat = format;
         std::istringstream ins(ref);
         ins >> *this;
         }

      ~TDate(void) = default;

      TDate& operator = (TDate const& ref) {
         data = ref.data;
         return *this;
         }

      TDate& operator = (std::tm const& ref) {
         *this = TDate(ref);
         return *this;
         }

      TDate& operator = (std::string const& ref) {
         *this = TDate(ref);
         return *this;
         }


      // Konversion Operatoren
      operator std::tm() const { 
         std::tm ts;
         ts.tm_year = Year() - 1900;
         ts.tm_mon  = Month() - 1;
         ts.tm_mday    = Day();

         ts.tm_hour =  ts.tm_min = ts.tm_sec = 0; 
         return ts;
         }

      operator std::string() const {
         std::ostringstream os;
         os << *this;
         return os.str();
         }


      // mathematische Operatoren
      TDate& operator += (int iDays) { return AddDays(iDays); }
      TDate& operator += (TDateDiff val) { return AddDiff(val); }

      TDate& operator -= (int iDays) { return AddDays(-iDays); }
      TDate& operator ++ () { return operator += (1); }
      TDate& operator -- () { return operator -= (1); };

      TDate operator ++ (int) {  TDate ret(*this); operator += (1); return ret; }
      TDate operator -- (int ) { TDate ret(*this); operator -= (1); return ret; }


      // Selektoren
      int Year() const { return std::get<0>(data); }
      int Month() const { return std::get<1>(data); }
      int Day() const { return std::get<2>(data); }
      std::string const& Format() const { return strLocFormat; }
      bool IsLeapYear() const { return IsLeapYear(Year()); }

      /// \name Manipulatoren der Datenklasse
      /// \{
      int Year(int newVal)  { return std::get<0>(data) = newVal; }
      int Month(int newVal) { return std::get<1>(data) = newVal; }
      int Day(int newVal)   { return std::get<2>(data) = newVal; }
      std::string const& Format(std::string const& newVal)  { return strLocFormat = newVal; }

      static void SetDefaultFormat(std::string const& format) {  dateformat = format; }
      /// \}

      TDate& AddDays(int idays);
      TDate& AddYears(int iYears);   
      TDate& AddMonths(int iMonths);
      static long Difference(TDate const& date1, TDate const& date2);
    //  long Difference360(TDate const& date1, TDate const& date2);


      /// \name relationale Operatoren für Instanzen der Klasse
      /// \{
      constexpr bool operator == (TDate const& ref) const { return _Compare(ref) == 0; }
      constexpr bool operator != (TDate const& ref) const { return _Compare(ref) != 0; }
      constexpr bool operator <  (TDate const& ref) const { return _Compare(ref) <  0; }
      constexpr bool operator <= (TDate const& ref) const { return _Compare(ref) <= 0; }
      constexpr bool operator >  (TDate const& ref) const { return _Compare(ref) >  0; }
      constexpr bool operator >= (TDate const& ref) const { return _Compare(ref) >= 0; }
      /// \}      

      static TDate Current(void) { 
         std::time_t t;
         std::time(&t);
         TDate ret(t);
         return ret;
         }

      static bool IsLeapYear(int iYear) { 
	      bool boRetVal = false;
	      if((iYear % 400) == 0)	    boRetVal = true;
	      else if((iYear % 100) == 0) boRetVal = false;
		   else if((iYear % 4) == 0)   boRetVal = true;
         else                        boRetVal = false;
	      return boRetVal;
         }

      /// Instanz- Methode, die die Anzahl der Tage im aktuellen Monat zurück gibt
      int DaysInMonth() const { return DaysInMonth(Year(), Month()); }


      /// Klassen- Methode, die die Anzahl der Tage im Monat im Jahr zurückgibt
      
      static int DaysInMonth(int iYear, int iMonth) {
         if(iMonth == 2 && IsLeapYear(iYear)) return 29;
         return _DaysInMonth()[iMonth];
         }

   private:
      TDate& AddDiff(TDateDiff const& diff) {
         AddYears(diff.Years());
         AddMonths(diff.Months());
         AddDays(diff.Days());
         return *this;
         }

      int _Compare(TDate const& ref) const {
         if(data < ref.data) return -1;
         if(data > ref.data) return  1;
         return 0;
         }

      static std::array<int, 13> const& _DaysInMonth(void) {
         static std::array<int, 13> intern = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
         return intern;
         }

      static std::array<int, 13> const& _Days2Month(void) {
         static std::array<int, 13> intern = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
         return intern;
         }


   };



#endif
