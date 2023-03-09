#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <stdexcept>

#include <thread>
#include <future>
#include <mutex>


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

int mult_2(int& val) { return val *= 2; }

using IntVect = std::vector<int>;
typedef IntVect::iterator        itIntVect;
typedef IntVect::const_iterator  citIntVect;




template <typename iterator, typename Pred, typename Operation>
void for_each_if(iterator begin, iterator end, Pred pred, Operation op) {
   if (begin < end) {
      for (; begin != end; ++begin) if (pred(*begin)) op(*begin);
      }
   else throw std::range_error("iterator begin isn't before end iterator");
   return;
   }

struct my_map_not_found { };

template <typename ty>
typename ty::mapped_type& my_map_find(ty& container, typename ty::key_type const& value) {
   auto it = container.find(value);
   if (it == container.end()) throw my_map_not_found();
   else return it->second;
   } 

int main(void) {
   std::vector<int> values = { 12, 54, 13, 19, 32, 16, 8, 15, 17, 23, 35, 7 };
   std::map<int, int> test = { {   1, 25 }, {  3,  27 }, {   2, 25 }, { 6, 26  },
                               {  11, 25 }, {  13, 27 }, {  12, 25 }, { 16, 26 },
                               {  21, 25 }, {  23, 27 }, {  22, 25 }, { 16, 26 } };
   auto it = std::partition(values.begin(), values.end(), is_odd);

   auto thread_function1 = [](auto begin, auto end, std::function<void(int&)> func) {
      std::for_each(begin, end, func);
   };

   std::vector<std::thread> t;
   t.push_back(std::thread(thread_function1, values.begin(), it, [](auto& val) mutable { val *= 2; } ));
   t.push_back(std::thread(thread_function1, it, values.end(), [](auto& val) mutable { val *= 3; }));

   for (auto& val : t) val.detach();
   //std::for_each(values.begin(), it, [](auto& val) mutable { val *= 2; });
   //std::for_each(it, values.end(),   [](auto& val) mutable { val *= 3;  });



   for_each(values.begin(), values.end(), [](auto& val) { std::cout << val << ", ";  });
   std::cout << "\n";
   for_each_if(values.begin(), values.end(), TMyLesser(20), [](auto& val) mutable { return val *= 2; });
   for_each(values.begin(), values.end(), [](auto& val) { std::cout << val << ", ";  });
   std::cout << "\n";

   try {
      std::cout << my_map_find(test, 36) << " found.\n";
   }
   catch(my_map_not_found) {
      std::cout << "key not found.\n";
      }


   std::cout << "\n";
   }
