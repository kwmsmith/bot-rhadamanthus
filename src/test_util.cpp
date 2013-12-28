#include "util.h"
#include "gtest/gtest.h"

TEST(util, trim_copy) {
  EXPECT_EQ("foobar", trim_copy("   foobar   "));
  EXPECT_EQ("foo bar", trim_copy("   foo bar   "));
  EXPECT_EQ("foo bar", trim_copy("foo bar   "));
  EXPECT_EQ("foo bar", trim_copy("   foo bar"));
}

TEST(util, split) {
  std::vector<std::string> result;
  split(result, "   a      b    ", " ", false);
  std::vector<std::string> comp = {"a", "b"};
  EXPECT_EQ(comp, result);

  result.clear();

  split(result, "a|b", "|", false);
  EXPECT_EQ(comp, result);

  result.clear();
  comp = {"a"};
  split(result, "a", " ", false);
  EXPECT_EQ(comp, result);
}

TEST(util, to_lower) {
  std::string ss = "FOOBAR";
  to_lower(ss);
  EXPECT_EQ("foobar", ss);

  ss = "!@#$%^";
  EXPECT_EQ("!@#$%^", ss);
}
