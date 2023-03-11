/**
 \file
 \brief Hauptdatei für das Leseprojekt
 \details Diese Datei enthält die Funktion "main" des Testprogramms. Hier beginnt und endet die Ausführung des Programms.
 \details Zusätzlich sind noch alle Routinen, Datentypen und Hilfskonstrukte hier
 \author  Volker Hillmann
 \date    09.12.2022
 \copyright Copyright &copy; 2022-2023. adecc Systemhaus GmbH
 \version 1.0
*/

#include "MyData.h"
#include "Tool_Helper.h"
#include "MyFileIterator.h"

#include <iostream>
#include <utility>
#include <string>
#include <string_view> // !! C++17
#include <vector>
#include <array>
#include <fstream>
#include <functional>
#include <execution> // !! C++17
#include <iterator>
#include <optional> // !! C++17
#include <atomic>
#include <filesystem>

using namespace std::literals;
namespace fs = std::filesystem;

const func_vector funcs = {
   [](TData<double>& data, std::string const& val) { data.City(val); } ,
   //nullptr,
   [](TData<double>& data, std::string const& val) { data.Street(val); } ,
   [](TData<double>& data, std::string const& val) { data.StreetNumber(val); },
   [](TData<double>& data, std::string const& val) { data.ZipCode(val); },
   [](TData<double>& data, std::string const& val) { data.UrbanUnit(val); },
   [](TData<double>& data, std::string const& val) { data.UrbanUnit_Old(val); },
   [](TData<double>& data, std::string const& val) { data.District(val); },
   [](TData<double>& data, std::string const& val) { data.Latitude(val); },
   [](TData<double>& data, std::string const& val) { data.Longitude(val); },
   [](TData<double>&, std::string const&) { throw std::runtime_error("unexpected number of elements.");  } 
};

const func_vector_vw funcs_vw = {
   [](TData<double>& data, std::string_view const& val) { data.City(val); } ,
   [](TData<double>& data, std::string_view const& val) { data.Street(val); } ,
   [](TData<double>& data, std::string_view const& val) { data.StreetNumber(val); },
   [](TData<double>& data, std::string_view const& val) { data.ZipCode(val); },
   [](TData<double>& data, std::string_view const& val) { data.UrbanUnit(val); },
   [](TData<double>& data, std::string_view const& val) { data.UrbanUnit_Old(val); },
   [](TData<double>& data, std::string_view const& val) { data.District(val); },
   [](TData<double>& data, std::string_view const& val) { data.Latitude(val); },
   [](TData<double>& data, std::string_view const& val) { data.Longitude(val); },
   [](TData<double>&, std::string_view const&) { throw std::runtime_error("unexpected number of elements.");  }
};


template <typename ty>
inline size_t Read_0(data_vector<ty>& vData, func_vector const& funcs, std::istream& ifs) {
   size_t iLineCnt = 0u;
   std::string strRow;
   while(std::getline(ifs, strRow)) {
      TData<double> data;
      auto input = tokenize(strRow, ";", 9);
      for (size_t iCnt = 0u; auto const& element : input) { funcs[iCnt](data, input[iCnt]); ++iCnt; }
      vData.emplace_back(std::forward<TData<double>>(data));
      ++iLineCnt;
      }
   return iLineCnt;
   }

template <typename ty>
inline size_t Read_1(data_vector<ty>& vData, func_vector const& funs, std::string const& buffer) {
   size_t pos = 0u;
   for(auto end = buffer.find('\n'); end != std::string::npos; pos = end + 1u, end = buffer.find('\n', pos )) {
      size_t iCnt = 0u;
      TData<ty> data;
      do {
         auto tmp = buffer.find(';', pos);
         if (tmp > end) tmp = end;
         funcs[iCnt++](data, buffer.substr(pos, tmp - pos));
         pos = tmp + 1;
         } 
      while (pos < end);
         vData.emplace_back(std::forward<TData<ty>>(data));
    }
   return vData.size();
}

template <typename ty>
inline size_t Read_2(data_vector<ty>& vData, func_vector_vw const& funcs, std::string const& buffer) {
   std::string_view view(buffer.c_str(), buffer.size());
   using my_size_t = data_vector<ty>::size_type;
   using my_pair = std::pair< my_size_t, my_size_t>;
   for (auto [pos, end] = my_pair { 0u, view.find('\n') }; end != std::string_view::npos; pos = end + 1u, end = view.find('\n', pos)) {
      size_t iCnt = 0u;
      TData<ty> data;
      do {
         auto tmp = view.find(';', pos);
         if (tmp > end) tmp = end;
         funcs[iCnt++](data, view.substr(pos, tmp - pos));
         pos = tmp + 1;
      } while (pos < end);
      vData.emplace_back(std::forward<TData<ty>>(data));
   }
   return vData.size();
}


void Test1(std::string const& strFilename) {
   std::cout << "1st Test, Reading sequential with getline, vector dynamic increased.\n";
   data_vector<double> vecData;
   std::ifstream ifs(strFilename);
   if (!ifs.is_open()) {
      std::cerr << "File \"" << strFilename << "\" can't open!\n";
      return;
   }

   std::chrono::milliseconds runtime;
   std::cout << Call(runtime, Read_0<double>, std::ref(vecData), std::cref(funcs), std::ref(ifs)) << " datasets read\n";
   std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";
   
   std::ofstream ofs("D:\\Test\\Testausgabe.txt");
   Write<double>(vecData, ofs);
}

std::array<char, 4'096 * 100> buffer;

void Test2(std::string const& strFilename) {
   std::cout << "2nd Test, Reading sequential with getline, changed read buffer, vector dynamic increased.\n";
   data_vector<double> vecData;
   std::ifstream ifs(strFilename);
   ifs.rdbuf()->pubsetbuf(buffer.data(), buffer.size());
   if (!ifs.is_open()) {
      std::cerr << "File \"" << strFilename << "\" can't open!\n";
      return;
   }

   std::chrono::milliseconds runtime;
   std::cout << Call(runtime, Read_0<double>, std::ref(vecData), std::cref(funcs), std::ref(ifs)) << " datasets read\n";
   std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";

   std::ofstream ofs("D:\\Test\\Testausgabe.txt");
   Write<double>(vecData, ofs);
}

void Test3(std::string const& strFilename) {
   std::cout << "3rd Test, Reading sequential 2 times,  with getline, changed read buffer, vector prereserved.\n";
   data_vector<double> vecData;
   std::ifstream ifs(strFilename);
   ifs.rdbuf()->pubsetbuf(buffer.data(), buffer.size());
   if (!ifs.is_open()) {
      std::cerr << "File \"" << strFilename << "\" can't open!\n";
      return;
   }

   vecData.reserve(std::count(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), '\n'));
   // stronger  vecData.reserve(std::count(std::execution::par, std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), '\n'));
   ifs.seekg(0, std::ios::beg);

   std::chrono::milliseconds runtime;
   std::cout << Call(runtime, Read_0<double>, std::ref(vecData), std::cref(funcs), std::ref(ifs)) << " datasets read\n";
   std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";

   std::ofstream ofs("D:\\Test\\Testausgabe.txt");
   Write<double>(vecData, ofs);
}



void Test4(std::string const& strFilename) {
   std::cout << "4th Test, Reading sequential about stringstream with getline, vector reserved.\n";
   std::ifstream ins(strFilename, std::ifstream::binary);
   if(!ins) {
      std::cerr << "file with name \"" << strFilename << "\" can't open.\n";
      return;
      }
   else {
      ins.seekg(0, std::ios::end);
      auto size = ins.tellg();
      ins.seekg(0, std::ios::beg);
      std::string strBuffer(size, '\0');
      ins.read(strBuffer.data(), size);
      ins.close();
      data_vector<double> vData;
      //vData.reserve(std::count(strBuffer.begin(), strBuffer.end(), '\n'));
      vData.reserve(std::count(std::execution::par, strBuffer.begin(), strBuffer.end(), '\n'));
      std::istringstream iss(strBuffer);

      std::chrono::milliseconds runtime;
      std::cout << Call(runtime, Read_0<double>, std::ref(vData), std::cref(funcs), std::ref(iss)) << " datasets read\n";
      std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";

      std::ofstream ofs("D:\\Test\\Testausgabe.txt");
      Write<double>(vData, ofs);
   }
}

void Test5(std::string const& strFilename) {
   std::cout << "5th Test, string operations, vector reserved.\n";
   std::ifstream ins(strFilename, std::ifstream::binary);
   if (!ins) {
      std::cerr << "file with name \"" << strFilename << "\" can't open.\n";
      return;
   }
   else {
      ins.seekg(0, std::ios::end);
      auto size = ins.tellg();
      ins.seekg(0, std::ios::beg);
      std::string strBuffer(size, '\0');
      ins.read(strBuffer.data(), size);
      ins.close();
      data_vector<double> vData;
      //vData.reserve(std::count(strBuffer.begin(), strBuffer.end(), '\n'));
      vData.reserve(std::count(std::execution::par, strBuffer.begin(), strBuffer.end(), '\n'));

      std::chrono::milliseconds runtime;
      std::cout << Call(runtime, Read_1<double>, std::ref(vData), std::cref(funcs), std::cref(strBuffer)) << " datasets read\n";
      std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";

      std::ofstream ofs("D:\\Test\\Testausgabe.txt");
      Write<double>(vData, ofs);

   }

}

void Test6(std::string const& strFilename) {
   std::cout << "6th Test, our iterator, vector reserved.\n";
   std::ifstream ins(strFilename, std::ifstream::binary);
   if (!ins) {
      std::cerr << "file with name \"" << strFilename << "\" can't open.\n";
      return;
   }
   else {
      //std::stringstream strStream;
      //strStream << ins.rdbuf();
      //std::string_view test(strStream.view());

      const auto iSize = fs::file_size(strFilename);
      std::string strBuffer(iSize, '\0');
      ins.read(strBuffer.data(), iSize);
      ins.close();
      std::string_view test(strBuffer.data(), strBuffer.size());

      my_line_count::reset();
      std::vector<my_line_count> lines;
      lines.reserve(std::count(test.begin(), test.end(), '\n'));
      my_lines file_data(test);
      std::copy(file_data.begin(), file_data.end(), std::back_inserter(lines));
      data_vector<double> vData;
      vData.resize(lines.size());
      auto func_start = std::chrono::high_resolution_clock::now();
      std::for_each(std::execution::par, lines.begin(), lines.end(), [&vData](auto const& val) {
                                                  size_t iPos = 0u, iEnd;
                                                  int iCount = 0;  // funcs_vw
                                                  do {
                                                     iEnd = val.view.find(';', iPos);
                                                     funcs_vw[iCount++](vData[val.index], val.view.substr(iPos, iEnd - iPos));
                                                     if (iEnd != std::string_view::npos) iPos = iEnd + 1;
                                                     } 
                                                  while (iEnd != std::string_view::npos);
                                                  } );
      auto func_ende = std::chrono::high_resolution_clock::now();
      auto time_ = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);
      std::cout << vData.size() << " datasets read\n" 
                << std::setw(12) << std::setprecision(3) << time_.count() / 1000. << " sec\n";
      
      std::ofstream ofs("D:\\Test\\Testausgabe.txt");
      Write<double>(vData, ofs);
   }

}


int main() {
   std::ios_base::sync_with_stdio(false);
   std::cout.setf(std::ios::showpoint);
   std::cout.setf(std::ios::fixed);
/*
   TData<double> data;
   Location<double> point  = { 2.0, 1.0 };
   std::cout << "Hello World!\n";
   Calculate<double>(data, point);
 */

   Test6("D:\\Test\\berlin_infos.dat");

   return 0;
}

