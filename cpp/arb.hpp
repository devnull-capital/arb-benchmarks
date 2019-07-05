#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

#ifndef ARB_H_
#define ARB_H_

namespace arb {

struct rate {
  string from, to, exchange;
  double rate, vol;
};

vector< vector< vector<rate*> > > arbFromRates(const vector<rate*> &rates, const unsigned int depth);
vector< vector< vector<rate*> > > arbFromCombos(const vector< vector< vector<rate*> > > &combos);
bool isDupe(const vector<vector<rate*> > &list, const vector<rate*> &arb);
bool isArb(const vector<rate*> &list);
vector< vector< vector<rate*> > > combosFromRates(const vector<rate*> &rates, const unsigned int depth);
bool isListClosing(const vector<rate*> &list);
void buildBase(const vector<rate*> &rates, vector< vector<rate*> > &ret);
bool isRateInList(const vector<rate*> &list, const rate *r);

} // namespace arb

#endif // ARB_H_
