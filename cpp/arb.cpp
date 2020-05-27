#include "arb.hpp"

namespace arb {

vector< vector< vector<rate*> > > arbFromRates(const vector<rate*> &rates, const unsigned int depth)
{
  return arbFromCombos(combosFromRates(rates, depth));
}

vector< vector< vector<rate*> > > combosFromRates(const vector<rate*> &rates, const unsigned int depth)
{
  vector< vector< vector<rate*> > > combos(depth);
  buildBase(rates, combos.at(0));

  int i, j, k;
  for (i = 1; i < depth; ++i) {
    for (j = 0; j < combos.at(i-1).size(); ++j) {
      for (k = 0; k < rates.size(); ++k) {
        if (combos.at(i-1).at(j).back()->to == rates.at(k)->from &&
        //!find(combos.at(i-1).at(j).begin(), combos.at(i-1).at(j).end(), rates.at(k)) != combos.at(i-1).at(j).end() &&
        !isRateInList(combos.at(i-1).at(j), rates.at(k)) &&
        !isListClosing(combos.at(i-1).at(j))) {
          vector<rate*> newV(combos.at(i-1).at(j));
          newV.push_back(rates.at(k));
          combos.at(i).push_back(newV);
        }
      }
    }
  }

  return combos;
}

vector< vector< vector<rate*> > > arbFromCombos(const vector< vector< vector<rate*> > > &combos)
{
  vector< vector< vector<rate*> > > arb(combos.size());
  int i, j;
  for (i = 0; i < combos.size(); ++i) {
    for (j = 0; j < combos.at(i).size(); ++j) {
      if (isArb(combos.at(i).at(j)) && !isDupe(arb.at(i), combos.at(i).at(j)))
          arb.at(i).push_back(combos.at(i).at(j));
    }
  }

  return arb;
}

void buildBase(const vector<rate*> &rates, vector< vector<rate*> > &ret)
{
  int i, j;
  for (i = 0; i < rates.size(); ++i) {
    for (j = i + 1; j < rates.size(); ++j) {
      if (rates.at(i)->to == rates.at(j)->from) {
        vector<rate*> v = {
          rates.at(i),
          rates.at(j),
        };

        ret.push_back(v);
      }
    }
  }
}

bool isListClosing(const vector<rate*> &list)
{
  return list.front()->from == list.back()->to;
}

bool isRateInList(const vector<rate*> &list, const rate *r)
{
  int i;
  for (auto &l: list) {
    if (l == r)
      return true;
  }

  return false;
}

bool isArb(const vector<rate*> &list)
{
  if (list.size() < 2)
    return false;

  if (list.front()->from != list.back()->to)
    return false;

  double prod = list.front()->rate;
  int i;
  for (i = 1; i < list.size(); ++i) {
    if (list.at(i-1)->to != list.at(i)->from)
      return false;

    prod *= list.at(i)->rate;
  }

  return prod > 1.0;
}

bool isDupe(const vector<vector<rate*> > &list, const vector<rate*> &arb)
{
  if (list.size() == 0)
    return false;

  unordered_set<rate*> u = {};
  for (auto &r : arb)
    u.insert(r);

  for (auto &v : list) {
    for (auto &r : v) {
      if (u.find(r) == u.end())
        goto cnt;
    }
    return true;
    cnt:;
  }

  return false;
}

} // namespace arb
