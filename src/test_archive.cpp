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
    // vector<string> moves = get_record_setup(mp, B);
    // EXPECT_EQ(moves.size(), 1);
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
    std::cout << gs.to_std_string() << std::endl;
    gs.clear();
    EXPECT_TRUE(play_archive_game(g, &gs));
    std::cout << gs.to_std_string() << std::endl;
}
