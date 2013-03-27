#include "arimaa_archive.h"
#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

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
            it != canon.end(); ++it) {
        EXPECT_EQ(mp.count(*it), 1);
    }
}

TEST(ArimaaArchive, get_record_setup) {
    ArimaaArchive a;
    EXPECT_EQ(a.init("fake_archive.txt"), 1);
    const map_ss mp = a.get_record();
}

TEST(ArimaaArchive, make_archive_game) {
    ArimaaArchive a;
    EXPECT_EQ(a.init("fake_archive.txt"), 1);
    ArchivedGame g(a.get_record());
    EXPECT_EQ(g.get_numply(), 129);
}

TEST(ArimaaArchive, play_archive_game) {
    ArimaaArchive a;
    a.init("fake_archive.txt");
    ArchivedGame g(a.get_record());
    GameState gs;
    EXPECT_TRUE(setup_archive_game(g, &gs));
    gs.clear();
    EXPECT_TRUE(play_archive_game(g, &gs));
}

TEST(ArimaaArchive, mobile_pieces) {
    ArimaaArchive a;
    a.init("fake_archive.txt");
    ArchivedGame g(a.get_record());
    GameState gs;
    EXPECT_TRUE(setup_archive_game(g, &gs));
    GameState mobile_w, mobile_b;
    gs.copy_to(&mobile_w);
    mobile_w.apply_mask(mobile_pieces(gs, W));
    gs.copy_to(&mobile_b);
    mobile_b.apply_mask(mobile_pieces(gs, B));
    // std::cout << gs.to_std_string() << std::endl;
    // std::cout << mobile_w.to_std_string() << std::endl;
    // std::cout << mobile_b.to_std_string() << std::endl;
}
