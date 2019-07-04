#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
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
    r20,
  };
  
  //vector<rate*> rv = {};
  //vector< vector<rate*>* > rvv = {&rv};

  int i;
  vector< vector< vector<rate*>* > *> ret(5);
  for (i = 0; i < ret.size(); ++i) {
    vector<rate*> rv = {};
    vector< vector<rate*>* > rvv = {&rv};
    ret.at(i) = {&rvv};
  }

  vector< vector< vector<rate*>* > *> combos(5);
  for (i = 0; i < combos.size(); ++i) {
    vector<rate*> rv = {};
    vector< vector<rate*>* > rvv = {&rv};
    ret.at(i) = {&rvv};
  }
  arbFromRates(rates, 5, combos, ret);

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
  buildBase(rates, *combos.at(0));

  int i, j, k;
  for (i = 1; i < depth; ++i) {
    cout << "i: " << i << "\n";
    for (j = 0; j < combos.at(i-1)->size(); ++j) {
      cout << "j: " << j << "\n";
      for (k = 0; k < rates.size(); ++k) {
        cout << "k: " << k << "\n";
        if (combos.at(i-1)->at(j)->back()->to == rates.at(k)->from &&
        !(find(combos.at(i-j)->at(j)->begin(), combos.at(i-j)->at(j)->end(), rates.at(k)) != combos.at(i-j)->at(j)->end()) &&
        !isListClosing(*combos.at(i-1)->at(j))) {
          vector<rate*> *newV(combos.at(i-1)->at(j));
          newV->push_back(rates.at(k));
          combos.at(i)->push_back(newV);
        }
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
  cout << "creating ret\n";
  ret = {};
  cout << "done creating ret\n";

  int i, j;
  for (i = 0; i < rates.size(); ++i) {
    cout << "build base i: " << i << "\n";
    for (j = i + 1; j < rates.size(); ++j) {
      cout << "build base j: " << j << "\n";
      if (rates.at(i)->to == rates.at(j)->from) {
        for (auto i: ret)
            std::cout << i << ' ';
        vector<rate*> v = {
          rates.at(i),
          rates.at(j),
        };
        cout << "adding rates\n";

        ret.push_back(&v);
      }
    }
  }
}

bool isListClosing(const vector<rate*> &list)
{
  return list.front()->from == list.back()->to;
}
