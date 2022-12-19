// Move.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

class TTracer {
   friend std::ostream& operator << (std::ostream& out, TTracer const& data) { out << data.i << " (TTrace)"; return out; }
   friend void swap(TTracer& lhs, TTracer& rhs) {
      std::swap(lhs.i, rhs.i);
   }
private:
   int i = 0;

public:
   TTracer(void) { std::cout << "standard constructed with i = 0" << std::endl; i = 0; };

   TTracer(TTracer const& ref) { std::cout << "copy constructed with i = " << ref.i << std::endl; i = ref.i; };
   TTracer(TTracer&& ref) noexcept { std::cout << "pirate constructed with i = " << ref << std::endl; std::swap(*this, ref); };

   TTracer(int val) { std::cout << "initialization constructed with i = " << val << std::endl; i = val; };
   ~TTracer(void) { std::cout << "destructed with i = " << i << std::endl; };

   TTracer& operator = (TTracer const& ref) { std::cout << "assign operator with i = " << ref << std::endl; i = ref.i; return *this; };
   TTracer& operator = (TTracer&& ref) noexcept { std::cout << "rvalue operator with i = " << ref << std::endl; swap(*this, ref); return *this; };
   TTracer& operator = (int const& ref) { std::cout << "assign operator with i = " << ref << std::endl; i = ref; return *this; };

   operator int() { std::cout << "conversion operator i = " << i << std::endl; return i; };

};

TTracer Funktion(int x) {
   TTracer ret = x;
   return ret;
   }

std::vector<TTracer> Funktion2(void) {
   std::vector<TTracer> vecVal;  // Variable auf dem "Stack"
   vecVal.reserve(30);
   std::cout << "begin of function2" << std::endl;
   //for(int i = 0; i < 30; ++i) vecVal.push_back(i * 5);
   for (int i = 0; i < 30; ++i) vecVal.emplace_back(i * 5); // C++11
   return vecVal; // mit C++11 keine Kopie mehr
   }



void Funktion2_98(std::vector<TTracer>& vecVal) {
     
   vecVal.clear();
   vecVal.reserve(30);
   std::cout << "begin of function2_98" << std::endl;
   //for(int i = 0; i < 30; ++i) vecVal.push_back(i * 5);
   for (int i = 0; i < 30; ++i) vecVal.emplace_back(i * 5); // C++11
   return; 
}


int rechne(int val) { 
   return val * 5;  
   }

void Funktion3_98(std::vector<int>& vecVal) {
   vecVal.clear();
   for (int i = 0; i < 30; ++i) vecVal.push_back(i * 5);
   return;
}

// vor C++11 mit Kopie, danach move ( named return value optimization / unnamed ...)
std::vector<int> Funktion3() {
   std::vector<int> vecVal;
   for (int i = 0; i < 30; ++i) vecVal.push_back(i * 5);
   return vecVal;
}

std::vector<int> Funktion4_1() {
   std::vector<int> vecVal;
   for (int i = 0; i < 30; ++i) vecVal.push_back(i);
   return vecVal;
}

std::vector<int> Funktion4(std::vector<int> values) {
   //std::for_each(values.begin(), values.end(), rechne);
   std::transform(values.begin(), values.end(), values.begin(), rechne);
   return values;
}

std::vector<TTracer> traced_vals; // "Heap"


void test(int para, TTracer& t1, TTracer& t2, TTracer& t3) {
   t1 = 1 * para;
   t2 = 2 * para;
   t3 = 3 * para;
   return;
}

std::tuple<TTracer, TTracer, TTracer> test(int para = 1) {
   return { 1 * para, 2 * para, 3 * para };
   } 

int main() {
   TTracer val;
   val = Funktion(5);
   std::cout << val << "\n";

   //TTracer x = 10;  // Dieses Wert die Variable für Bla Bla Bla
   //auto x = Funktion();
   //std::cout << "Der Rückgabewert ist " << x << "\n";

   //traced_vals = Funktion2();
   //std::cout << "back from function2" << std::endl;
   //for (std::vector<TTracer>::iterator it = traced_vals.begin(); it != traced_vals.end(); ++it) std::cout << *it << std::endl;

   //std::vector<int> vals;
   //Funktion3_98(vals);
   //for (std::vector<int>::iterator it = vals.begin(); it != vals.end(); ++it) std::cout << *it << std::endl;


   //std::vector<int> vals = Funktion4(Funktion4_1());
   //for (std::vector<int>::iterator it = vals.begin(); it != vals.end(); ++it) std::cout << *it << std::endl;

   //TTracer t1, t2, t3;
   //test(1, t1, t2, t3);
   //std::cout << t1 << ", " << t2 << ", " << t3 << std::endl;

   //auto val = test(1);
   //std::cout << std::get<0>(val) << ", " << std::get<1>(val) << ", " << std::get<2>(val) << std::endl;
 
   //TTracer t1, t2, t3;
   //std::tie(t1, t2, t3) = test();
   //auto [t1, t2, t3] = test(); 
   //std::cout << t1 << ", " << t2 << ", " << t3 << std::endl;

   //std::cout << "end of main" << std::endl;
   }

