#pragma once

#include "MyAddress.h"
#include "MyLocation.h"

#include <algorithm>
#include <charconv>
#include <vector>

template <typename ty>
class TData : public TAddress {
   friend void swap(TData& lhs, TData& rhs) {
      swap(static_cast<TAddress&>(lhs), static_cast<TAddress&>(rhs));
      swap(lhs.mLoc, rhs.mLoc);
   }

private:
   Location<ty> mLoc;
public:
   TData(void) : TAddress() { _init(); }
   TData(TData const& ref) : TAddress(ref) { _copy(ref); }

   TData(TData&& ref) noexcept : TAddress(ref) { _swap(std::forward<TData<ty>>(ref)); }

   virtual ~TData(void) override { }

   TData& operator = (TData const& ref) {
      copy(ref);
      return *this;
   }

   TData& operator = (TData&& ref) noexcept {
      _swap(std::forward<TData>(ref));
      return *this;
   }

   virtual TAddress* create() override { return new TData<ty>; }
   virtual void init(void) override { TAddress::init();  _init(); };
   virtual void copy(TAddress const& ref) override {
      TAddress::copy(ref);
      try {
         _copy(dynamic_cast<TData const&>(ref));
      }
      catch (std::bad_cast&) { ; }
   }

   operator Location<ty>& () { return mLoc; }
   operator Location<ty> const& () const { return mLoc; }

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
   void _swap(TData&& ref) noexcept { swap(mLoc, ref.mLoc); }
};

template <typename ty>
using data_vector = std::vector<TData<ty>>;

using func_vector = std::vector<std::function<void(TData<double>&, std::string const&)>>;

using func_vector_vw = std::vector<std::function<void(TData<double>&, std::string_view const&)>>;


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

