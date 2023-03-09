// Test.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <utility>
#include <string>
#include <string_view> // !! C++17
#include <vector>
#include <array>
#include <fstream>
#include <chrono>
#include <functional>
#include <execution> // !! C++17
#include <iterator>
#include <optional> // !! C++17
#include <atomic>

using namespace std::literals;

struct my_line_iterator {
   using iterator_category = std::input_iterator_tag;
   using value_type        = std::string_view;
   using difference_type   = std::ptrdiff_t;
   using reference_type    = const value_type&;
   using pointer_type      = const value_type*;

   my_line_iterator() { };
   my_line_iterator(std::string_view const& input) : theText(input) { ++* this; };

   my_line_iterator& operator = (std::string_view const& vw) {
      theText = vw;
      start_pos = 0u;
      end_pos = 0u;
      return *this;
      }

   my_line_iterator& operator = (my_line_iterator const& ref) {
      theText = ref.theText;
      theLine = ref.theLine;
      start_pos = ref.start_pos;
      end_pos = ref.end_pos;
      return *this;
      }

   reference_type operator*() const { return theLine; }
   pointer_type operator->() const { return &theLine; }

   my_line_iterator& operator++() {
      if(theText) {
         end_pos = theText->find('\n', start_pos);
         if(end_pos != std::string_view::npos) {
            theLine = theText->substr(start_pos, end_pos - start_pos);
            start_pos = end_pos + 1;
            }
         else {
            theText = { };
            // theText = std::nullopt;
            }
         }
      return *this;
      }

   my_line_iterator operator++(int) {
      auto elem(*this);
      ++* this;
      return elem;
   }

   friend  bool operator == (my_line_iterator const& lhs, my_line_iterator const& rhs) {
      return lhs.theText == rhs.theText;
      }

   friend  bool operator != (my_line_iterator const& lhs, my_line_iterator const& rhs) {
      return !(lhs == rhs);
   }

private:
   std::optional<std::string_view> theText = { };
   std::string_view theLine;
   size_t start_pos = 0u, end_pos = 0u;
};

template <typename tty, typename fty, typename... Args>
inline auto Call(tty& time, fty function, Args... args) {
   auto func_start = std::chrono::high_resolution_clock::now();
   auto ret = function(std::forward<Args>(args)...);
   auto func_ende = std::chrono::high_resolution_clock::now();
   time = std::chrono::duration_cast<tty>(func_ende - func_start);
   return ret;
}

//using myType = double;

template <typename ty>
using Location = std::pair<ty, ty>;

template <typename ty>
std::ostream& operator << (std::ostream& out, Location<ty> const& data) {
   out << "(" << data.first << ", " << data.second << ")";
   return out;
   } 

/** Klasse mit den notwendige Daten für eine Adresse (ohne geografische Position)
*/
class TAddress {
   friend void swap(TAddress& lhs, TAddress& rhs) {
      swap(lhs.strCity, rhs.strCity);
      swap(lhs.strStreet, rhs.strStreet);
      swap(lhs.strStreetNumber, rhs.strStreetNumber);
      swap(lhs.strZipCode, rhs.strZipCode);
      swap(lhs.strUrbanUnit, rhs.strUrbanUnit);
      swap(lhs.strUrbanUnit_Old, rhs.strUrbanUnit_Old);
      swap(lhs.strDistrict, rhs.strDistrict);
   }
private:
   std::string strCity          = "";  ///< Stadt, an der sich die Adresse befindet
   std::string strStreet        = "";  ///< Straße zu dieser Adresse
   std::string strStreetNumber  = "";  ///< Hausnummer zu dieser Straße
   std::string strZipCode       = "";  ///< Postleitzahl
   std::string strUrbanUnit     = "";  ///< Stadtbezirk
   std::string strUrbanUnit_Old = "";  ///< Stadtbezirk (alte Aufteilung)
   std::string strDistrict      = "";  ///< Ortsteil

public:
   TAddress(void) {
      _init();
      }

   TAddress(TAddress const& ref) {
      _copy(ref);
      }

   TAddress(TAddress&& ref) noexcept {
      _swap(std::forward<TAddress>(ref));
   }


   virtual ~TAddress(void) = default;

   virtual TAddress* create() { return new TAddress;  }

   TAddress& operator = (TAddress const& ref) {
      copy(ref);
      return *this;
      }

   TAddress& operator = (TAddress&& ref) noexcept {
      swap(*this, ref);
      return *this;
   }


   virtual void init() { _init(); }
   virtual void copy(TAddress const& ref) { _copy(ref); }


   /// /name Selektoren
   /// {
   std::string const& City(void) const { return strCity; }
   std::string const& Street(void) const { return strStreet; }
   std::string const& StreetNumber(void) const { return strStreetNumber; }
   std::string const& ZipCode(void) const { return strZipCode; }
   std::string const& UrbanUnit(void) const { return strUrbanUnit; }
   std::string const& UrbanUnit_Old(void) const { return strUrbanUnit_Old; }
   std::string const& District(void) const { return strDistrict; }
   /// }

   void City(std::string const& newVal) { strCity = newVal; }
   void Street(std::string const& newVal) { strStreet = newVal; }
   void StreetNumber(std::string const& newVal) { strStreetNumber = newVal; }
   void ZipCode(std::string const& newVal) { strZipCode = newVal; }
   void UrbanUnit(std::string const& newVal) { strUrbanUnit = newVal; }
   void UrbanUnit_Old(std::string const& newVal) { strUrbanUnit_Old = newVal; }
   void District(std::string const& newVal) { strDistrict = newVal; }

   void City(std::string&& newVal) { strCity = std::forward<std::string>(newVal); }
   void Street(std::string&& newVal) { strStreet = std::forward<std::string>(newVal); }
   void StreetNumber(std::string&& newVal) { strStreetNumber = std::forward<std::string>(newVal); }
   void ZipCode(std::string&& newVal) { strZipCode = std::forward<std::string>(newVal); }
   void UrbanUnit(std::string&& newVal) { strUrbanUnit = std::forward<std::string>(newVal); }
   void UrbanUnit_Old(std::string&& newVal) { strUrbanUnit_Old = std::forward<std::string>(newVal); }
   void District(std::string&& newVal) { strDistrict = std::forward<std::string>(newVal); }

   void City(std::string_view const& newVal) { strCity = std::move(std::string{ newVal.data(), newVal.size() }); }
   void Street(std::string_view const& newVal) { strStreet = std::move(std::string{ newVal.data(), newVal.size() }); }
   void StreetNumber(std::string_view const& newVal) { strStreetNumber = std::move(std::string{ newVal.data(), newVal.size() }); }
   void ZipCode(std::string_view const& newVal) { strZipCode = std::move(std::string{ newVal.data(), newVal.size() }); }
   void UrbanUnit(std::string_view const& newVal) { strUrbanUnit = std::move(std::string{ newVal.data(), newVal.size() }); }
   void UrbanUnit_Old(std::string_view const& newVal) { strUrbanUnit_Old = std::move(std::string{ newVal.data(), newVal.size() }); }
   void District(std::string_view const& newVal) { strDistrict = std::move(std::string{ newVal.data(), newVal.size() }); }


private:
   void _init(void) {
      strCity          = "";
      strStreet        = "";
      strStreetNumber  = "";
      strZipCode       = "";
      strUrbanUnit     = "";
      strUrbanUnit_Old = "";
      strDistrict      = "";
      }
   void _copy(TAddress const& ref) {
      City(ref.City());
      Street(ref.Street());
      StreetNumber(ref.StreetNumber());
      ZipCode(ref.ZipCode());
      UrbanUnit(ref.UrbanUnit());
      UrbanUnit_Old(ref.UrbanUnit_Old());
      District(ref.District());
      }

   void _swap(TAddress&& ref) noexcept {
      swap(strCity, ref.strCity);
      swap(strStreet, ref.strStreet);
      swap(strStreetNumber, ref.strStreetNumber);
      swap(strZipCode, ref.strZipCode);
      swap(strUrbanUnit, ref.strUrbanUnit);
      swap(strUrbanUnit_Old, ref.strUrbanUnit_Old);
      swap(strDistrict, ref.strDistrict);
   }
};


template <typename ty>
class TData : public TAddress {
   friend void swap(TData& lhs, TData&rhs) {
      swap(static_cast<TAddress&>(lhs), static_cast<TAddress&>(rhs));
      swap(lhs.mLoc, rhs.mLoc);
      }

private:
   Location<ty> mLoc;
public:
   TData(void) : TAddress() { _init(); }
   TData(TData const& ref) : TAddress(ref) { _copy(ref);  }

   TData(TData&& ref) noexcept : TAddress(ref) { _swap(std::forward<TData<ty>>(ref));  }

   virtual ~TData(void) override { }

   TData& operator = (TData const& ref) {
      copy(ref);
      return *this;
      }

   TData& operator = (TData&& ref) noexcept {
      _swap(std::forward<TData>(ref));
      return *this;
   }

   virtual TAddress* create() { return new TData<ty>; }
   virtual void init(void) { TAddress::init();  _init(); };
   virtual void copy(TAddress const& ref) { 
       TAddress::copy(ref);
       try {
          _copy(dynamic_cast<TData const&>(ref));
          }
       catch (std::bad_cast&) { ; }
   }

   operator Location<ty> & ()  { return mLoc; }
   operator Location<ty> const& () const { return mLoc;  }

   ty const& Latitude(void) const { return mLoc.first; }
   ty const& Longitude(void) const { return mLoc.second; }


   void Latitude(ty const& newVal) { mLoc.first = newVal; }
   void Longitude(ty const& newVal) { mLoc.second = newVal; }

   void Latitude(std::string const& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.first); }  // mLoc.first = std::stod(newVal); }
   void Longitude(std::string const& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.second); } //mLoc.second = std::stod(newVal); }

   void Latitude(std::string&& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.first); }  // mLoc.first = std::stod(newVal); }
   void Longitude(std::string&& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.second); } //mLoc.second = std::stod(newVal); }

   void Latitude(std::string_view const& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.first); }  // mLoc.first = std::stod(newVal); }
   void Longitude(std::string_view const& newVal) { auto result = std::from_chars(newVal.data(), newVal.data() + newVal.size(), mLoc.second); } //mLoc.second = std::stod(newVal); }

private:
   void _init(void) { mLoc = { 0.0, 0.0 }; }
   void _copy(TData const& ref) { mLoc = ref.mLoc; }
   void _swap(TData&& ref) noexcept { swap(mLoc, ref.mLoc);  }
};

template <typename ty>
using data_vector = std::vector<TData<ty>>;

std::vector<std::string> tokenize(std::string const& strInput, std::string const& strDelimiter, int iSize = -1) {
   std::vector<std::string> vecData;
   if (iSize >= 0) vecData.reserve(iSize);
   size_t iPos = 0, iEnd;
   do {
      iEnd = strInput.find(strDelimiter, iPos);
      std::string strPart = strInput.substr(iPos, iEnd - iPos);
      vecData.emplace_back(std::forward<std::string>(strPart));
      iPos = iEnd + strDelimiter.length();
   } while (iEnd != std::string::npos);
   return vecData;
}

std::string ReadHlp(std::string const& strRow, size_t& iPos) {
   size_t iEnd = strRow.find(";", iPos), iOldPos(iPos);
   iPos = iEnd + 1;
   return strRow.substr(iPos, iEnd - iOldPos);
   }

using func_vector = std::vector<std::function<void(TData<double>&, std::string const&)>>;

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


using func_vector_vw = std::vector<std::function<void(TData<double>&, std::string_view const&)>>;

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


template <typename ty>
inline void Write(data_vector<ty>& vData, std::ostream& os) {
   os.setf(std::ios::showpoint);
   os.setf(std::ios::fixed);
   os.precision(6);
   std::for_each(vData.cbegin(), vData.cend(), [&os](auto const& val) {
      os << val.ZipCode() << " " << val.City() << " / " << val.UrbanUnit() << ", "
         << val.Street() << " " << val.StreetNumber() << " -> (" << val.Latitude() << ", "
         << val.Longitude() << ")\n";
      });
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
   std::cout << "3rd Test, string operations, vector reserved.\n";
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
      vData.reserve(std::count(strBuffer.begin(), strBuffer.end(), '\n'));
      //vData.reserve(std::count(std::execution::par, strBuffer.begin(), strBuffer.end(), '\n'));

      std::chrono::milliseconds runtime;
      std::cout << Call(runtime, Read_2<double>, std::ref(vData), std::cref(funcs_vw), std::cref(strBuffer)) << " datasets read\n";
      std::cout << std::setw(12) << std::setprecision(3) << runtime.count() / 1000. << " sec\n";

      std::ofstream ofs("D:\\Test\\Testausgabe.txt");
      Write<double>(vData, ofs);

   }

}



template< typename ty>
void Calculate(Location<ty> const& a, Location<ty> const& b) {
   std::cout << a << " -> " << b << "\n";
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

   Test5("D:\\Test\\berlin_infos.dat");

   return 0;
}

