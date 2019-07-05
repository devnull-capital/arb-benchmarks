#include "arb.hpp"
#include "gtest/gtest.h"

namespace {

class ArbTest : public ::testing::Test {
  protected:
    //void SetUp() override {}

    //void TearDown() override {}
};

TEST_F(ArbTest, IsRateInList) {
  rate *r0 = new rate{"foo", "bar", "baz", 0.0, 2.0};
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

  EXPECT_EQ(isRateInList(rates, r1), true);
  EXPECT_EQ(isRateInList(rates, r2), true);
  EXPECT_EQ(isRateInList(rates, r0), false);

  delete r0;
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
}

TEST_F(ArbTest, BuildBase) {
  rate *r30 = new rate{"a", "b", "baz", 0.0, 2.0};
  rate *r31 = new rate{"b", "c", "baz", 0.0, 2.0};
  rate *r32 = new rate{"c", "d", "baz", 0.0, 2.0};
  rate *r33 = new rate{"y", "z", "baz", 0.0, 2.0};
  vector<rate*> tmpRates = {
    r30,
    r31,
    r32,
    r33,
  };
  vector< vector<rate*> > base;

  buildBase(tmpRates, base);

  EXPECT_EQ(base.size(), 2);
  EXPECT_EQ(base.at(0).size(), 2);
  EXPECT_EQ(base.at(1).size(), 2);

  delete r30;
  delete r31;
  delete r32;
  delete r33;
}

TEST_F(ArbTest, CombosFromRates) {
  rate *r30 = new rate{"a", "b", "baz", 0.0, 2.0};
  rate *r31 = new rate{"b", "c", "baz", 0.0, 2.0};
  rate *r32 = new rate{"c", "d", "baz", 0.0, 2.0};
  rate *r33 = new rate{"y", "z", "baz", 0.0, 2.0};
  vector<rate*> tmpRates = {
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

  delete r30;
  delete r31;
  delete r32;
  delete r33;
}

TEST_F(ArbTest, IsArb) {
  rate *r1 = new rate{"a", "b", "baz", 0.0, 2.0};
  rate *r2 = new rate{"y", "z", "baz", 0.0, 2.0};
  rate *r3 = new rate{"b", "c", "baz", 0.0, 2.0};
  rate *r4 = new rate{"a", "b", "baz", 2.0, 2.0};
  rate *r5 = new rate{"b", "c", "baz", 2.0, 2.0};
  rate *r6 = new rate{"a", "b", "baz", 1.0, 2.0};
  rate *r7 = new rate{"b", "a", "baz", 1.0, 2.0};
  rate *r8 = new rate{"b", "a", "baz", 2.0, 2.0};
  rate *r9 = new rate{"c", "a", "baz", 2.0, 2.0};

  vector<rate*> expFalse1 = {
    r1,
  };
  vector<rate*> expFalse2 = {
    r1,
    r2,
  };
  vector<rate*> expFalse3 = {
    r1,
    r3,
  };
  vector<rate*> expFalse4 = {
    r4,
    r5,
  };
  vector<rate*> expFalse5 = {
    r6,
    r7,
  };
  vector<rate*> expTrue1 = {
    r4,
    r8,
  };
  vector<rate*> expTrue2 = {
    r4,
    r5,
    r9,
  };

  EXPECT_EQ(isArb(expFalse1), false);
  EXPECT_EQ(isArb(expFalse2), false);
  EXPECT_EQ(isArb(expFalse3), false);
  EXPECT_EQ(isArb(expFalse4), false);
  EXPECT_EQ(isArb(expFalse5), false);
  EXPECT_EQ(isArb(expTrue1), true);
  EXPECT_EQ(isArb(expTrue2), true);

  delete r1;
  delete r2;
  delete r3;
  delete r4;
  delete r5;
  delete r6;
  delete r7;
  delete r8;
  delete r9;
}

TEST_F(ArbTest, ArbFromCombos) {
  rate *r1 = new rate{"a", "b", "baz", 2.0, 2.0};
  rate *r2 = new rate{"b", "c", "baz", 2.0, 2.0};
  rate *r3 = new rate{"c", "a", "baz", 2.0, 2.0};
  rate *r4 = new rate{"y", "z", "baz", 2.0, 2.0};

  vector<rate*> combos = {
    r1,
    r2,
    r3,
    r4,
  };

  auto arb = arbFromCombos(combosFromRates(combos, 4));

  EXPECT_EQ(arb.size(), 4);
  EXPECT_EQ(arb.at(0).size(), 0);
  EXPECT_EQ(arb.at(1).size(), 1);
  EXPECT_EQ(arb.at(1).at(0).size(), 3);
  EXPECT_EQ(arb.at(2).size(), 0);
  EXPECT_EQ(arb.at(3).size(), 0);

  delete r1;
  delete r2;
  delete r3;
  delete r4;
}

TEST_F(ArbTest, IsDupe) {
  rate *r1 = new rate{"a", "b", "baz", 2.0, 2.0};
  rate *r2 = new rate{"b", "c", "baz", 2.0, 2.0};
  rate *r3 = new rate{"c", "a", "baz", 2.0, 2.0};
  rate *r4 = new rate{"d", "b", "baz", 2.0, 2.0};
  rate *r5 = new rate{"c", "d", "baz", 2.0, 2.0};
  rate *r6 = new rate{"b", "a", "baz", 2.0, 2.0};
  rate *r7 = new rate{"c", "b", "baz", 2.0, 2.0};

  vector<rate*> l1 = {
    r1,
    r2,
    r3,
  };
  vector<rate*> l2 = {
    r2,
    r3,
    r1,
  };
  vector<rate*> l3 = {
    r2,
    r5,
    r4,
  };
  vector<rate*> l4 = {
    r1,
    r6,
  };
  vector<rate*> l5 = {
    r6,
    r1,
  };
  vector<rate*> l6 = {
    r2,
    r7,
  };

  vector< vector<rate*> > arb1 = {l1,};
  vector< vector<rate*> > arb2 = {l1,l2,l3,};
  vector< vector<rate*> > arb3 = {l2,l3,};
  vector< vector<rate*> > arb4 = {l3,};
  vector< vector<rate*> > arb5 = {l4,};

  EXPECT_EQ(isDupe(arb1, l1), true);
  EXPECT_EQ(isDupe(arb2, l1), true);
  EXPECT_EQ(isDupe(arb3, l1), true);
  EXPECT_EQ(isDupe(arb4, l1), false);
  EXPECT_EQ(isDupe(arb5, l4), true);
  EXPECT_EQ(isDupe(arb5, l5), true);
  EXPECT_EQ(isDupe(arb5, l6), false);

  delete r1;
  delete r2;
  delete r3;
  delete r4;
  delete r5;
  delete r6;
  delete r7;
}

TEST_F(ArbTest, IsListClosing) {

  rate *r1 = new rate{"a", "b", "baz", 2.0, 2.0};
  rate *r2 = new rate{"b", "a", "baz", 2.0, 2.0};
  rate *r3 = new rate{"b", "c", "baz", 2.0, 2.0};
  rate *r4 = new rate{"y", "z", "baz", 2.0, 2.0};
  rate *r5 = new rate{"c", "a", "baz", 2.0, 2.0};

  vector<rate*> l1 = {
    r1,
    r2,
  };
  vector<rate*> l2 = {
    r1,
    r3,
    r4,
  };
  vector<rate*> l3 = {
    r1,
    r3,
    r5
  };
  vector<rate*> l4 = {
    r1,
    r3,
  };

  EXPECT_EQ(isListClosing(l1), true);
  EXPECT_EQ(isListClosing(l2), false);
  EXPECT_EQ(isListClosing(l3), true);
  EXPECT_EQ(isListClosing(l4), false);

  delete r1;
  delete r2;
  delete r3;
  delete r4;
  delete r5;
}

} // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
