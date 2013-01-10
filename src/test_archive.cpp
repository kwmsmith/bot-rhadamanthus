#include "arimaa_archive.h"
#include "gtest/gtest.h"

TEST(ArimaaArchive, read_header) {
    ArimaaArchive a;
    EXPECT_EQ(a.init("fake_archive.txt"), 1);
    EXPECT_EQ(a.read_header(), 1);
}
