#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <type_traits>



template <typename ty>
bool Sort_greater(ty const& lhs, ty const& rhs) {
   return lhs > rhs;
}

template <typename ty>
void Output(ty const& value) {
   std::cout << std::setw(3) << value << ",";
   return;
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
};

template <typename ty>
class TOutput {
private:
   int iWidth = 3;
   int iColumns = 6;
   int iCounter = 0;
   std::ostream& out;
public:
   TOutput() : TOutput(8, 3, std::cout) { }
   TOutput(int ic, int iw, std::ostream& stream = std::cout) : out(stream), iColumns(ic), iWidth(iw), iCounter(0) { }
   TOutput(std::ostream& stream = std::cout) : TOutput(8, 3, stream) { }
   TOutput(TOutput const& ref) : out(ref.out), iColumns(ref.iColumns), iWidth(ref.iWidth), iCounter(ref.iCounter) { }

   void operator()(ty const& value) {
      if (iCounter > 0 && iCounter % iColumns == 0) out << "\n";
      out << (iCounter++ % iColumns == 0 ? "" : ",");
      out << std::setw(iWidth) << value;
      return;
   }

   void operator()(void) { out << "\n"; }

};



template <typename ty>
void Test(std::vector<ty>& values,
          std::function<bool(ty const&, ty const&)> fkt,
          std::function<bool(ty const&)> split,
          std::function<void(ty const&)> out) {
   auto it = std::partition(values.begin(), values.end(), split);
   std::sort(values.begin(), it, fkt);
   std::for_each(values.begin(), it, out);
}





bool is_lesser_than(int elem, int max_value) {
   return elem < max_value;
}

template <typename ty>
bool Sort_lesser(ty lhs, ty rhs) {
   return lhs < rhs;
}


#include <random>
#include <chrono>

enum class EDistribution : int { normal, uniform, undefined = 999 };

inline std::vector<double> Build(EDistribution distr = EDistribution::uniform,
                          size_t count = 100'000,
                          std::pair<double,double> const& limits = { -1'000.0, 1'000.0 }) {
   using myParam = std::pair<double, double>;

   static std::map<EDistribution, std::function<std::function<double()>(myParam const&)>> funct = {
      { EDistribution::normal,  [](myParam const& limits) {
             std::mt19937 rndNbr;
             rndNbr.seed(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));
             double middle = limits.first + (limits.second - limits.first) / 2;
             double range = (limits.second - limits.first) / 4;
             std::normal_distribution<double> distNbr(middle, range);
             return std::bind(distNbr, rndNbr);
             }    },
      { EDistribution::uniform, [](myParam const& limits) {
             std::mt19937 rndNbr;
             rndNbr.seed(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));
             std::uniform_real_distribution<double> distNbr(limits.first, limits.second);
             //distNbr.param(std::uniform_real_distribution<double>(limits.first, limits.second).param());
             return std::bind(distNbr, rndNbr);
             }    }
      // { EDistribution::normal, [](myParam const& limits) {}    }

   };

   if (limits.first >= limits.second) throw std::range_error("error for calling Build, wrong parameter for limits");
   std::vector<double> data(count);

   if(auto it = funct.find(distr); it != funct.end()) {
      auto generator = it->second(limits);
      for (auto& val : data) {
         do {
            val = generator();
            } 
         while (val < limits.first || val > limits.second);
         }
      }
   else {
      throw std::runtime_error("error: entry for Distribution not found");
      }


   return data;
   }

int main(void) {

   std::cout.setf(std::ios::showpoint);
   std::cout.setf(std::ios::fixed);
   std::cout.precision(1);

   //std::vector<int> values = { 12, 3, 54, 13, 4, 63, 14, 7, 23, 5, 35, 1, 65, 15, 9, 6, 11, 41 };
   std::vector<double> values = { 12.0, 3.0, 54.0, 13.0, 4.0, 63.0, 14.0, 7.0, 23.0, 5.0, 35.0,
                                  1.0, 65.0, 15.0, 9.0, 6.0, 11.0, 41.0 };
   using used_type = decltype(values)::value_type;
   Test<used_type>(values, Sort_greater<used_type>, TTestLesser<used_type>(15.0), TOutput<used_type>(5, 5));
   std::cout << "\n\n";
   Test<used_type>(values, Sort_greater<used_type>, [](used_type const& val) { return val < 15.0; }, TOutput<used_type>(5, 5));
   auto test_data = Build(EDistribution::uniform, 500'000);
   std::cout << "\n" << test_data.size() << "\n";
  // getchar();
}