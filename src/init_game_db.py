from pymongo import MongoClient
from arimaa_archive import games_from_archive
from glob import glob

archived_file = glob('/Users/ksmith/arimaa/archived-games/*')

db = MongoClient().game_db.game_db

for af in archived_file:
    print af
    gmes = games_from_archive(af)
    while True:
        try:
            for gm in gmes:
                db.insert(gm)
        except UnicodeDecodeError:
            continue
        else:
            break

