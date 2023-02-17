#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <functional>
#include <type_traits>

 using namespace std::placeholders;

template <typename ty>
bool Sort_greater(ty const& lhs, ty const& rhs) {
   return lhs > rhs;
}

template <typename ty>
bool Sort_lesser(ty const& lhs, ty const& rhs) {
   return lhs < rhs;
}


template <>
bool Sort_lesser<double>(double const& lhs, double const& rhs) {
   return lhs < rhs;
}



template <typename ty>
bool Output(ty const& value) {
   std::cout << std::setw(3) << value << ",";
   return true; 
   }

/*
bool Lesser_15(int element) {
   return element < 15;
   }
*/

template <typename ty>
class TTestLesser {
   private:
      ty aMax;
   public:
      TTestLesser() = delete;
      TTestLesser(TTestLesser const&) = default;
      TTestLesser(ty iVal) : aMax(iVal) { }

      bool operator ()(ty const& element) {
         return element < aMax;
         }

      bool compare(ty const& element) {
         return element < aMax;  
         }

};

template <typename ty>
class TOutput {
   private:
      int iWidth   = 3;
      int iColumns = 6;
      int iCounter = 0;
      std::ostream& out;
   public:
      TOutput() : TOutput(8, 3, std::cout) { }
      TOutput(int ic, int iw, std::ostream& stream = std::cout) : out(stream), iColumns(ic), iWidth(iw), iCounter(0) { }
      TOutput(std::ostream& stream = std::cout) : TOutput(8, 3, stream) { }
      TOutput(TOutput const& ref) : out(ref.out), iColumns(ref.iColumns), iWidth(ref.iWidth), iCounter(ref.iCounter) { }

      bool operator()(ty const& value) {
         if(iCounter > 0 && iCounter % iColumns == 0) out << "\n";
         out << (iCounter++ % iColumns == 0 ? "" : ",");
         out << std::setw(iWidth) << value;
         return true; 
         }

      bool operator()(void) { out << "\n"; return true; }   

};


template<typename ty>
using my_functions = std::vector<std::function<bool (ty const&)>>;

template <typename ty>
void Test(std::vector<ty>& values, my_functions<ty>& stack, 
          std::function<bool (ty const&, ty const&)> fkt) {

   auto it = std::partition(values.begin(), values.end(), stack[0]);
   std::sort(values.begin(), it, fkt);
   std::for_each(values.begin(), it, stack[1]);

   }

template <typename ty>
void Test(std::vector<ty>& values, 
          std::function<bool (ty const&, ty const&)> fkt, 
          std::function<bool (ty const&)> split, 
          std::function<bool (ty const&)> out) { 
   auto it = std::partition(values.begin(), values.end(), split);
   std::sort(values.begin(), it, fkt);
   
   std::for_each(values.begin(), it, out);
   }





bool is_lesser_than(int elem, int max_value) {
   return elem < max_value;
   }

/*
template <typename ty>
bool Sort_lesser(ty lhs, ty rhs) {
   return lhs < rhs;
}
*/



int main(void) {

   std::cout.setf(std::ios::showpoint);
   std::cout.setf(std::ios::fixed);
   std::cout.precision(1);

   double test = 100'000;

   //std::vector<int> values = { 12, 3, 54, 13, 4, 63, 14, 7, 23, 5, 35, 1, 65, 15, 9, 6, 11, 41 };
   using my_type = double;
   std::vector<my_type> values = { 12.0, 3.0, 54.0, 13.0, 4.0, 63.0, 14.0, 7.0, 23.0, 5.0, 35.0, 
                                  1.0, 65.0, 15.0, 9.0, 6.0, 11.0, 41.0 };

   

   //Test<double>(values, Sort_greater<double>, TTestLesser<double>(15.0), TOutput<double>(5, 5));
   TTestLesser<my_type> sort_lesser(15.0);
   my_functions<my_type> funcs = { std::bind(&TTestLesser<my_type>::compare, &sort_lesser, _1), 
                                   TOutput<my_type>(5, 5) }; 
   //Test<my_type>(values, std::bind(Sort_lesser<my_type>, _2, _1), std::bind(Sort_lesser<my_type>, _1, 15.0), TOutput<my_type>(5, 5));
   //Test<double>(values, Sort_greater<double>, [](double const& val) { return val < 15.0; }, TOutput<double>(5, 5));
   Test<my_type>(values, funcs, std::bind(Sort_lesser<my_type>, _2, _1) );
   getchar();
}
