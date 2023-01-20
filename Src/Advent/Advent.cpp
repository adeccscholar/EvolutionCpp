// Advent.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//


#include <iostream>
#include <iomanip>
#include <exception>
#include <string>
#include <sstream>

using namespace std::string_literals;

#include "AOC_Day1.h"
#include "AOC_Day2.h"
#include "AOC_Day3.h"
#include "AOC_Day4.h"
#include "AOC_Day5.h"
#include "AOC_Day6.h"
#include "AOC_Day7.h"
#include "AOC_Day8.h"
#include "AOC_Day9.h"
#include "AOC_Day10.h"
#include "AOC_Day11.h"
#include "AOC_Day12.h"
#include "AOC_Day13.h"
#include "AOC_Day14.h"
#include "AOC_Day15.h"
#include "AOC_Day16.h"
#include "AOC_Day17.h"
#include "AOC_Day18.h"
#include "AOC_Day19.h"
#include "AOC_Day20.h"
#include "AOC_Day21.h"
#include "AOC_Day22.h"
#include "AOC_Day23.h"
#include "AOC_Day24.h"
#include "AOC_Day25.h"

const std::string strPath = "D:\\Test\\AoC2022\\"s;

std::string FileNameDay(int iDay, bool boTest = false) {
   std::ostringstream os;
   os << strPath << (boTest ? "Test\\"s : ""s) << "values_day"s << std::setw(2) << std::setfill('0') << iDay << ".txt"s;
   return os.str();
   }

int main() {
   try {
   /*
    std::cout << "Hello Advent time! Time to solve this funny puzzle\n";
    std::cout << "\n--- Day 1: Calorie Counting ---\n";
    Day1::ReadValues_1(FileNameDay(1));
    Day1::ReadValues_2(FileNameDay(1));

    std::cout << "\n--- Day 2: Rock Paper Scissors ---\n";
    Day2::ReadValues_1(FileNameDay(2));
    Day2::ReadValues_2(FileNameDay(2));

    std::cout << "\n--- Day 3: Rucksack Reorganization ---\n";
    Day3::ReadValues_1(FileNameDay(3));
    Day3::ReadValues_2(FileNameDay(3));

    std::cout << "\n--- Day 4: Camp Cleanup ---\n";
    Day4::ReadValues_1(FileNameDay(4)); 
    Day4::ReadValues_2(FileNameDay(4));

    std::cout << "\n--- Day 5: Supply Stacks ---\n";
    Day5::ReadValues_1(FileNameDay(5));
    Day5::ReadValues_2(FileNameDay(5));

    std::cout << "\n--- Day 6: Tuning Trouble ---\n";
    Day6::ReadValues_1(FileNameDay(6));
    Day6::ReadValues_2(FileNameDay(6));

    std::cout << "\n--- Day 7: No Space Left On Device ---\n";
    Day7::ReadValues_1(FileNameDay(7));
    Day7::ReadValues_2(FileNameDay(7));

    std::cout << "\n--- Day 8: Treetop Tree House ---\n";
    Day8::ReadValues_1(FileNameDay(8));
    Day8::ReadValues_2(FileNameDay(8));

    std::cout << "\n--- Day 9: Rope Bridge ---\n";
    Day9::ReadValues_1(FileNameDay(9));
    Day9::ReadValues_2(FileNameDay(9));

    std::cout << "\n--- Day 10: Cathode-Ray Tube ---\n";
    Day10::ReadValues_1(FileNameDay(10));
    Day10::ReadValues_2(FileNameDay(10));

    std::cout << "\n--- Day 11: Monkey in the Middle ---\n";
    Day11::ReadValues_1(FileNameDay(11));
    Day11::ReadValues_2(FileNameDay(11));
    
    std::cout << "\n--- Day 12: Hill Climbing Algorithm ---\n";
    //Day12::ReadValues_1(FileNameDay(12, true));
    //Day12::ReadValues_2(FileNameDay(12, true));
    //Day12::ReadValues_1(FileNameDay(12));
    //Day12::ReadValues_2(FileNameDay(12));
    
    
    //std::cout << "\n--- Day 13: Distress Signal ---\n";
    //Day13::ReadValues_1(FileNameDay(13));
    //Day13::ReadValues_2(FileNameDay(13));
    */
    std::cout << "\n--- Day 14: Regolith Reservoir ---\n";
    Day14::ReadValues_1(FileNameDay(14));
    Day14::ReadValues_2(FileNameDay(14));
 
    /*
    std::cout << "\n--- Day 15: Beacon Exclusion Zone ---\n";

    std::cout << "\n--- Day 16: Proboscidea Volcanium ---\n";
    Day16::ReadValues_1(FileNameDay(16));
    Day16::ReadValues_2(FileNameDay(16));
    
    std::cout << "\n--- Day 17: Pyroclastic Flow ---\n";
    Day17::ReadValues_1(FileNameDay(17));
    Day17::ReadValues_2(FileNameDay(17));

    std::cout << "\n--- Day 18: Boiling Boulders ---\n";
    Day18::ReadValues_1(FileNameDay(18));
    Day18::ReadValues_2(FileNameDay(18));

    std::cout << "\n--- Day 19: Not Enough Minerals ---\n";
    Day19::ReadValues_1(FileNameDay(19));
    Day19::ReadValues_2(FileNameDay(19));

    std::cout << "\n--- Day 20: Grove Positioning System ---\n";
    Day20::ReadValues_1(FileNameDay(20));
    Day20::ReadValues_2(FileNameDay(20));

    std::cout << "\n--- Day 21: Monkey Math ---\n";
    Day21::ReadValues_1(FileNameDay(21));
    Day21::ReadValues_2(FileNameDay(21));

    std::cout << "\n--- Day 22: Monkey Map ---\n";
    Day22::ReadValues_1(FileNameDay(22));
    Day22::ReadValues_2(FileNameDay(22));

    std::cout << "\n--- Day 23: Unstable Diffusion ---\n";
    Day23::ReadValues_1(FileNameDay(23));
    Day23::ReadValues_2(FileNameDay(23));

    std::cout << "\n--- Day 24:  ---\n";
    Day24::ReadValues_1(FileNameDay(24));
    Day24::ReadValues_2(FileNameDay(24));

    std::cout << "\n--- Day 25:  ---\n";
    Day25::ReadValues_1(FileNameDay(25));
    Day25::ReadValues_2(FileNameDay(25));
    */
    //getchar();
   }
   catch(std::exception& ex) {
      std::cerr << "error: " << ex.what() << std::endl;
      }
}

