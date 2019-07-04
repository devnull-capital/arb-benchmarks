#include "arb.hpp"
#include "gtest/gtest.h"

namespace {

class ArbTest : public ::testing::Test {
  protected:
    //void SetUp() override {}

    //void TearDown() override {}
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
};

TEST_F(ArbTest, IsRateInList) {
  rate r0 = rate{"foo", "bar", "baz", 0.0, 2.0};

  EXPECT_EQ(isRateInList(rates, r1), true);
  EXPECT_EQ(isRateInList(rates, r2), true);
  EXPECT_EQ(isRateInList(rates, r0), false);
}

TEST_F(ArbTest, BuildBase) {
  rate r30 = rate{"a", "b", "baz", 0.0, 2.0};
  rate r31 = rate{"b", "c", "baz", 0.0, 2.0};
  rate r32 = rate{"c", "d", "baz", 0.0, 2.0};
  rate r33 = rate{"y", "z", "baz", 0.0, 2.0};
  vector<rate> tmpRates = {
    r30,
    r31,
    r32,
    r33,
  };
  vector< vector<rate> > base;

  buildBase(tmpRates, base);

  EXPECT_EQ(base.size(), 2);
  EXPECT_EQ(base.at(0).size(), 2);
  EXPECT_EQ(base.at(1).size(), 2);
}

TEST_F(ArbTest, CombosFromRates) {
  rate r30 = rate{"a", "b", "baz", 0.0, 2.0};
  rate r31 = rate{"b", "c", "baz", 0.0, 2.0};
  rate r32 = rate{"c", "d", "baz", 0.0, 2.0};
  rate r33 = rate{"y", "z", "baz", 0.0, 2.0};
  vector<rate> tmpRates = {
    r30,
    r31,
    r32,
    r33,
  };

  auto combos = combosFromRates(tmpRates, 4);

  EXPECT_EQ(combos.size(), 4);
  EXPECT_EQ(combos.at(0).size(), 2);
  EXPECT_EQ(combos.at(0).at(0).size(), 2);
  EXPECT_EQ(combos.at(0).at(1).size(), 2);
  EXPECT_EQ(combos.at(1).size(), 1);
  EXPECT_EQ(combos.at(1).at(0).size(), 3);
}

TEST_F(ArbTest, IsArb) {
  vector<rate> expFalse1 = {
    rate{"a", "b", "baz", 0.0, 2.0},
  };
  vector<rate> expFalse2 = {
    rate{"a", "b", "baz", 0.0, 2.0},
    rate{"y", "z", "baz", 0.0, 2.0},
  };
  vector<rate> expFalse3 = {
    rate{"a", "b", "baz", 0.0, 2.0},
    rate{"b", "c", "baz", 0.0, 2.0},
  };
  vector<rate> expFalse4 = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "c", "baz", 2.0, 2.0},
  };
  vector<rate> expFalse5 = {
    rate{"a", "b", "baz", 1.0, 2.0},
    rate{"b", "a", "baz", 1.0, 2.0},
  };
  vector<rate> expTrue1 = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "a", "baz", 2.0, 2.0},
  };
  vector<rate> expTrue2 = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "c", "baz", 2.0, 2.0},
    rate{"c", "a", "baz", 2.0, 2.0},
  };

  EXPECT_EQ(isArb(expFalse1), false);
  EXPECT_EQ(isArb(expFalse2), false);
  EXPECT_EQ(isArb(expFalse3), false);
  EXPECT_EQ(isArb(expFalse4), false);
  EXPECT_EQ(isArb(expFalse5), false);
  EXPECT_EQ(isArb(expTrue1), true);
  EXPECT_EQ(isArb(expTrue2), true);
}

TEST_F(ArbTest, ArbFromCombos) {
  vector<rate> combos = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "c", "baz", 2.0, 2.0},
    rate{"c", "a", "baz", 2.0, 2.0},
    rate{"y", "z", "baz", 2.0, 2.0},
  };

  auto arb = arbFromCombos(combosFromRates(combos, 4));

  EXPECT_EQ(arb.size(), 4);
  EXPECT_EQ(arb.at(0).size(), 0);
  EXPECT_EQ(arb.at(1).size(), 1);
  EXPECT_EQ(arb.at(1).at(0).size(), 3);
  EXPECT_EQ(arb.at(2).size(), 0);
  EXPECT_EQ(arb.at(3).size(), 0);
}

TEST_F(ArbTest, IsDupe) {
  vector<rate> l1 = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "c", "baz", 2.0, 2.0},
    rate{"c", "a", "baz", 2.0, 2.0},
  };
  vector<rate> l2 = {
    rate{"b", "c", "baz", 2.0, 2.0},
    rate{"c", "a", "baz", 2.0, 2.0},
    rate{"a", "b", "baz", 2.0, 2.0},
  };
  vector<rate> l3 = {
    rate{"b", "c", "baz", 2.0, 2.0},
    rate{"c", "d", "baz", 2.0, 2.0},
    rate{"d", "b", "baz", 2.0, 2.0},
  };
  vector<rate> l4 = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "a", "baz", 2.0, 2.0},
  };
  vector<rate> l5 = {
    rate{"b", "a", "baz", 2.0, 2.0},
    rate{"a", "b", "baz", 2.0, 2.0},
  };
  vector<rate> l6 = {
    rate{"b", "c", "baz", 2.0, 2.0},
    rate{"c", "b", "baz", 2.0, 2.0},
  };

  vector< vector<rate> > arb1 = {l1,};
  vector< vector<rate> > arb2 = {l1,l2,l3,};
  vector< vector<rate> > arb3 = {l2,l3,};
  vector< vector<rate> > arb4 = {l3,};
  vector< vector<rate> > arb5 = {l4,};

  EXPECT_EQ(isDupe(arb1, l1), true);
  EXPECT_EQ(isDupe(arb2, l1), true);
  EXPECT_EQ(isDupe(arb3, l1), true);
  EXPECT_EQ(isDupe(arb4, l1), false);
  EXPECT_EQ(isDupe(arb5, l4), true);
  EXPECT_EQ(isDupe(arb5, l5), true);
  EXPECT_EQ(isDupe(arb5, l6), false);
}

TEST_F(ArbTest, IsListClosing) {
  vector<rate> l1 = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "a", "baz", 2.0, 2.0},
  };
  vector<rate> l2 = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "c", "baz", 2.0, 2.0},
    rate{"y", "z", "baz", 2.0, 2.0},
  };
  vector<rate> l3 = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "c", "baz", 2.0, 2.0},
    rate{"c", "a", "baz", 2.0, 2.0},
  };
  vector<rate> l4 = {
    rate{"a", "b", "baz", 2.0, 2.0},
    rate{"b", "c", "baz", 2.0, 2.0},
  };

  EXPECT_EQ(isListClosing(l1), true);
  EXPECT_EQ(isListClosing(l2), false);
  EXPECT_EQ(isListClosing(l3), true);
  EXPECT_EQ(isListClosing(l4), false);
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
