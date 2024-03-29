// Move.cpp : Diese Datei enth�lt die Funktion "main". Hier beginnt und endet die Ausf�hrung des Programms.
//

#include <iostream>
#include <vector>
#include <algorithm>

class TTracer {
   friend std::ostream& operator << (std::ostream& out, TTracer const& data) { out << data.i; return out; }
   friend void swap(TTracer& lhs, TTracer& rhs) {
      std::swap(lhs, rhs);
      }
private:
   int i;

public:
   TTracer(void) { std::cout << "standard constructed with i = 0" << std::endl; i = 0; };

   TTracer(TTracer const& ref) { std::cout << "copy constructed with i = " << ref.i << std::endl; i = ref.i; };
   TTracer(TTracer && ref) { std::cout << "pirate constructed with i = " << ref.i << std::endl; std::swap(i, ref.i); };

   TTracer(int val) { std::cout << "initialization constructed with i = " << val << std::endl; i = val; };
   ~TTracer(void) { std::cout << "destructed with i = " << i << std::endl; };

   TTracer& operator = (TTracer const& ref) { std::cout << "assign operator with i = " << ref << std::endl; i = ref.i; return *this; };
   TTracer& operator = (TTracer && ref) { std::cout << "rvalue operator with i = " << ref << std::endl; swap(*this, ref); return *this; };
   TTracer& operator = (int ref) { std::cout << "assign operator with i = " << ref << std::endl; i = ref; return *this; };

   operator int() { std::cout << "conversion operator i = " << i << std::endl; return i; };

};

int Funktion(void) {
   return 5;
   }

std::vector<TTracer> Funktion2(void) {
   std::vector<TTracer> vecVal;  // Variable auf dem "Stack"
   vecVal.reserve(30);
   std::cout << "begin of function2" << std::endl;
   //for(int i = 0; i < 30; ++i) vecVal.push_back(i * 5);
   for (int i = 0; i < 30; ++i) vecVal.push_back(i * 5); // C++11
   return vecVal; // mit C++11 keine Kopie mehr
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

//std::vector<TTracer> traced_vals; // "Heap"

int main() {
   {
   int x = Funktion();
   std::cout << "Der R�ckgabewert ist " << x << "\n";

   std::vector<TTracer> traced_vals; // "Stack"
   traced_vals = Funktion2();
   std::cout << "back from function2" << std::endl;
   for (std::vector<TTracer>::iterator it = traced_vals.begin(); it != traced_vals.end(); ++it) std::cout << *it << std::endl;

   //std::vector<int> vals;
   //Funktion3_98(vals);
   //for (std::vector<int>::iterator it = vals.begin(); it != vals.end(); ++it) std::cout << *it << std::endl;


   //std::vector<int> vals = Funktion4(Funktion4_1());
   //for (std::vector<int>::iterator it = vals.begin(); it != vals.end(); ++it) std::cout << *it << std::endl;

   std::cout << "end of main" << std::endl;
   }
   getchar();
   }


