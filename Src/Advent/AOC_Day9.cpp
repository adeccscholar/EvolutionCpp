#include <iomanip>
#include <vector>
#include <set>
#include <array>
#include <utility>
#include <algorithm>

#include "AOC_Day9.h"
#include "AOC_Read.h"

using namespace std::literals::string_literals;

namespace Day9 {

   enum class EDirection : char { left = 'L', right = 'R', up = 'U', down = 'D'};

   using move_ty  = std::pair<EDirection, int>;
   using input_ty = std::vector<move_ty>;

   std::istream& operator >> (std::istream& ins, move_ty& data) {
      char dir;
      int  step;
      ins >> dir >> std::skipws >> step;
      data = std::make_pair ( static_cast<EDirection>(dir), step );
      return ins;
      }

   class TCoordinate {
      friend void Step(TCoordinate& head, TCoordinate& tail, EDirection direction);
      friend bool Following(TCoordinate& head, TCoordinate& tail);

      friend std::ostream& operator << (std::ostream& out, TCoordinate const& data) {
               out << "(" << std::setw(3) << data.x << ", " << std::setw(3) << data.y << ")";
               return out;
               }

      friend TCoordinate operator + (TCoordinate const& a, TCoordinate const& b) {
         return { a.x + b.x, a.y + b.y };
         }


   private:
      int x = 0;
      int y = 0;
   public:
      TCoordinate(void) = default;
      TCoordinate(TCoordinate const& ref) : x(ref.x), y(ref.y) { }
      TCoordinate(int new_x, int new_y) : x(new_x), y(new_y) { }
      ~TCoordinate(void) = default;

      TCoordinate& operator = (TCoordinate const& ref) {
         x = ref.x;
         y = ref.y;
         return *this;
         }

      TCoordinate& operator += (TCoordinate const& ref) {
         x += ref.x;
         y += ref.y;
         return *this;
         }

      constexpr auto operator <=> (TCoordinate const& ref) const { return std::tie(x, y) <=> std::tie(ref.x, ref.y); }
      // auto operator<=>(const TCoordinate&) const = default;

      void Set(int new_x, int new_y) {
         x = new_x;
         y = new_y;
         }


      constexpr bool Touching(TCoordinate const& ref) const {
         constexpr auto my_abs = [](int a, int b) { return a > b ? a - b : b - a; };
         return my_abs(x, ref.x) < 2 && my_abs(y, ref.y) < 2;
         }

      constexpr int Distance(TCoordinate const& ref) const {
         constexpr auto my_abs = [](int a, int b) { return a > b ? a - b : b - a; };
         return my_abs(x, ref.x) + my_abs(y, ref.y);
         }

      constexpr bool Diagonal(TCoordinate const& ref) const {
         return x != ref.x && y != ref.y ? true : false;
         }

      };

   using Visited = std::set<TCoordinate>;
   Visited result;


   bool Following(TCoordinate& head, TCoordinate& tail) {
      if (!head.Touching(tail)) {
         int i_delta_x = head.x - tail.x;
         int i_delta_y = head.y - tail.y;
         switch (auto dist = head.Distance(tail); dist) {
            case 0:
               break;
            case 2:
               if (head.Diagonal(tail)) throw std::runtime_error("unexpected position");
               tail += { i_delta_x / 2, i_delta_y / 2 };
               break;
            case 3:
               if (!head.Diagonal(tail)) throw std::runtime_error("unexpected position");
               if (std::abs(i_delta_x) == 2) tail += { i_delta_x / 2, i_delta_y };
               else tail += { i_delta_x, i_delta_y / 2 };
               break;
            case 4:
               if (!head.Diagonal(tail)) throw std::runtime_error("unexpected position");
               tail += { i_delta_x / 2, i_delta_y / 2 };
               break;
            default: throw std::runtime_error("unexpected distance");
            }
         return true;
      }
      else return false;
      }

   void Step(TCoordinate& head, TCoordinate& tail, EDirection direction) {
      switch (direction) {
         case EDirection::left:  head += { -1, 0 }; break;
         case EDirection::right: head += {  1, 0 }; break;
         case EDirection::up:    head += {  0, 1 }; break;
         case EDirection::down:  head += {  0, -1 }; break;
         default: throw std::runtime_error("unexpected direction in move");
         }
      }

   void Move(TCoordinate& head, TCoordinate& tail, EDirection direction, size_t steps) {
      //std::cout << static_cast<char>(direction) << steps << "\n";
      for(;steps > 0; --steps) {
         Step(head, tail, direction);
         Following(head, tail);
         result.insert(tail);
         //std::cout << head << " -- " << tail << "\n";
         }
      }


   void ReadValues_1(std::string const& strFilename) {
      TCoordinate head, tail;
      move_ty data;
      result.clear();
      result.insert(tail);
      auto ifs = input(strFilename);
      if (ifs.eof()) throw std::runtime_error("input file empty");
      do {
         ifs >> data;
         Move(head, tail, data.first, data.second);
         }
      while (!ifs.eof());
      std::cout << "Day 9/1: " << result.size() << "\n";
   }


   std::array<TCoordinate, 10> rope;

   void Move(std::array<TCoordinate, 10>& rope, EDirection direction, size_t steps) {
      //std::cout << static_cast<char>(direction) << steps << "\n";
      for (; steps > 0; --steps) {
         bool boChanged = true;
         Step(rope[0], rope[1], direction);
         for(size_t part = 1u; part < rope.size(); ++part) {
            if (boChanged = Following(rope[part - 1], rope[part]); !boChanged) {
               break;
               }
            }

         //std::for_each(rope.begin(), rope.end(), [](auto val) { std::cout << val; });
         //std::cout << "\n";
         if(boChanged) result.insert(rope[rope.size() - 1]);
         }
      }



   void ReadValues_2(std::string const& strFilename) {
      std::for_each(rope.begin(), rope.end(), [](auto& val) mutable { val = { 0, 0 };  });
      auto ifs = input(strFilename);
      if (ifs.eof()) throw std::runtime_error("input file empty");
      move_ty data;
      result.clear();
      result.insert( rope[rope.size() - 1] );
      do {
         ifs >> data;
         Move(rope, data.first, data.second);
         }
      while (!ifs.eof());
      std::cout << "Day 9/2: " << result.size() << "\n";
      }

}