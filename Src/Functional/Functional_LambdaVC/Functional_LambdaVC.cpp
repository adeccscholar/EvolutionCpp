#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

class TMyLesser {
private:
   int max_val;
public:
   TMyLesser() = delete;
   TMyLesser(int param) : max_val(param) { }
   TMyLesser(TMyLesser const& ref) : max_val(ref.max_val) { }
   TMyLesser(TMyLesser&& ref) noexcept { std::swap(max_val, ref.max_val); }
   ~TMyLesser(void) { }

   bool operator ()(int const& val) { return val < max_val; }
};


bool is_lesser_than_15(int val) { return val < 15; }


bool is_odd(int val) { return val % 2 != 0; }

using IntVect = std::vector<int>;
typedef IntVect::iterator        itIntVect;
typedef IntVect::const_iterator  citIntVect;

int main(void) {
   std::vector<int> values = { 12, 54, 13, 19, 32, 16, 8, 15, 17, 23, 35, 7 };
   auto it = std::partition(values.begin(), values.end(), TMyLesser(15));
}
