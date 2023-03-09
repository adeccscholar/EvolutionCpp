#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <utility>
#include <algorithm>
#include <type_traits>

class TMyLesser {
   private:
      int max_value;
   public:
      TMyLesser(int param) : max_value(param) { }
      TMyLesser(TMyLesser const& ref) : max_value(ref.max_value) { }
      ~TMyLesser(void) { }

      bool operator ()(int val) { return val < max_value; } 
};


bool is_lesser_than_15(int val) { 
    return val < 15; 
    }


bool is_odd(int val) { return val % 2 != 0; }


template <typename ty>
bool sort_lesser(ty const& a, ty const& b) { return a < b; }

using IntVect = std::vector<int>;
typedef IntVect::iterator        itIntVect; 
typedef IntVect::const_iterator  citIntVect; 

/*
capture
parameter
rückgabewert
funktions- body
*/


template <typename ty, typename _ = void>
struct is_vector {
  static constexpr bool value = false;
};

template <typename ty>
struct is_vector<ty, typename std::enable_if<std::is_same<ty, std::vector<typename ty::value_type,
                                             typename ty::allocator_type>>::value>::type> {
  static constexpr bool value = true;
};

template <typename ty>
void Sortiere(ty& values) { 
   //using used_type  = typename std::remove_reference<typename std::remove_cv<decltype(values)>::type>::type;
   using used_type  = typename std::remove_reference<decltype(values)>::type;
   static_assert(is_vector<used_type>::value, "parameter value isn't a vector");
    std::sort(values.begin(), values.end(), sort_lesser<typename used_type::value_type>);
   }









template <typename ty, int dim>
class MyArray {

   public:
    using value_type        = ty;
    using difference_type   = std::ptrdiff_t;
    using reference_type    = value_type&;
    using pointer_type      = value_type*;


    struct my_iterator {
    using iterator_category = std::input_iterator_tag;


    my_iterator() {  }
    my_iterator(value_type p[]) : data(p) {  
       end_pos = sizeof(p) / sizeof(p[0]);
       ++*this; 
       }

      
    my_iterator& operator = (value_type p[]) {
       data = p;
       start_pos = -1;
       end_pos   = 0;
       return *this;
       }

    my_iterator& operator = (my_iterator const& ref) {
       data      = ref.data;
       start_pos = ref.start_pos;
       end_pos   = ref.end_pos;;
       return *this;
       }

    reference_type operator*() { return data[start_pos];  }
    pointer_type operator->() { return &data[start_pos]; }


    reference_type operator [](int par) {

    my_iterator& operator++() {
       if(start_pos + 1 < sizeof(data)/sizeof(data[0])) {
          ++start_pos;
          }
       else {
          start_pos = -1;
          }
       return *this;
       }


    my_iterator operator++(int) {
       my_iterator copy(*this);
       ++*this;
       return copy;
       }
     
    friend bool operator == (my_iterator const& lhs, my_iterator const& rhs) {
       return lhs.data == rhs.data && lhs.start_pos == lhs.start_pos;
       }

    friend bool operator != (my_iterator const& lhs, my_iterator const& rhs) {
       return !(lhs == rhs);
       }

    private:
       value_type* data = nullptr;
       int start_pos    = -1;
       int end_pos      = 0; 
    };






   private:
      ty _data[dim];  
   public:
      my_iterator begin() { return my_iterator(_data); }
      my_iterator end()  { return my_iterator(); }
      
      ty& data() { return _data; }
      ty const& data() const { return _data; }

};


std::array<int, 5>  feld;
int feld1[5];


int main(void)  {
   std::vector<int> values = { 12, 54, 13, 19, 32, 16, 8, 15, 17, 23, 35, 7 };
   using myType = decltype(values)::value_type;
   //const int max_value = 15;
   auto constexpr is_lesser_than = [max_value = 15](auto val) -> bool {  
      return val < max_value;
      };

   //TMyLesser checker(15);
   auto it = std::partition(values.begin(), values.end(), is_lesser_than);
   Sortiere(values);

}
