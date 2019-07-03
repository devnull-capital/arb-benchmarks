#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct rate {
  string from, to, exchange;
  double rate, vol;
};

void arbFromRates(const vector<rate*> &rates, const unsigned int depth, vector< vector< vector<rate*>* > *> &combos, vector< vector< vector<rate*>* > *> &ret);
void arbFromCombos(const vector< vector< vector<rate*>* >* > &combos, vector< vector< vector<rate*>* >* > &arb);
bool isDupe(const vector<vector<rate*>* > &list, const vector<rate*> &arb);
bool isArb(const vector<rate*> &list);
void combosFromRates(const vector<rate*> &rates, const unsigned int depth, vector< vector< vector<rate*>* >* > &combos);
bool isListClosing(const vector<rate*> &list);
void buildBase(const vector<rate*> &rates, vector< vector<rate*>* > &ret);
bool isRateInList(const vector<rate*> &list, rate &r);

int main ()
{
  rate* r1 = new rate{"a", "b", "foo", 2.0, 0.0};
  rate* r2 = new rate{"a", "c", "foo", 2.0, 0.0};
  rate* r3 = new rate{"a", "d", "foo", 2.0, 0.0};
  rate* r4 = new rate{"a", "e", "foo", 2.0, 0.0};
  rate* r5 = new rate{"b", "a", "foo", 2.0, 0.0};
  rate* r6 = new rate{"b", "c", "foo", 2.0, 0.0};
  rate* r7 = new rate{"b", "d", "foo", 2.0, 0.0};
  rate* r8 = new rate{"b", "e", "foo", 2.0, 0.0};
  rate* r9 = new rate{"c", "a", "foo", 2.0, 0.0};
  rate* r10 = new rate{"c", "b", "foo", 2.0, 0.0};
  rate* r11 = new rate{"c", "d", "foo", 2.0, 0.0};
  rate* r12 = new rate{"c", "e", "foo", 2.0, 0.0};
  rate* r13 = new rate{"d", "a", "foo", 2.0, 0.0};
  rate* r14 = new rate{"d", "b", "foo", 2.0, 0.0};
  rate* r15 = new rate{"d", "c", "foo", 2.0, 0.0};
  rate* r16 = new rate{"d", "e", "foo", 2.0, 0.0};
  rate* r17 = new rate{"e", "a", "foo", 2.0, 0.0};
  rate* r18 = new rate{"e", "b", "foo", 2.0, 0.0};
  rate* r19 = new rate{"e", "c", "foo", 2.0, 0.0};
  rate* r20 = new rate{"e", "d", "foo", 2.0, 0.0};

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
    r20
  };
  
  vector< vector< vector<rate*>* > *> ret(5);
  vector< vector< vector<rate*>* > *> combos(5);
  arbFromRates(rates, 5, combos, ret);

  cout << "done!\n";
  return 0;
}


void arbFromRates(const vector<rate*> &rates, const unsigned int depth, vector< vector< vector<rate*>* > *> &combos, vector< vector< vector<rate*>* > *> &ret)
{
  combosFromRates(rates, depth, combos);
  return arbFromCombos(combos, ret);
}

void combosFromRates(const vector<rate*> &rates, const unsigned int depth, vector< vector< vector<rate*>* >* > &combos)
{
  buildBase(rates, combos[0]);

  int i, j, k;
  for (i = 1; i < depth; ++i) {
    for (j = 0; j < combos[i-1]->size(); ++j) {
      for (k = 0; k < rates.size(); ++k) {
        cout << "in da loop\n";
      }
    }
  }
}

void arbFromCombos(const vector< vector< vector<rate*>* >* > &combos, vector< vector< vector<rate*>* >* > &arb)
{
  cout << "in arbFromCombos\n";
}

void buildBase(const vector<rate*> &rates, vector< vector<rate*>* > &ret)
{
  cout << "in buildBase\n";
}
