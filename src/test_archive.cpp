#include "arimaa_archive.h"
#include "gtest/gtest.h"

TEST(ArimaaArchive, init) {
    ArimaaArchive a;
    EXPECT_EQ(a.init("fake_archive.txt"), 1);
}

TEST(ArimaaArchive, init_invalid) {
    ArimaaArchive a;
    EXPECT_EQ(a.init("empty_archive.txt"), 0);
}

TEST(ArimaaArchive, get_record) {
    ArimaaArchive a;
    EXPECT_EQ(a.init("fake_archive.txt"), 1);
    const map_ss mp = a.get_record();
    EXPECT_EQ(mp.size(), 29);
    std::vector<std::string> canon = ArimaaArchive::canonical_columns();
    EXPECT_EQ(canon.size(), 29);
    for (std::vector<std::string>::iterator it = canon.begin();
            it != canon.end(); ++it)
        EXPECT_EQ(mp.count(*it), 1);
}
