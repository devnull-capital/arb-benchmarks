#include "arb.hpp"
#include "gtest/gtest.h"

namespace {

class ArbTest : public ::testing::Test {
  protected:
    //void SetUp() override {}

    //void TearDown() override {}
};

TEST_F(ArbTest, IsRateInList) {
  arb::rate *r0 = new arb::rate{"foo", "bar", "baz", 0.0, 2.0};
  arb::rate *r1 = new arb::rate{"a", "b", "foo", 2.0, 0.0};
  arb::rate *r2 = new arb::rate{"a", "c", "foo", 2.0, 0.0};
  arb::rate *r3 = new arb::rate{"a", "d", "foo", 2.0, 0.0};
  arb::rate *r4 = new arb::rate{"a", "e", "foo", 2.0, 0.0};
  arb::rate *r5 = new arb::rate{"b", "a", "foo", 2.0, 0.0};
  arb::rate *r6 = new arb::rate{"b", "c", "foo", 2.0, 0.0};
  arb::rate *r7 = new arb::rate{"b", "d", "foo", 2.0, 0.0};
  arb::rate *r8 = new arb::rate{"b", "e", "foo", 2.0, 0.0};
  arb::rate *r9 = new arb::rate{"c", "a", "foo", 2.0, 0.0};
  arb::rate *r10 = new arb::rate{"c", "b", "foo", 2.0, 0.0};
  arb::rate *r11 = new arb::rate{"c", "d", "foo", 2.0, 0.0};
  arb::rate *r12 = new arb::rate{"c", "e", "foo", 2.0, 0.0};
  arb::rate *r13 = new arb::rate{"d", "a", "foo", 2.0, 0.0};
  arb::rate *r14 = new arb::rate{"d", "b", "foo", 2.0, 0.0};
  arb::rate *r15 = new arb::rate{"d", "c", "foo", 2.0, 0.0};
  arb::rate *r16 = new arb::rate{"d", "e", "foo", 2.0, 0.0};
  arb::rate *r17 = new arb::rate{"e", "a", "foo", 2.0, 0.0};
  arb::rate *r18 = new arb::rate{"e", "b", "foo", 2.0, 0.0};
  arb::rate *r19 = new arb::rate{"e", "c", "foo", 2.0, 0.0};
  arb::rate *r20 = new arb::rate{"e", "d", "foo", 2.0, 0.0};
  
  vector<arb::rate*> rates = {
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

  EXPECT_EQ(arb::isRateInList(rates, r1), true);
  EXPECT_EQ(arb::isRateInList(rates, r2), true);
  EXPECT_EQ(arb::isRateInList(rates, r0), false);

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
  arb::rate *r30 = new arb::rate{"a", "b", "baz", 0.0, 2.0};
  arb::rate *r31 = new arb::rate{"b", "c", "baz", 0.0, 2.0};
  arb::rate *r32 = new arb::rate{"c", "d", "baz", 0.0, 2.0};
  arb::rate *r33 = new arb::rate{"y", "z", "baz", 0.0, 2.0};
  vector<arb::rate*> tmpRates = {
    r30,
    r31,
    r32,
    r33,
  };
  vector< vector<arb::rate*> > base;

  arb::buildBase(tmpRates, base);

  EXPECT_EQ(base.size(), 2);
  EXPECT_EQ(base.at(0).size(), 2);
  EXPECT_EQ(base.at(1).size(), 2);

  delete r30;
  delete r31;
  delete r32;
  delete r33;
}

TEST_F(ArbTest, CombosFromRates) {
  arb::rate *r30 = new arb::rate{"a", "b", "baz", 0.0, 2.0};
  arb::rate *r31 = new arb::rate{"b", "c", "baz", 0.0, 2.0};
  arb::rate *r32 = new arb::rate{"c", "d", "baz", 0.0, 2.0};
  arb::rate *r33 = new arb::rate{"y", "z", "baz", 0.0, 2.0};
  vector<arb::rate*> tmpRates = {
    r30,
    r31,
    r32,
    r33,
  };

  auto combos = arb::combosFromRates(tmpRates, 4);

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
  arb::rate *r1 = new arb::rate{"a", "b", "baz", 0.0, 2.0};
  arb::rate *r2 = new arb::rate{"y", "z", "baz", 0.0, 2.0};
  arb::rate *r3 = new arb::rate{"b", "c", "baz", 0.0, 2.0};
  arb::rate *r4 = new arb::rate{"a", "b", "baz", 2.0, 2.0};
  arb::rate *r5 = new arb::rate{"b", "c", "baz", 2.0, 2.0};
  arb::rate *r6 = new arb::rate{"a", "b", "baz", 1.0, 2.0};
  arb::rate *r7 = new arb::rate{"b", "a", "baz", 1.0, 2.0};
  arb::rate *r8 = new arb::rate{"b", "a", "baz", 2.0, 2.0};
  arb::rate *r9 = new arb::rate{"c", "a", "baz", 2.0, 2.0};

  vector<arb::rate*> expFalse1 = {
    r1,
  };
  vector<arb::rate*> expFalse2 = {
    r1,
    r2,
  };
  vector<arb::rate*> expFalse3 = {
    r1,
    r3,
  };
  vector<arb::rate*> expFalse4 = {
    r4,
    r5,
  };
  vector<arb::rate*> expFalse5 = {
    r6,
    r7,
  };
  vector<arb::rate*> expTrue1 = {
    r4,
    r8,
  };
  vector<arb::rate*> expTrue2 = {
    r4,
    r5,
    r9,
  };

  EXPECT_EQ(arb::isArb(expFalse1), false);
  EXPECT_EQ(arb::isArb(expFalse2), false);
  EXPECT_EQ(arb::isArb(expFalse3), false);
  EXPECT_EQ(arb::isArb(expFalse4), false);
  EXPECT_EQ(arb::isArb(expFalse5), false);
  EXPECT_EQ(arb::isArb(expTrue1), true);
  EXPECT_EQ(arb::isArb(expTrue2), true);

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
  arb::rate *r1 = new arb::rate{"a", "b", "baz", 2.0, 2.0};
  arb::rate *r2 = new arb::rate{"b", "c", "baz", 2.0, 2.0};
  arb::rate *r3 = new arb::rate{"c", "a", "baz", 2.0, 2.0};
  arb::rate *r4 = new arb::rate{"y", "z", "baz", 2.0, 2.0};

  vector<arb::rate*> combos = {
    r1,
    r2,
    r3,
    r4,
  };

  auto arbitrage = arb::arbFromCombos(arb::combosFromRates(combos, 4));

  EXPECT_EQ(arbitrage.size(), 4);
  EXPECT_EQ(arbitrage.at(0).size(), 0);
  EXPECT_EQ(arbitrage.at(1).size(), 1);
  EXPECT_EQ(arbitrage.at(1).at(0).size(), 3);
  EXPECT_EQ(arbitrage.at(2).size(), 0);
  EXPECT_EQ(arbitrage.at(3).size(), 0);

  delete r1;
  delete r2;
  delete r3;
  delete r4;
}

TEST_F(ArbTest, IsDupe) {
  arb::rate *r1 = new arb::rate{"a", "b", "baz", 2.0, 2.0};
  arb::rate *r2 = new arb::rate{"b", "c", "baz", 2.0, 2.0};
  arb::rate *r3 = new arb::rate{"c", "a", "baz", 2.0, 2.0};
  arb::rate *r4 = new arb::rate{"d", "b", "baz", 2.0, 2.0};
  arb::rate *r5 = new arb::rate{"c", "d", "baz", 2.0, 2.0};
  arb::rate *r6 = new arb::rate{"b", "a", "baz", 2.0, 2.0};
  arb::rate *r7 = new arb::rate{"c", "b", "baz", 2.0, 2.0};

  vector<arb::rate*> l1 = {
    r1,
    r2,
    r3,
  };
  vector<arb::rate*> l2 = {
    r2,
    r3,
    r1,
  };
  vector<arb::rate*> l3 = {
    r2,
    r5,
    r4,
  };
  vector<arb::rate*> l4 = {
    r1,
    r6,
  };
  vector<arb::rate*> l5 = {
    r6,
    r1,
  };
  vector<arb::rate*> l6 = {
    r2,
    r7,
  };

  vector< vector<arb::rate*> > arb1 = {l1,};
  vector< vector<arb::rate*> > arb2 = {l1,l2,l3,};
  vector< vector<arb::rate*> > arb3 = {l2,l3,};
  vector< vector<arb::rate*> > arb4 = {l3,};
  vector< vector<arb::rate*> > arb5 = {l4,};

  EXPECT_EQ(arb::isDupe(arb1, l1), true);
  EXPECT_EQ(arb::isDupe(arb2, l1), true);
  EXPECT_EQ(arb::isDupe(arb3, l1), true);
  EXPECT_EQ(arb::isDupe(arb4, l1), false);
  EXPECT_EQ(arb::isDupe(arb5, l4), true);
  EXPECT_EQ(arb::isDupe(arb5, l5), true);
  EXPECT_EQ(arb::isDupe(arb5, l6), false);

  delete r1;
  delete r2;
  delete r3;
  delete r4;
  delete r5;
  delete r6;
  delete r7;
}

TEST_F(ArbTest, IsListClosing) {

  arb::rate *r1 = new arb::rate{"a", "b", "baz", 2.0, 2.0};
  arb::rate *r2 = new arb::rate{"b", "a", "baz", 2.0, 2.0};
  arb::rate *r3 = new arb::rate{"b", "c", "baz", 2.0, 2.0};
  arb::rate *r4 = new arb::rate{"y", "z", "baz", 2.0, 2.0};
  arb::rate *r5 = new arb::rate{"c", "a", "baz", 2.0, 2.0};

  vector<arb::rate*> l1 = {
    r1,
    r2,
  };
  vector<arb::rate*> l2 = {
    r1,
    r3,
    r4,
  };
  vector<arb::rate*> l3 = {
    r1,
    r3,
    r5
  };
  vector<arb::rate*> l4 = {
    r1,
    r3,
  };

  EXPECT_EQ(arb::isListClosing(l1), true);
  EXPECT_EQ(arb::isListClosing(l2), false);
  EXPECT_EQ(arb::isListClosing(l3), true);
  EXPECT_EQ(arb::isListClosing(l4), false);

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
