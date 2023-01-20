#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <tuple>
#include <locale>
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

TDateDiff operator + (TDateDiff const& op1, TDateDiff const& op2) {
   return { op1.iDays + op2.iDays, op1.iMonths + op2.iMonths, op1.iYears + op2.iYears };
   }

TDateDiff operator - (TDateDiff const& op1, TDateDiff const& op2) {
   return { op1.iDays - op2.iDays, op1.iMonths - op2.iMonths, op1.iYears - op2.iYears };
   }


   
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
      TDate(TDate const& ref) : data(ref.data) { }

      TDate(std::tm const& ref, std::string const& format = dateformat) { 
         strLocFormat = format;
         data = std::make_tuple(ref.tm_year + 1900, ref.tm_mon + 1, ref.tm_mday); 
         }

      TDate(std::time_t const& ref) : TDate(*std::localtime(&ref)) { 
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
    TDate date = "29.02.2020"s;
    TDate date2(date);
    date2.AddMonths(3);
    date.Format("%m/%d/%Y");
    date += 1_years + 2_months + 7_days + 2_weeks;
    std::cout << "Das Datum ist " << date << "\n";
    std::cout << "2. Datum " << date2 << "\n";
    std::cout << date2 - date << "\n";
    std::cout << "Kontrolle " << date2 + (date2 - date) << "\n";
    std::getchar();
    }
