// Regolith Reservoir
// 498,4 -> 498,6 -> 496,6
// 503,4 -> 502,4 -> 502,9 -> 494,9
// rock = '#', air = '.', source = '+', sand = 'o'

#define _CRT_SECURE_NO_WARNINGS

#include "AOC_Day14.h"
#include "AOC_Read.h"

#include <memory>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std::literals::string_literals;

namespace Day14 {

   class Help_Basisklasse {
      friend class BasisKlasse;
   private:
      char* szName;
      char* szFirstname;

   public:
      Help_Basisklasse(void) {
         szName = 0;
         szFirstname = 0;
         }

      virtual ~Help_Basisklasse(void) {
         delete [] szName;
         delete [] szFirstname;
      }

   };

   class BasisKlasse : private Help_Basisklasse {
   public:
      BasisKlasse(const char * const pName, const char* const pFirstname) : Help_Basisklasse() {
         szName = static_cast<char*>(malloc(strlen(pName) + 1));
         strcpy(szName, pName);

         szFirstname = static_cast<char*>(malloc(strlen(pFirstname) + 1));
         strcpy(szFirstname, pFirstname);
         }


   };

   void Testcase0(void) {
      BasisKlasse person[] = { { "Mustermann", "Manfred" }, { "Musterfrau", "Gisela" } };

      }
 
   class TBasis {
   public:
      virtual ~TBasis(void) = default;
      virtual void test(void) = 0;
      //void test(void) { std::cout << "test called"; }
      };

   void TBasis::test(void) { std::cout << "test called"; }

   class Ableitung : public TBasis {
   public:
      virtual void test(void) override { TBasis::test(); std::cout << " in Ableitung\n"; }
   };

   void Testcase(void) {
      // TBasis x;    // abstract class can't created
      std::unique_ptr<TBasis> a( new Ableitung() );
      a->test();
   }


   class TBasis2 {
   public:
      virtual ~TBasis2(void) = default;
      virtual void test(void) { std::cout << "test called\n"; } 
   };

   class Ableitung2 : public TBasis2 {
   public:
      virtual void test(void) = 0;
   };

   void Testcase2(void) {
      // Ableitung2 x;  // abstract class can't created
      std::unique_ptr<TBasis2> a(new TBasis2());
      a->test();

      }

 class TTest3 {
 private:
    int iValue = 0;
    mutable int iTest = 0;
 public:
    TTest3(void) = default;
    TTest3(TTest3 const&) = default;
    ~TTest3(void) = default;


    void Test(TTest3 const& ref) { ref.iTest = iValue;  ref.Print(); }

    void Print(void) const& { std::cout << "const lvalue qualified with value " << iValue << "\n"; }
    void Print(void) & {  std::cout << "lvalue qualified with value " << iValue << "\n"; }
    void Print(void) && { std::cout << "rvalue qualified with value " << iValue << "\n"; }
 };

 auto Testcase3(void) -> int {
    TTest3 test;
    test.Test(test);
    TTest3().Print();
    return 0;
    }

 /** test 4 - example for conversion constructors and operators
 
 */
  class TTest4 {
     std::string strText = ""s; ///< variable to test conversions
  public:
     /// default constructor
     TTest4(void) = default;
     /// copy constructor
     TTest4(TTest4 const& ref) : strText(ref.strText) { }
     /// rvalue (pirates) constructor
     TTest4(TTest4&& ref) noexcept { std::swap(strText, ref.strText);  }
     /// initialize constructor
     TTest4(std::string const& newVal) : strText(newVal) { }

     ~TTest4(void) = default; ///< destructor for our test class

     // valid construction constructor with C++98
     TTest4(int iValue) : strText( std::to_string(iValue) ) {  }

     TTest4(double flValue, int iScale) { 
        std::ostringstream os;
        os.setf(std::ios::showpoint);
        os.setf(std::ios::fixed);
        os << std::setprecision(iScale) << flValue;
        strText = os.str(); 
        }

     TTest4& operator = (TTest4 const& ref) {
        strText = ref.strText;
        return *this;
        }


    explicit operator int () {  return std::stoi(strText);  }
    explicit operator long long() { return std::stoll(strText); }

 };

  void Testcase4(void) {
     double iVal = 5;
     TTest4 test;
     test = iVal;
     int iWert = static_cast<int>(test);

     }


   // - ------------------------------------------------------

   using Coordinate_ty = unsigned int;

   class TCoordinate {
      friend std::ostream& operator << (std::ostream& out, TCoordinate const& data) {
         out << data.the_x << "," << data.the_y;
         return out;
         }

      friend std::istream& operator >> (std::istream& iss, TCoordinate& data) {
         char c;
         iss >> std::skipws >> data.the_x >> c >> data.the_y;
         return iss;
         }

   private:
      Coordinate_ty the_x = 0;
      Coordinate_ty the_y = 0;


   public:
      TCoordinate(void) : TCoordinate(0, 0) { };
      TCoordinate(Coordinate_ty const& p_x, Coordinate_ty const& p_y) {
         the_x = p_x;
         the_y = p_y;
         }

      TCoordinate(TCoordinate const& ref) {
         _Copy(ref);
         }

      TCoordinate(std::string const& val) {
         std::istringstream iss(val);
         iss >> *this;
      }

      virtual ~TCoordinate(void) = default;

      // assignment operator
      TCoordinate& operator = (TCoordinate const& ref) {
         _Copy(ref);
         return *this;
         }

      operator std::string () const {
         std::ostringstream os;
         os << *this;
         return os.str();
         }

      virtual void Init(void) {
         _Init();
      }

      virtual void Copy(TCoordinate const& ref) {
         _Copy(ref);
         }

      // Selektoren
      Coordinate_ty const& X(void) const { return the_x; }
      Coordinate_ty const& Y(void) const { return the_y; }
      // Manipulatoren
      void X(Coordinate_ty const& newValue) { the_x = newValue; }
      void Y(Coordinate_ty const& newValue) { the_y = newValue; }

   private:
      void _Init(void) noexcept {
         the_x = 0;
         the_y = 0;
         }

      void _Copy(TCoordinate const& ref) noexcept {
         the_x = ref.the_x;
         the_y = ref.the_y;
         }

      };

   enum class EMaterial : char { undefined = 0,  rock = '#', air = '.', source = '+', sand = 'o' };

   void ReadValues_1(std::string const& strFilename) {
      //Testcase();
      //Testcase2();
      //Testcase3();
      Testcase4();
      }

   void ReadValues_2(std::string const& strFilename) {
      }
}
