#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

constexpr unsigned int depth = 5;

struct rate {
  string from, to, exchange;
  double rate, vol;
};

rate r1 = rate{"a", "b", "foo", 2.0, 0.0};
rate r2 = rate{"a", "c", "foo", 2.0, 0.0};
rate r3 = rate{"a", "d", "foo", 2.0, 0.0};
rate r4 = rate{"a", "e", "foo", 2.0, 0.0};
rate r5 = rate{"b", "a", "foo", 2.0, 0.0};
rate r6 = rate{"b", "c", "foo", 2.0, 0.0};
rate r7 = rate{"b", "d", "foo", 2.0, 0.0};
rate r8 = rate{"b", "e", "foo", 2.0, 0.0};
rate r9 = rate{"c", "a", "foo", 2.0, 0.0};
rate r10 = rate{"c", "b", "foo", 2.0, 0.0};
rate r11 = rate{"c", "d", "foo", 2.0, 0.0};
rate r12 = rate{"c", "e", "foo", 2.0, 0.0};
rate r13 = rate{"d", "a", "foo", 2.0, 0.0};
rate r14 = rate{"d", "b", "foo", 2.0, 0.0};
rate r15 = rate{"d", "c", "foo", 2.0, 0.0};
rate r16 = rate{"d", "e", "foo", 2.0, 0.0};
rate r17 = rate{"e", "a", "foo", 2.0, 0.0};
rate r18 = rate{"e", "b", "foo", 2.0, 0.0};
rate r19 = rate{"e", "c", "foo", 2.0, 0.0};
rate r20 = rate{"e", "d", "foo", 2.0, 0.0};

vector<rate> rates = {
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

void arbFromRates(const vector<rate> &rates, const unsigned int depth, vector< vector< vector<rate> > > &combos, vector< vector< vector<rate> > > &ret);
void arbFromCombos(const vector< vector< vector<rate> > > &combos, vector< vector< vector<rate> > > &arb);
bool isDupe(const vector<vector<rate> > &list, const vector<rate> &arb);
bool isArb(const vector<rate> &list);
void combosFromRates(const vector<rate> &rates, const unsigned int depth, vector< vector< vector<rate> > > &combos);
bool isListClosing(const vector<rate> &list);
void buildBase(const vector<rate> &rates, vector< vector<rate> > &ret);
bool isRateInList(const vector<rate> &list, const rate &r);

int main ()
{
  vector< vector< vector<rate> > > ret(depth);
  vector< vector< vector<rate> > > combos(depth);

  arbFromRates(rates, depth, combos, ret);

  cout << "done!\n";
  return 0;
}


void arbFromRates(const vector<rate> &rates, const unsigned int depth, vector< vector< vector<rate> > > &combos, vector< vector< vector<rate> > > &ret)
{
  combosFromRates(rates, depth, combos);
  return arbFromCombos(combos, ret);
}

void combosFromRates(const vector<rate> &rates, const unsigned int depth, vector< vector< vector<rate> > > &combos)
{
  buildBase(rates, combos.at(0));

  int i, j, k;
  for (i = 1; i < depth; ++i) {
    for (j = 0; j < combos.at(i-1).size(); ++j) {
      for (k = 0; k < rates.size(); ++k) {
        if (combos.at(i-1).at(j).back().to == rates.at(k).from &&
        !isRateInList(combos.at(i-1).at(j), rates.at(k)) &&
        !isListClosing(combos.at(i-1).at(j))) {
          vector<rate> newV(combos.at(i-1).at(j));
          newV.push_back(rates.at(k));
          combos.at(i).push_back(newV);
        }
      }
    }
  }
}

void arbFromCombos(const vector< vector< vector<rate> > > &combos, vector< vector< vector<rate> > > &arb)
{
  int i, j;
  for (i = 0; i < combos.size(); ++i) {
    for (j = 0; j < combos.at(i).size(); ++j) {
      if (isArb(combos.at(i).at(j)) && !isDupe(arb.at(i), combos.at(i).at(j)))
          arb.at(i).push_back(combos.at(i).at(j));
    }
  }
}

void buildBase(const vector<rate> &rates, vector< vector<rate> > &ret)
{
  int i, j;
  for (i = 0; i < rates.size(); ++i) {
    for (j = i + 1; j < rates.size(); ++j) {
      if (rates.at(i).to == rates.at(j).from) {
        vector<rate> v = {
          rates.at(i),
          rates.at(j),
        };

        ret.push_back(v);
      }
    }
  }
}

bool isListClosing(const vector<rate> &list)
{
  return list.front().from == list.back().to;
}

bool isRateInList(const vector<rate> &list, const rate &r)
{
  int i;
  for (i = 0; i < list.size(); ++i) {
    if (list[i].from == r.from &&
    list[i].to == r.to &&
    list[i].exchange == r.exchange &&
    list[i].rate == r.rate &&
    list[i].vol == r.vol)
      return true;
  }

  return false;
}

bool isArb(const vector<rate> &list)
{
  if (list.size() < 2)
    return false;

  if (list.front().from != list.back().to)
    return false;

  double prod = list.front().rate;
  int i;
  for (i = 1; i < list.size(); ++i) {
    if (list.at(i-1).to != list.at(i).from)
      return false;

    prod *= list.at(i).rate;
  }

  return prod > 1.0;
}

bool isDupe(const vector<vector<rate> > &list, const vector<rate> &arb)
{
  if (list.size() == 0)
    return false;

  unordered_map<string, bool> u = {};
  for (auto r : arb)
    u[r.from + "|" + r.to + "|" + r.exchange] = true;

  for (auto v : list) {
    for (auto r : v) {
      if (!u[r.from + "|" + r.to + "|" + r.exchange]) {
        // note: break the inner loop
        goto cnt;
      }
    }
    return true;
    cnt:;
  }

  return false;
}
