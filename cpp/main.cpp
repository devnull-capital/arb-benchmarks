#include "arb.hpp"
using namespace std;

constexpr unsigned int depth = 5;

int main ()
{
  rate *r1 = new rate{"a", "b", "foo", 2.0, 0.0};
  rate *r2 = new rate{"a", "c", "foo", 2.0, 0.0};
  rate *r3 = new rate{"a", "d", "foo", 2.0, 0.0};
  rate *r4 = new rate{"a", "e", "foo", 2.0, 0.0};
  rate *r5 = new rate{"b", "a", "foo", 2.0, 0.0};
  rate *r6 = new rate{"b", "c", "foo", 2.0, 0.0};
  rate *r7 = new rate{"b", "d", "foo", 2.0, 0.0};
  rate *r8 = new rate{"b", "e", "foo", 2.0, 0.0};
  rate *r9 = new rate{"c", "a", "foo", 2.0, 0.0};
  rate *r10 = new rate{"c", "b", "foo", 2.0, 0.0};
  rate *r11 = new rate{"c", "d", "foo", 2.0, 0.0};
  rate *r12 = new rate{"c", "e", "foo", 2.0, 0.0};
  rate *r13 = new rate{"d", "a", "foo", 2.0, 0.0};
  rate *r14 = new rate{"d", "b", "foo", 2.0, 0.0};
  rate *r15 = new rate{"d", "c", "foo", 2.0, 0.0};
  rate *r16 = new rate{"d", "e", "foo", 2.0, 0.0};
  rate *r17 = new rate{"e", "a", "foo", 2.0, 0.0};
  rate *r18 = new rate{"e", "b", "foo", 2.0, 0.0};
  rate *r19 = new rate{"e", "c", "foo", 2.0, 0.0};
  rate *r20 = new rate{"e", "d", "foo", 2.0, 0.0};

  vector<rate*> rates = {
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    r16,
    r17,
    r18,
    r19,
    r20,
  };

  int i;

  // note: we don't want the compiler to optimize by removing the output
  vector< vector< vector<rate*> > > arb(depth);
  for (i = 0; i < 100; ++i) {
    arb = arbFromRates(rates, depth);
  }

  cout << "tmp len: " << arb.size() << "\n";

  delete r1;
  delete r2;
  delete r3;
  delete r4;
  delete r5;
  delete r6;
  delete r7;
  delete r8;
  delete r9;
  delete r10;
  delete r11;
  delete r12;
  delete r13;
  delete r14;
  delete r15;
  delete r16;
  delete r17;
  delete r18;
  delete r19;
  delete r20;

  return 0;
}
