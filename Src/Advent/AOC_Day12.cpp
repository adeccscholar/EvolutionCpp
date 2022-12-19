#include <iomanip>
#include <vector>
#include <set>
#include <tuple>
#include <map>
#include <utility>
#include <optional>
#include <syncstream>
#include <thread>
#include <future>
#include <ranges>
#include <random>
#include <chrono>

#include "AOC_Day12.h"
#include "AOC_Read.h"

using namespace std::literals::string_literals;

namespace Day12 {


   std::mutex mtx_cout;

   class TRoute {
      friend void swap(TRoute& lhs, TRoute& rhs) noexcept {
         std::swap(lhs.the_current, rhs.the_current);
         std::swap(lhs.the_pathway, rhs.the_pathway);
         }
      public:
         enum class EDirections : char { left, right, up, down };

         // chessboard topology
         using topography_value = char;
         using topography_row = std::vector<topography_value>;
         using topography_grid = std::vector<topography_row>;
         using topography_pos = std::pair<size_t, size_t>;
         using topography_path = std::vector<topography_pos>;

         // shortest distance to the coordinate
         using path_row = std::vector<std::optional<size_t>>;
         using path_grid = std::vector<path_row>;

      private:
         topography_pos  the_current;
         topography_path the_pathway;

         static topography_pos the_start_pos, the_end_pos;
         static topography_grid the_grid;
      public:
         TRoute() {
            the_current = { 0u, 0u };
            }

         
         TRoute(TRoute const& ref) {
            the_current = ref.the_current;
            the_pathway = ref.the_pathway;
         }
         

         TRoute(TRoute&& ref) noexcept {
            std::swap(the_current, ref.the_current);
            std::swap(the_pathway, ref.the_pathway);
            }

         TRoute(topography_path const& until, topography_pos start)  {
            the_current = start;
            the_pathway = until;
            the_pathway.emplace_back(start);
            }

         ~TRoute() = default;


         TRoute& operator = (TRoute const& ref) {
            the_current = ref.the_current;
            the_pathway = ref.the_pathway;
            return *this;
            }

         static size_t max_x() { return the_grid[0].size(); }  // possible problem if grid.size() < 1
         static size_t max_y() { return the_grid.size(); }

         topography_pos& current() { return the_current; }
         topography_path& pathway() { return the_pathway; }
         topography_pos const& current() const { return the_current; }
         topography_path const& pathway() const { return the_pathway; }


         static topography_pos& start_pos() { return the_start_pos; }
         static topography_pos & end_pos() {  return the_end_pos; }
         static topography_grid& grid() {     return the_grid; }

         static void reset_grid() {
            the_start_pos = { 0u, 0u };
            the_end_pos = { 0u, 0u };
            for(auto& row : the_grid) row.clear();
            the_grid.clear();
         }

         auto& grid_element(topography_pos const& pos) {
            return the_grid[pos.second][pos.first];
            }

         auto const& grid_element(topography_pos const& pos) const {
            return the_grid[pos.second][pos.first];
         }

         void step(topography_pos const& pos) {
            the_pathway.emplace_back(pos);
            the_current = pos;
            }

         void start(topography_pos const& starting_point) {
            the_start_pos = starting_point;
            the_pathway.clear();
            step(the_start_pos);
            }

         std::pair<topography_pos, bool> go_to(EDirections direction) const {
            topography_pos max = { max_x(), max_y() };
            topography_pos pos = the_current;
            bool retval = false;
            switch(direction) {
               case EDirections::right:
                  if (the_current.first < max_x() - 1) {
                     ++pos.first;
                     retval = true;
                     }
                  break;
               case EDirections::down:
                  if (the_current.second < max_y() - 1) {
                     ++pos.second;
                     retval = true;
                     }
                  break;
               case EDirections::left:
                  if(the_current.first > 0) {
                     --pos.first;
                     retval = true;
                     } 
                  break;
               case EDirections::up:
                  if (the_current.second > 0) {
                     --pos.second;
                     retval = true;
                  }
                  break;
               default: throw std::runtime_error("unexpected direction for the move, critical.");
               }
            return { pos, retval };
            }

         std::set<topography_pos> possible_steps() const {
            std::set<topography_pos> steps;
            auto current_height = grid_element(the_current);
            for(auto dir : { EDirections::right, EDirections::down, EDirections::left, EDirections::up }) {
               if(auto [next_pos, has_moved] = go_to(dir); has_moved) {
                  auto next_height = grid_element(next_pos);
                  if(next_height - 1 <= current_height) {
                     if (std::find(the_pathway.begin(), the_pathway.end(), next_pos) == the_pathway.end()) 
                        steps.insert(next_pos);
                     }
                  }
               }
            auto cc = steps.size();
            return steps;
            }

      };

      // static data member of class TRoute
      TRoute::topography_pos TRoute::the_start_pos = { 0u, 0u };
      TRoute::topography_pos TRoute::the_end_pos = { 0u, 0u };
      TRoute::topography_grid TRoute::the_grid;


      TRoute::topography_pos operator + (TRoute::topography_pos const& lhs, TRoute::topography_pos const& rhs) {
         TRoute::topography_pos sum = lhs;
         sum.first += rhs.first;
         sum.second += rhs.second;
         return sum;
      }

      std::ostream& operator << (std::ostream& out, TRoute::topography_pos const& pos) {
         out << "( " << std::setw(2) << pos.first << ", " << std::setw(2) << pos.second << ")";
         return out;
      }

   std::pair<TRoute, bool> MakeSteps(TRoute&& route, TRoute::path_grid& distances) {
      if (route.current() == route.end_pos()) {
         return { std::move(route), true };
         }

      auto& pos =  distances[route.current().second][route.current().first];
      if (!pos) pos = route.pathway().size();
      else {
         if (*pos > route.pathway().size()) pos = route.pathway().size();
         else return { std::move(route), false };
         }

      auto next = route.possible_steps();
      if (next.size() == 0) {
          return { std::move(route), false };
          }

      /*
      std::vector<std::future<std::pair<TRoute, bool>>> results;
      auto my_rref = [](auto type) mutable {  return std::move(type); };
      
      for(auto const& val : next) {
         results.emplace_back( std::async(MakeSteps, my_rref(TRoute { route.pathway(), val } ), std::ref(distances)));
         }

      std::optional<size_t> counter = std::nullopt;
      TRoute retval;
      for(auto& result : results) {
         auto [new_route, success] = result.get();

         if (success) {
            if (!counter) {
               counter = new_route.pathway().size();
               swap(retval, new_route);
               }
            else if (new_route.pathway().size() < *counter) {
               counter = new_route.pathway().size();
               swap(retval, new_route);
               }
            }
         }
       */
      
      std::optional<size_t> counter = std::nullopt;
      TRoute retval;
      for(auto& val : next) {
         auto [alternative, success] = MakeSteps({ route.pathway(), val }, distances);
         if(success) {
            if(!counter) {
               counter = alternative.pathway().size();
               retval = alternative;
               }
            else if(alternative.pathway().size() < *counter) {
               counter = alternative.pathway().size();
               retval = alternative;
               }
            } 
         }
       
      if (counter) return { std::move(retval), true };
      else return { std::move(retval), false };
      }

// matrix(m,n)
// height in (a, z)
// start position S with height a
// end position E with height z
// directions up, down, left, right
// elevation to next step max 1 (or equal, or smaller)


   std::pair<TRoute, TRoute::topography_pos> ReadValues(std::string const& strFilename) {
       TRoute::reset_grid();
       TRoute route;
       TRoute::topography_pos starting_here;
       auto ifs = input(strFilename);
       size_t x_pos = 0u, y_pos = 0u;
       std::string strRow;
       while (std::getline(ifs, strRow)) {
          TRoute::topography_row current;
          x_pos = 0u;
          for (auto c : strRow) {
             switch (c) {
                case 'S': 
                   starting_here = { x_pos, y_pos };
                   current.emplace_back('a');
                   break;
                case 'E':
                   route.end_pos() = { x_pos, y_pos };
                   current.emplace_back('z');
                   break;
                default:
                   if(c < 'a' || c > 'z') throw std::runtime_error("unexpected input for file \""s + strFilename +"\" in row "s + 
                                                                    std::to_string(y_pos) + " and col " + std::to_string(x_pos) + "."s);
                   current.emplace_back(c);
                }
             ++x_pos;
             }
         route.grid().emplace_back(current);
         ++y_pos;
         }


      size_t x_max = route.grid()[0].size();
      for (auto const& row : route.grid()) if (row.size() != x_max) throw std::runtime_error("unexpected len in grid"s);
      return { route, starting_here };
      }

   void Output(std::ostream& out, TRoute const& route) {
      out << "Start: " << route.start_pos() << "\n"
          << "End:   " << route.end_pos() << "\n"
          << "0 - " << route.max_x() << " / 0 - " << route.max_y() << "\n";

      out << "Start: " << route.start_pos() << "\n"
          << "End:   " << route.end_pos() << "\n"
          << "0 - " << route.max_x() << " / 0 - " << route.max_y() << "\n\n";
      for (auto const& row : route.grid()) {
         for (auto const& c : row) out << c;
         out << "\n";
         }
      }

   void OuptputPath(std::ostream& out, TRoute const& route) {
      for (auto const& pos : route.pathway()) out << pos << " ";
      out << "\n";
      }

   void ReadValues_1(std::string const& strFilename) {
      
      auto [route, starting_here] = ReadValues(strFilename);
      route.start(starting_here);

      TRoute::path_grid grid_distance(route.max_y(), TRoute::path_row(route.max_x(), std::nullopt));
      auto [new_route, success] = MakeSteps(std::forward<TRoute>(route), grid_distance);
      if(success) {
         std::cout << "Day 12/1: " << new_route.pathway().size() - 1 << "\n";
         }
      else {
         std::cout << "ERROR " << new_route.pathway().size() << "\n";
         }

      }


   std::vector<TRoute::topography_pos> ReadAsStart(TRoute const& route, TRoute::topography_value value) {
      std::vector<TRoute::topography_pos> positions;
      auto test = TRoute::topography_pos{ route.max_x(), route.max_y() };
      for (int x = 0u; x < route.max_x(); ++x)
         for (int y = 0u; y < route.max_y(); ++y)
            if (route.grid()[y][x] == value) positions.emplace_back(TRoute::topography_pos { x, y });
      
      std::random_device rd;
      std::mt19937 gen{ rd() };
      std::ranges::shuffle(positions, gen);
      return positions;
      }


   std::optional<size_t> calculate(TRoute const&route, std::vector<TRoute::topography_pos> const& positions, size_t iTask, size_t start, size_t end) {
      static std::atomic<size_t> cnt = 0u;
      std::optional<size_t> counter = std::nullopt;
      TRoute retval;
      for (; start < end; ++start) {
         TRoute localroute(route);
         TRoute::path_grid distances = TRoute::path_grid{ localroute.max_y(), TRoute::path_row(localroute.max_x(), std::nullopt) };
         localroute.start(positions[start]);
         auto [new_route, success] = MakeSteps(std::forward<TRoute>(localroute), distances);

         if (success) {
            {
            //std::osyncstream syncStream(std::cout);
            //syncStream << std::setw(5) << ++cnt << " / " << std::setw(2) << iTask << " = " 
            //           << std::setw(4) << start << " " << new_route.start_pos() << " -> " << new_route.pathway().size() - 1 << "\n";
            }
            if (!counter) {
               counter = new_route.pathway().size();
               swap(retval, new_route);
            }
            else if (new_route.pathway().size() < *counter) {
               counter = new_route.pathway().size();
               swap(retval, new_route);
            }
         }
      }
      return counter;
   };


   void ReadValues_2(std::string const& strFilename) {
      auto [route, start] = ReadValues(strFilename);
      auto start_points = ReadAsStart(route, 'a');


      //std::cout << "Anzahl: " << start_points.size() << std::endl;
      

      auto func_start = std::chrono::high_resolution_clock::now();
 
      ///*  parallel
      std::vector<std::future<std::optional<size_t>>> tasks;
      auto needed = start_points.size();
      const size_t cpus = 1;
      size_t max = needed < cpus ? needed : cpus;
      for(size_t iTask = 0; iTask < max; ++iTask) {
         size_t start = iTask * needed / max;
         size_t end = iTask <  max - 1 ? (iTask + 1) * needed / max : needed;
         //std::cout << iTask << ": " << start << ", " << end << "\n";
         tasks.emplace_back(async(calculate, std::cref(route), std::cref(start_points), iTask, start, end));
         }

         std::optional<size_t> counter = std::nullopt;
      TRoute retval;

      for (auto& result : tasks) {
         auto new_val = result.get();

         if (new_val) {
            if (!counter) {
               counter = *new_val;
               }
            else if (*new_val < *counter) {
               counter = *new_val;
               }
            }
         }

      //*/

      // linear
      //size_t starting = 0u, end = start_points.size();
      //auto counter = calculate(route, start_points, 0u, starting, end);

      auto func_ende = std::chrono::high_resolution_clock::now();
      auto time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);

      if (counter) {
         std::cout << "Day 12/2: " << *counter - 1 << " in " << time.count() / 1000. << " sec" "\n";
         }
      else {
         std::cout << "ERROR " << "\n";
         }
      }
}
