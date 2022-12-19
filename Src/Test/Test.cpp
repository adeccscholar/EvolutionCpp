

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

static constexpr auto input = [](std::string const& strFile) {
   std::ifstream ifs;
   ifs.open(strFile);
   if (!ifs.is_open()) {
      std::ostringstream os;
      os << "File " << strFile << " couldn't opened.";
      throw std::runtime_error(os.str());
   }
   return ifs;
};


using tree_row  = std::vector<size_t>;
using tree_grid = std::vector<tree_row>;

void Day8() {

   try {
      std::cout << "AOC 2022 Day 8!\n";
      //auto ifs = input("D:\\test\\aoc2022\\test_values8.txt");
      auto ifs = input("D:\\test\\aoc2022\\values_day8.txt");
      tree_grid grid;
      std::string strRow;
      while (std::getline(ifs, strRow)) {
         tree_row current;
         for (auto c : strRow) {
            if (c < '0' || c > '9') throw std::runtime_error("unexpected input");
            current.emplace_back(c - '0');
         }
         grid.emplace_back(current);
      }



      size_t x_max = grid.size();
      size_t y_max = grid[0].size();
      size_t visible = 2 * x_max + 2 * (y_max - 2);

      std::for_each(grid.begin(), grid.end(), [y_max](auto val) { if (val.size() != y_max) throw std::runtime_error("unexpected size in tree row");  });

      static constexpr auto new_max = [](size_t& max, size_t& current) { if (current > max) max = current;  };



      for (size_t x = 1u; x < x_max - 1u; ++x) {
         for (size_t y = 1u; y < y_max - 1u; ++y) {
            {
               size_t m_x1 = grid[0][y];
               for (auto x1 = 0u; x1 < x; ++x1) new_max(m_x1, grid[x1][y]);
               if (m_x1 < grid[x][y]) {
                  ++visible;
                  continue;
               }
            }
            size_t m_x2 = grid[x + 1][y];
            for (auto x2 = x + 1u; x2 < x_max; ++x2) new_max(m_x2, grid[x2][y]);
            if (m_x2 < grid[x][y]) {
               ++visible;
               continue;
            }

            size_t m_y1 = grid[x][0];
            for (auto y1 = 0u; y1 < y; ++y1) new_max(m_y1, grid[x][y1]);
            if (m_y1 < grid[x][y]) {
               ++visible;
               continue;
            }

            size_t m_y2 = grid[x][y + 1];
            for (auto y2 = y + 1u; y2 < y_max; ++y2) new_max(m_y2, grid[x][y2]);
            if (m_y2 < grid[x][y]) {
               ++visible;
               continue;
            }
         }
      }
      std::cout << "Day 8 / 1: " << visible << "\n";

      // -------------------------------------------------------------------------------
      size_t metric_max = 0u;
      for (size_t x = 1u; x < x_max - 1u; ++x) {
         for (size_t y = 1u; y < y_max - 1u; ++y) {
            size_t x1 = x - 1u, x2 = x + 1u, y1 = y - 1u, y2 = y + 1u, metric = 1u;
            for (;;) {
               if (grid[x1][y] >= grid[x][y]) {
                  break;
               }
               if (x1 == 0) break;
               --x1;
            }
            metric *= (x - x1);

            for (;;) {
               if (grid[x2][y] >= grid[x][y]) {
                  break;
               }
               if (x2 == x_max - 1) break;
               ++x2;
            }
            metric *= (x2 - x);

            for (;;) {
               if (grid[x][y1] >= grid[x][y]) {
                  break;
               }
               if (y1 == 0) break;
               --y1;
            }
            metric *= (y - y1);

            for (;;) {
               if (grid[x][y2] >= grid[x][y]) {
                  break;
               }
               if (y2 == y_max - 1) break;
               ++y2;
            }
            metric *= (y2 - y);

            new_max(metric_max, metric);
         }
      }
      std::cout << "Day 8 / 2: " << metric_max << "\n";
   }
   catch (std::exception& ex) {
      std::cerr << "Error: " << ex.what() << "\n";
   }

}

enum class EDirection : char {  left = 'L', right = 'R', up = 'U', down = 'D' };

using move_ty  = std::pair<EDirection, int>;
using input_ty = std::vector<move_ty>;

std::istream& operator >> (std::istream& ins, move_ty& data) {
   char dir;
   int step;
   ins >> dir >> std::skipws >> step;
   data = std::make_pair(static_cast<EDirection>(dir), step);
   return ins;
   }

class TCoordinate {
   friend std::ostream& operator << (std::ostream& out, TCoordinate const& data) {
             out << "(" << data.x << ", " << data.y << ")";
             return out;
             }
   friend void Move(TCoordinate& head, TCoordinate& tail, EDirection direction, size_t steps);
private:
   int x = 0;
   int y = 0;
public:
   TCoordinate() = default;
   TCoordinate(TCoordinate const& ref) : x(ref.x), y(ref.y) { }
   ~TCoordinate() = default;

   auto operator <=> (TCoordinate const& ref) const {
      return std::tie(x, y) <=> std::tie(ref.x, ref.y); 
      }

   void Set(int new_x, int new_y) {
      x = new_x;
      y = new_y;
      }

   int Distance(TCoordinate const& ref) const {
      return std::max( { x < ref.x ? ref.x - x  : x - ref.x, y < ref.y ? ref.y - y : y - ref.y } );
      }

};

using Visited = std::set<TCoordinate>;
Visited result;

void Move(TCoordinate& head, TCoordinate& tail, EDirection direction, size_t steps) {
   std::cerr << static_cast<char>(direction) << " " << steps << "\n";
   for(;steps > 0; --steps) {
      switch(direction) {
         case EDirection::left:  --head.x; break;
         case EDirection::right: ++head.x; break;
         case EDirection::up: ++head.y; break;
         case EDirection::down: --head.y; break;
         default: throw std::runtime_error("unexpected direction in move");
         }
      if(head.Distance(tail) > 1) {
         switch (direction) {
            case EDirection::left:  tail.Set(head.x + 1, head.y); break;
            case EDirection::right: tail.Set(head.x - 1, head.y); break;
            case EDirection::up: tail.Set(head.x, head.y - 1); break;
            case EDirection::down: tail.Set(head.x, head.y + 1); break;
            default: throw std::runtime_error("unexpected direction in move");
            }
         }
      result.insert(tail);
      // std::cerr << head << " " << tail << "\n";
      }
   }



int main() {
   // Day8();

   try {
      //auto ifs = input("D:\\test\\aoc2022\\test_values9.txt");
      auto ifs = input("D:\\test\\aoc2022\\values_day9.txt");

      TCoordinate head, tail;
      move_ty data;
      result.clear();
      result.insert(tail);
      if (ifs.eof()) throw std::runtime_error("input file is empty");
      do {
         ifs >> data;
         Move(head, tail, data.first, data.second);
         } 
      while (!ifs.eof());

      std::cout << "Day 9 / 1: " << result.size() << "\n";
      }
   catch(std::exception& ex) {
      std::cerr << "error: " << ex.what() << "\n";
      }
   }

