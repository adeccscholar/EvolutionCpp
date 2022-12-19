#include <vector>
#include <utility>
#include <algorithm>

#include "AOC_Day8.h"
#include "AOC_Read.h"

using namespace std::literals::string_literals;

namespace Day8 {

   using tree_row = std::vector<size_t>;
   using tree_grid = std::vector<tree_row>;


   void ReadValues_1(std::string const& strFilename) {
      auto ifs = input(strFilename);
      tree_grid grid;
      std::string strRow;
      while (std::getline(ifs, strRow)) {
         tree_row current;
         for(auto c : strRow) {
            if (c < '0' || c > '9') throw std::runtime_error("unexpected input");
            current.emplace_back(c - '0');
            }
         grid.emplace_back(current);
         }

      size_t x_max = grid.size();
      size_t y_max = grid[0].size();
      size_t visible = 2 * x_max + 2 * (y_max - 2);

      static constexpr auto new_max = [](size_t& max, size_t& current) { if (current > max) max = current;  };

      for(size_t x = 1; x < x_max - 1; ++x) {
         for (size_t y = 1; y < y_max - 1; ++y) {
            //auto max_vals = std::make_pair (std::max_element())
            size_t m_x1 = grid[0][y];
            for (auto x1 = 0u; x1 < x; ++x1) new_max(m_x1, grid[x1][y]);
            if (m_x1 < grid[x][y]) {
               ++visible;
               continue;
               }

            size_t m_x2 = grid[x+1][y];
            for (auto x2 = x + 1; x2 < x_max; ++x2) new_max(m_x2, grid[x2][y]);
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

            size_t m_y2 = grid[x][y+1];
            for (auto y2 = y+1; y2 < y_max; ++y2) new_max(m_y2, grid[x][y2]);
            if (m_y2 < grid[x][y]) {
               ++visible;
               continue;
               }
            }
         }
      std::cout << "Day 8/1: " << visible << std::endl;
   }

   void ReadValues_2(std::string const& strFilename) {
      auto ifs = input(strFilename);
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
      size_t step = 0u;

      static constexpr auto new_max = [](size_t& max, size_t& current) { if (current > max) max = current;  };
      size_t metric_max = 0u;
      for (size_t x = 1; x < x_max - 1; ++x) {
         for (size_t y = 1; y < y_max - 1; ++y) {
            ++step;
            //std::cout << step << ": " << x << ", " << y << "\n";

            size_t x1 = x - 1, x2 = x + 1, y1 = y - 1, y2 = y + 1, metric = 1u;
            for(;;) {
               if (grid[x1][y] >= grid[x][y]) {
                  break;
                  }
               if (x1 == 0) break;
               --x1;
               }
            metric *= (x - x1);

            for(;;) {
               if (grid[x2][y] >= grid[x][y]) {
                  break;
                  }
               if (x2 == x_max - 1) break;
               ++x2;
               }
            metric *= (x2 - x);

            for(;;) {
               if (grid[x][y1] >= grid[x][y]) {
                  break;
                  }
               if (y1 == 0) break;
               --y1;
               }
            metric *= (y - y1);

            for(;;) {
               if (grid[x][y2] >= grid[x][y]) {
                  break;
                  }
               if (y2 == y_max - 1) break;
               ++y2;
               }
            metric *= (y2 - y);
            //if (metric > metric_max) metric_max = metric;
            new_max(metric_max, metric);
            //std::cout << x << ", " << y << " (" << m_x1 << ", " << m_x2 << ", " << m_y1 << ", " << m_y2 << ")\n";
         }
      }
      std::cout << "Day 8/2: " << metric_max << std::endl;
      /*
      std::for_each(grid.begin(), grid.end(), [](auto vals) {
                                                 for (auto t : vals) std::cout << t << ' ';
                                                 std::cout << "\n";
                                                 });
      */
   }


}